/****************************************************************************
 Copyright (c) 2012      greathqy
 Copyright (c) 2012      cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "network/HttpClient.h"
#include <queue>
#include <errno.h>
#include <curl/curl.h>
#include "base/CCDirector.h"
#include "platform/CCFileUtils.h"

namespace cocos2d {

namespace network {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
typedef int int32_t;
#endif

static HttpClient* _httpClient = nullptr; // pointer to singleton

typedef size_t (*write_callback)(void *ptr, size_t size, size_t nmemb, void *stream);

// Callback function used by libcurl for collect response data
static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}

// Callback function used by libcurl for collect header data
static size_t writeHeaderData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}


static int processGetTask(HttpClient* client, std::shared_ptr<HttpRequest> request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char* errorBuffer);
static int processPostTask(HttpClient* client, std::shared_ptr<HttpRequest> request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char* errorBuffer);
static int processPutTask(HttpClient* client, std::shared_ptr<HttpRequest> request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char* errorBuffer);
static int processDeleteTask(HttpClient* client, std::shared_ptr<HttpRequest> request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char* errorBuffer);

// Worker thread
void HttpClient::networkThread()
{   
	increaseThreadCount();
    
    while (true) 
    {
        std::shared_ptr<HttpRequest> request;

        // step 1: send http request if the requestQueue isn't empty
        {
            std::lock_guard<std::mutex> lock(_requestQueueMutex);
            while (_requestQueue.empty())
			{
                _sleepCondition.wait(_requestQueueMutex);
            }
            request = _requestQueue.front();
            _requestQueue.pop_front();
        }

        if (!request)
        {
            break;
        }

        // step 2: libcurl sync access
        
        // Create a HttpResponse object, the default setting is http access failed
        auto response = std::make_shared<HttpResponse>(request);
        
		processResponse(response, _responseMessage);

        // add response packet into queue
        {
            std::lock_guard<std::mutex> lock(_responseQueueMutex);
            _responseQueue.push_back(response);
        }
        
        {
            std::lock_guard<std::mutex> lock(_schedulerMutex);
            if (_scheduler)
            {
                _scheduler->performFunctionInCocosThread(CC_CALLBACK_0(HttpClient::dispatchResponseCallbacks, this));
            }
        }
    }
    
    // cleanup: if worker thread received quit signal, clean up un-completed request queue
    {
        std::lock_guard<std::mutex> lock(_requestQueueMutex);
        _requestQueue.clear();
    }
    
    {
        std::lock_guard<std::mutex> lock(_responseQueueMutex);
        _responseQueue.clear();
    }

	decreaseThreadCountAndMayDeleteThis();    
}

// Worker thread
void HttpClient::networkThreadAlone(std::shared_ptr<HttpRequest> request, std::shared_ptr<HttpResponse> response)
{
	increaseThreadCount();

	char responseMessage[RESPONSE_BUFFER_SIZE] = { 0 };
	processResponse(response, responseMessage);
	
    {
        std::lock_guard<std::mutex> lock(_schedulerMutex);

        if (_scheduler && request->getCallback())
        {
            _scheduler->performFunctionInCocosThread(
                [this, response, request]
                {
                    auto & callback = request->getCallback();
                    if (callback)
                    {
                        callback(this, response);
                    }
                }
            );
        }
    }

	decreaseThreadCountAndMayDeleteThis();
}

//Configure curl's timeout property
static bool configureCURL(HttpClient* client, CURL* handle, char* errorBuffer)
{
    if (!handle) {
        return false;
    }
    
    int32_t code;
    code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errorBuffer);
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_TIMEOUT, HttpClient::getInstance()->getTimeoutForRead());
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, HttpClient::getInstance()->getTimeoutForConnect());
    if (code != CURLE_OK) {
        return false;
    }

	std::string sslCaFilename = client->getSSLVerification();
	if (sslCaFilename.empty()) {
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);
    } else {
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 2L);
		curl_easy_setopt(handle, CURLOPT_CAINFO, sslCaFilename.c_str());
    }
    
    // FIXED #3224: The subthread of CCHttpClient interrupts main thread if timeout comes.
    // Document is here: http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTNOSIGNAL 
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);

    curl_easy_setopt(handle, CURLOPT_ACCEPT_ENCODING, "");

    return true;
}

class CURLRaii
{
    /// Instance of CURL
    CURL *_curl;
    /// Keeps custom header data
    curl_slist *_headers;
public:
    CURLRaii()
        : _curl(curl_easy_init())
        , _headers(nullptr)
    {
    }

    ~CURLRaii()
    {
        if (_curl)
            curl_easy_cleanup(_curl);
        /* free the linked list for header data */
        if (_headers)
            curl_slist_free_all(_headers);
    }

    template <class T>
    bool setOption(CURLoption option, T data)
    {
        return CURLE_OK == curl_easy_setopt(_curl, option, data);
    }

    /**
     * @brief Inits CURL instance for common usage
     * @param request Null not allowed
     * @param callback Response write callback
     * @param stream Response write stream
     */
    bool init(HttpClient* client, std::shared_ptr<HttpRequest> request, write_callback callback, void* stream, write_callback headerCallback, void* headerStream, char* errorBuffer)
    {
        if (!_curl)
            return false;
		if (!configureCURL(client, _curl, errorBuffer))
            return false;

        /* get custom header data (if set) */
        std::vector<std::string> headers = request->getHeaders();
        if(!headers.empty())
        {
            /* append custom headers one by one */
            for (auto& header : headers)
                _headers = curl_slist_append(_headers,header.c_str());
            /* set custom headers for curl */
            if (!setOption(CURLOPT_HTTPHEADER, _headers))
                return false;
        }
		std::string cookieFilename = client->getCookieFilename();
		if (!cookieFilename.empty()) {
			if (!setOption(CURLOPT_COOKIEFILE, cookieFilename.c_str())) {
                return false;
            }
			if (!setOption(CURLOPT_COOKIEJAR, cookieFilename.c_str())) {
                return false;
            }
        }

        return setOption(CURLOPT_URL, request->getUrl())
                && setOption(CURLOPT_WRITEFUNCTION, callback)
                && setOption(CURLOPT_WRITEDATA, stream)
                && setOption(CURLOPT_HEADERFUNCTION, headerCallback)
                && setOption(CURLOPT_HEADERDATA, headerStream);
        
    }

    /// @param responseCode Null not allowed
    bool perform(long *responseCode)
    {
        if (CURLE_OK != curl_easy_perform(_curl))
            return false;
        CURLcode code = curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, responseCode);
        if (code != CURLE_OK || !(*responseCode >= 200 && *responseCode < 300)) {
            CCLOGERROR("Curl curl_easy_getinfo failed: %s", curl_easy_strerror(code));
            return false;
        }
        // Get some mor data.
        
        return true;
    }
};

//Process Get Request
static int processGetTask(HttpClient* client, std::shared_ptr<HttpRequest> request, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
    CURLRaii curl;
	bool ok = curl.init(client, request, callback, stream, headerCallback, headerStream, errorBuffer)
            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process POST Request
static int processPostTask(HttpClient* client, std::shared_ptr<HttpRequest> request, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
    CURLRaii curl;
	bool ok = curl.init(client, request, callback, stream, headerCallback, headerStream, errorBuffer)
            && curl.setOption(CURLOPT_POST, 1)
            && curl.setOption(CURLOPT_POSTFIELDS, request->getRequestData())
            && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getRequestDataSize())
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process PUT Request
static int processPutTask(HttpClient* client, std::shared_ptr<HttpRequest> request, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
    CURLRaii curl;
	bool ok = curl.init(client, request, callback, stream, headerCallback, headerStream, errorBuffer)
            && curl.setOption(CURLOPT_CUSTOMREQUEST, "PUT")
            && curl.setOption(CURLOPT_POSTFIELDS, request->getRequestData())
            && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getRequestDataSize())
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process DELETE Request
static int processDeleteTask(HttpClient* client, std::shared_ptr<HttpRequest> request, write_callback callback, void* stream, long* responseCode, write_callback headerCallback, void* headerStream, char* errorBuffer)
{
    CURLRaii curl;
	bool ok = curl.init(client, request, callback, stream, headerCallback, headerStream, errorBuffer)
            && curl.setOption(CURLOPT_CUSTOMREQUEST, "DELETE")
            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

// HttpClient implementation
HttpClient* HttpClient::getInstance()
{
    if (_httpClient == nullptr) 
	{
        _httpClient = new (std::nothrow) HttpClient();
    }
    
    return _httpClient;
}

void HttpClient::destroyInstance()
{
	if (nullptr == _httpClient)
	{
		CCLOG("HttpClient singleton is nullptr");
		return;
	}

	CCLOG("HttpClient::destroyInstance begin");
	auto thiz = _httpClient;
	_httpClient = nullptr;

	thiz->_scheduler->unscheduleAllForTarget(thiz);

    {
        std::lock_guard<std::mutex> lock(thiz->_schedulerMutex);
        thiz->_scheduler = nullptr;
    }

	thiz->_requestQueueMutex.lock();
	thiz->_requestQueue.push_back(std::shared_ptr<HttpRequest>());
	thiz->_requestQueueMutex.unlock();

	thiz->_sleepCondition.notify_one();
	thiz->decreaseThreadCountAndMayDeleteThis();

	CCLOG("HttpClient::destroyInstance() finished!");
}

void HttpClient::enableCookies(const char* cookieFile)
{
    std::lock_guard<std::mutex> lock(_cookieFileMutex);
    if (cookieFile)
    {
        _cookieFilename = std::string(cookieFile);
    }
    else
    {
        _cookieFilename = (FileUtils::getInstance()->getWritablePath() + "cookieFile.txt");
    }
}
    
void HttpClient::setSSLVerification(const std::string& caFile)
{
    std::lock_guard<std::mutex> lock(_sslCaFileMutex);
    _sslCaFilename = caFile;
}

HttpClient::HttpClient()
: _isInited(false)
, _timeoutForConnect(30)
, _timeoutForRead(60)
, _threadCount(0)
, _cookie(nullptr)
{
	CCLOG("In the constructor of HttpClient!");
	memset(_responseMessage, 0, RESPONSE_BUFFER_SIZE * sizeof(char));
	_scheduler = & Director::getInstance()->getScheduler();
	increaseThreadCount();
}

HttpClient::~HttpClient()
{
	CCLOG("HttpClient destructor");
}

//Lazy create semaphore & mutex & thread
bool HttpClient::lazyInitThreadSemphore()
{
    if (!_isInited)
	{
        auto t = std::thread(CC_CALLBACK_0(HttpClient::networkThread, this));
        t.detach();
		_isInited = true;
    }
    
    return _isInited;
}

//Add a get task to queue
void HttpClient::send(std::shared_ptr<HttpRequest> request)
{    
    if (!lazyInitThreadSemphore()) 
    {
        return;
    }
    
    if (!request)
    {
        return;
    }
        
    {
        std::lock_guard<std::mutex> lock(_requestQueueMutex);
        _requestQueue.push_back(request);
    }

	// Notify thread start to work
	_sleepCondition.notify_one();
}

void HttpClient::sendImmediate(std::shared_ptr<HttpRequest> request)
{
    if(!request)
    {
        return;
    }

    // Create a HttpResponse object, the default setting is http access failed
    std::shared_ptr<HttpResponse> response( new (std::nothrow) HttpResponse(request) );

    auto t = std::thread(&HttpClient::networkThreadAlone, this, request, response);

    t.detach();
}

// Poll and notify main thread if responses exists in queue
void HttpClient::dispatchResponseCallbacks()
{
    // log("CCHttpClient::dispatchResponseCallbacks is running");
    //occurs when cocos thread fires but the network thread has already quited
    std::shared_ptr<HttpResponse> response;

    {
        std::lock_guard<std::mutex> lock(_responseQueueMutex);
        if (!_responseQueue.empty())
        {
            response = _responseQueue.front();
            _responseQueue.pop_front();
        }
    }

    if (response)
    {
        auto request = response->getHttpRequest();
        auto & callback = request->getCallback();

        if (callback)
        {
            callback(this, response);
        }
    }
}

// Process Response
void HttpClient::processResponse(std::shared_ptr<HttpResponse> response, char* responseMessage)
{
	auto request = response->getHttpRequest();
	long responseCode = -1;
	int retValue = 0;

	// Process the request -> get response packet
	switch (request->getRequestType())
	{
	case HttpRequest::Type::GET: // HTTP GET
		retValue = processGetTask(this, request,
			writeData,
			response->getResponseData(),
			&responseCode,
			writeHeaderData,
			response->getResponseHeader(),
			responseMessage);
		break;

	case HttpRequest::Type::POST: // HTTP POST
		retValue = processPostTask(this, request,
			writeData,
			response->getResponseData(),
			&responseCode,
			writeHeaderData,
			response->getResponseHeader(),
			responseMessage);
		break;

	case HttpRequest::Type::PUT:
		retValue = processPutTask(this, request,
			writeData,
			response->getResponseData(),
			&responseCode,
			writeHeaderData,
			response->getResponseHeader(),
			responseMessage);
		break;

	case HttpRequest::Type::DELETE:
		retValue = processDeleteTask(this, request,
			writeData,
			response->getResponseData(),
			&responseCode,
			writeHeaderData,
			response->getResponseHeader(),
			responseMessage);
		break;

	default:
		CCASSERT(true, "CCHttpClient: unknown request type, only GET and POSt are supported");
		break;
	}

	// write data to HttpResponse
	response->setResponseCode(responseCode);
	if (retValue != 0)
	{
		response->setSucceed(false);
		response->setErrorBuffer(responseMessage);
	}
	else
	{
		response->setSucceed(true);
	}
}

void HttpClient::increaseThreadCount()
{
    std::lock_guard<std::mutex> lock(_threadCountMutex);
	++_threadCount;
}

void HttpClient::decreaseThreadCountAndMayDeleteThis()
{
	bool needDeleteThis = false;
    {
        std::lock_guard<std::mutex> lock(_threadCountMutex);

        --_threadCount;
        if (0 == _threadCount)
        {
            needDeleteThis = true;
        }
    }
	if (needDeleteThis)
	{
		delete this;
	}
}

void HttpClient::setTimeoutForConnect(int value)
{
    std::lock_guard<std::mutex> lock(_timeoutForConnectMutex);
    _timeoutForConnect = value;
}
    
int HttpClient::getTimeoutForConnect()
{
    std::lock_guard<std::mutex> lock(_timeoutForConnectMutex);
    return _timeoutForConnect;
}
    
void HttpClient::setTimeoutForRead(int value)
{
    std::lock_guard<std::mutex> lock(_timeoutForReadMutex);
    _timeoutForRead = value;
}
    
int HttpClient::getTimeoutForRead()
{
    std::lock_guard<std::mutex> lock(_timeoutForReadMutex);
    return _timeoutForRead;
}
    
const std::string& HttpClient::getCookieFilename()
{
    std::lock_guard<std::mutex> lock(_cookieFileMutex);
    return _cookieFilename;
}
    
const std::string& HttpClient::getSSLVerification()
{
    std::lock_guard<std::mutex> lock(_sslCaFileMutex);
    return _sslCaFilename;
}

}

} // namespace cocos2d



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

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "network/HttpClient.h"

#include <queue>
#include <errno.h>

#import "network/HttpAsynConnection-apple.h"
#include "network/HttpCookie.h"
#include "base/CCDirector.h"
#include "platform/CCFileUtils.h"

namespace cocos2d {

namespace network {

static HttpClient *_httpClient = nullptr; // pointer to singleton

static int processTask(HttpClient* client, std::shared_ptr<HttpRequest> request, NSString *requestType, void *stream, long *errorCode, void *headerStream, char *errorBuffer);

// Worker thread
void HttpClient::networkThread()
{
    increaseThreadCount();
    
    while (true) @autoreleasepool {
        
        std::shared_ptr<HttpRequest> request;

        // step 1: send http request if the requestQueue isn't empty
        {
            std::lock_guard<std::mutex> lock(_requestQueueMutex);
            while (_requestQueue.empty()) {
                _sleepCondition.wait(_requestQueueMutex);
            }
            request = _requestQueue.front();
            _requestQueue.pop_front();
        }

        if (!request)
        {
            break;
        }
        
        // Create a HttpResponse object, the default setting is http access failed
        std::shared_ptr<HttpResponse> response(new (std::nothrow) HttpResponse(request));
        
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

                    if (callback != nullptr)
                    {
                        callback(this, response);
                    }
                }
            );
        }
    }

    decreaseThreadCountAndMayDeleteThis();
}

//Process Request
static int processTask(HttpClient* client, std::shared_ptr<HttpRequest> request, NSString* requestType, void* stream, long* responseCode, void* headerStream, char* errorBuffer)
{
    if (nullptr == client)
    {
        strcpy(errorBuffer, "client object is invalid");
        return 0;
    }
    
    //create request with url
    NSString* urlstring = [NSString stringWithUTF8String:request->getUrl()];
    NSURL *url = [NSURL URLWithString:urlstring];

    NSMutableURLRequest *nsrequest = [NSMutableURLRequest requestWithURL:url
                                               cachePolicy:NSURLRequestReloadIgnoringLocalAndRemoteCacheData
                                           timeoutInterval:HttpClient::getInstance()->getTimeoutForConnect()];
    
    //set request type
    [nsrequest setHTTPMethod:requestType];

    /* get custom header data (if set) */
    std::vector<std::string> headers=request->getHeaders();
    if(!headers.empty())
    {
        /* append custom headers one by one */
        for (auto& header : headers)
        {
            unsigned long i = header.find(':', 0);
            unsigned long length = header.size();
            std::string field = header.substr(0, i);
            std::string value = header.substr(i+1, length-i);
            NSString *headerField = [NSString stringWithUTF8String:field.c_str()];
            NSString *headerValue = [NSString stringWithUTF8String:value.c_str()];
            [nsrequest setValue:headerValue forHTTPHeaderField:headerField];
        }
    }

    //if request type is post or put,set header and data
    if([requestType  isEqual: @"POST"] || [requestType isEqual: @"PUT"])
    {   
        char* requestDataBuffer = request->getRequestData();
        if (nullptr !=  requestDataBuffer && 0 != request->getRequestDataSize())
        {
            NSData *postData = [NSData dataWithBytes:requestDataBuffer length:request->getRequestDataSize()];
            [nsrequest setHTTPBody:postData];
        }
    }

    //read cookie properties from file and set cookie
    std::string cookieFilename = client->getCookieFilename();
    if(!cookieFilename.empty() && nullptr != client->getCookie())
    {
        const CookiesInfo* cookieInfo = client->getCookie()->getMatchCookie(request->getUrl());
        if(cookieInfo != nullptr)
        {
            NSString *domain = [NSString stringWithCString:cookieInfo->domain.c_str() encoding:[NSString defaultCStringEncoding]];
            NSString *path = [NSString stringWithCString:cookieInfo->path.c_str() encoding:[NSString defaultCStringEncoding]];
            NSString *value = [NSString stringWithCString:cookieInfo->value.c_str() encoding:[NSString defaultCStringEncoding]];
            NSString *name = [NSString stringWithCString:cookieInfo->name.c_str() encoding:[NSString defaultCStringEncoding]];

            // create the properties for a cookie
            NSDictionary *properties = [NSDictionary dictionaryWithObjectsAndKeys: name,NSHTTPCookieName,
            value, NSHTTPCookieValue, path, NSHTTPCookiePath,
            domain, NSHTTPCookieDomain,
            nil];
            
            // create the cookie from the properties
            NSHTTPCookie *cookie = [NSHTTPCookie cookieWithProperties:properties];
            
            // add the cookie to the cookie storage
            [[NSHTTPCookieStorage sharedHTTPCookieStorage] setCookie:cookie];
        }
    }
    
    HttpAsynConnection *httpAsynConn = [[HttpAsynConnection new] autorelease];
    httpAsynConn.srcURL = urlstring;
    httpAsynConn.sslFile = nil;
    
    std::string sslCaFileName = client->getSSLVerification();
    if(!sslCaFileName.empty())
    {
        long len = sslCaFileName.length();
        long pos = sslCaFileName.rfind('.', len-1);
        
        httpAsynConn.sslFile = [NSString stringWithUTF8String:sslCaFileName.substr(0, pos).c_str()];
    }
    [httpAsynConn startRequest:nsrequest];
    
    while( httpAsynConn.finish != true)
    {
        [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
    }
    
    //if http connection return error
    if (httpAsynConn.connError != nil)
    {
        NSString* errorString = [httpAsynConn.connError localizedDescription];
        strcpy(errorBuffer, [errorString UTF8String]);
        return 0;
    }

    //if http response got error, just log the error
    if (httpAsynConn.responseError != nil)
    {
        NSString* errorString = [httpAsynConn.responseError localizedDescription];
        strcpy(errorBuffer, [errorString UTF8String]);
    }
    
    *responseCode = httpAsynConn.responseCode;
    
    //add cookie to cookies vector
    if(!cookieFilename.empty())
    {
        NSArray *cookies = [NSHTTPCookie cookiesWithResponseHeaderFields:httpAsynConn.responseHeader forURL:url];
        for (NSHTTPCookie *cookie in cookies)
        {
            //NSLog(@"Cookie: %@", cookie);
            NSString *domain = cookie.domain;
            //BOOL session = cookie.sessionOnly;
            NSString *path = cookie.path;
            BOOL secure = cookie.isSecure;
            NSDate *date = cookie.expiresDate;
            NSString *name = cookie.name;
            NSString *value = cookie.value;
            
            CookiesInfo cookieInfo;
            cookieInfo.domain = [domain cStringUsingEncoding: NSUTF8StringEncoding];
            cookieInfo.path = [path cStringUsingEncoding: NSUTF8StringEncoding];
            cookieInfo.secure = (secure == YES) ? true : false;
            cookieInfo.expires = [[NSString stringWithFormat:@"%ld", (long)[date timeIntervalSince1970]] cStringUsingEncoding: NSUTF8StringEncoding];
            cookieInfo.name = [name cStringUsingEncoding: NSUTF8StringEncoding];
            cookieInfo.value = [value cStringUsingEncoding: NSUTF8StringEncoding];
            cookieInfo.tailmatch = true;
            
            client->getCookie()->updateOrAddCookie(&cookieInfo);
        }
    }
    
    //handle response header
    NSMutableString *header = [NSMutableString string];
    [header appendFormat:@"HTTP/1.1 %ld %@\n", (long)httpAsynConn.responseCode, httpAsynConn.statusString];
    for (id key in httpAsynConn.responseHeader)
    {
        [header appendFormat:@"%@: %@\n", key, [httpAsynConn.responseHeader objectForKey:key]];
    }
    if (header.length > 0)
    {
        NSRange range = NSMakeRange(header.length-1, 1);
        [header deleteCharactersInRange:range];
    }
    NSData *headerData = [header dataUsingEncoding:NSUTF8StringEncoding];
    std::vector<char> *headerBuffer = (std::vector<char>*)headerStream;
    const void* headerptr = [headerData bytes];
    long headerlen = [headerData length];
    headerBuffer->insert(headerBuffer->end(), (char*)headerptr, (char*)headerptr+headerlen);

    //handle response data
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    const void* ptr = [httpAsynConn.responseData bytes];
    long len = [httpAsynConn.responseData length];
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+len);

    return 1;
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
    
    {
        std::lock_guard<std::mutex> lock(thiz->_requestQueueMutex);
        thiz->_requestQueue.push_back(std::shared_ptr<HttpRequest>());
    }


    thiz->_sleepCondition.notify_one();

    thiz->decreaseThreadCountAndMayDeleteThis();

    CCLOG("HttpClient::destroyInstance() finished!");
}

void HttpClient::enableCookies(const char* cookieFile)
{
    _cookieFileMutex.lock();
    if (cookieFile)
    {
        _cookieFilename = std::string(cookieFile);
        _cookieFilename = FileUtils::getInstance()->fullPathForFilename(_cookieFilename);
    }
    else
    {
        _cookieFilename = (FileUtils::getInstance()->getWritablePath() + "cookieFile.txt");
    }
    _cookieFileMutex.unlock();

    if (nullptr == _cookie)
    {
        _cookie = new(std::nothrow)HttpCookie;
    }
    _cookie->setCookieFileName(_cookieFilename);
    _cookie->readFile();
}

void HttpClient::setSSLVerification(const std::string& caFile)
{
    std::lock_guard<std::mutex> lock(_sslCaFileMutex);
    _sslCaFilename = caFile;
}

HttpClient::HttpClient()
: _timeoutForConnect(30)
, _timeoutForRead(60)
, _isInited(false)
, _threadCount(0)
, _cookie(nullptr)
{
    CCLOG("In the constructor of HttpClient!");
    memset(_responseMessage, 0, sizeof(char) * RESPONSE_BUFFER_SIZE);
    _scheduler = Director::getInstance()->getScheduler();
    increaseThreadCount();
}


HttpClient::~HttpClient()
{
    if (!_cookieFilename.empty() && nullptr != _cookie)
    {
        _cookie->writeFile();
        CC_SAFE_DELETE(_cookie);
    }
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

        if (callback != nullptr)
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
    NSString* requestType = nil;

    // Process the request -> get response packet
    switch (request->getRequestType())
    {
        case HttpRequest::Type::GET: // HTTP GET
            requestType = @"GET";
            break;

        case HttpRequest::Type::POST: // HTTP POST
            requestType = @"POST";
            break;

        case HttpRequest::Type::PUT:
            requestType = @"PUT";
            break;

        case HttpRequest::Type::DELETE:
            requestType = @"DELETE";
            break;

        default:
            CCASSERT(true, "CCHttpClient: unknown request type, only GET and POSt are supported");
            break;
    }

    retValue = processTask(this,
                           request,
                           requestType,
                           response->getResponseData(),
                           &responseCode,
                           response->getResponseHeader(),
                           responseMessage);

    // write data to HttpResponse
    response->setResponseCode(responseCode);

    if (retValue != 0)
    {
        response->setSucceed(true);
    }
    else
    {
        response->setSucceed(false);
        response->setErrorBuffer(responseMessage);
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

#endif // #if CC_TARGET_PLATFORM == CC_PLATFORM_MAC


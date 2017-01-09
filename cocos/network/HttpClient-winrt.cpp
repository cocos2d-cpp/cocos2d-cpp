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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)

#include "network/HttpClient.h"

#include <thread>
#include <deque>
#include <condition_variable>

#include <errno.h>

#include "base/CCVector.h"
#include "base/CCDirector.h"
#include "base/CCScheduler.h"

#include "platform/CCFileUtils.h"
#include "network/HttpConnection-winrt.h"

namespace cocos2d {

namespace network {

    static std::mutex       s_requestQueueMutex;
    static std::mutex       s_responseQueueMutex;

    static std::condition_variable_any s_SleepCondition;


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    typedef int int32_t;
#endif

    static std::deque<std::shared_ptr<HttpRequest>>  s_requestQueue;
    static std::deque<std::shared_ptr<HttpResponse>> s_responseQueue;

    static HttpClient *s_pHttpClient = nullptr; // pointer to singleton

    static std::string s_errorBuffer ="";

    static std::string s_cookieFilename = "";

    static std::string s_sslCaFilename = "";

    // function used to collect response/header data
    static void writeData(std::vector<char> *pFromBuffer, std::vector<char> *pToBuffer)
    {
        if(nullptr == pFromBuffer || nullptr == pToBuffer)
            return;

        pToBuffer->insert(pToBuffer->end(), pFromBuffer->begin(), pFromBuffer->end());
    }

    static void processHttpResponse(std::shared_ptr<HttpResponse>, std::string& errorStr);

    // Worker thread
    void HttpClient::networkThread()
    {
        auto scheduler = Director::getInstance()->getScheduler();

        while (true)
        {
            std::shared_ptr<HttpRequest> request;

            // step 1: send http request if the requestQueue isn't empty
            {
                std::lock_guard<std::mutex> lock(s_requestQueueMutex);
                while (s_requestQueue.empty()) {
                    s_SleepCondition.wait(s_requestQueueMutex);
                }
                request = std::shared_ptr<HttpRequest>( s_requestQueue.front().release() );
                s_requestQueue.erase(s_requestQueue.begin());
            }

            if (!request)
            {
                break;
            }

            // step 2: libcurl sync access

            // Create a HttpResponse object, the default setting is http access failed
            std::shared_ptr<HttpResponse> response( new (std::nothrow) HttpResponse(request) );

            processHttpResponse(response, s_errorBuffer);


            // add response packet into queue
            {
                std::lock_guard<std::mutex> lock(s_responseQueueMutex);
                s_responseQueue.push_back(response);
            }

            if (nullptr != s_pHttpClient) {
                scheduler->performFunctionInCocosThread(CC_CALLBACK_0(HttpClient::dispatchResponseCallbacks, this));
            }
        }

        // cleanup: if worker thread received quit signal, clean up un-completed request queue
        {
            std::lock_guard<std::mutex> lock(s_requestQueueMutex);
            s_requestQueue.clear();
        }
    }

    // Worker thread
    void HttpClient::networkThreadAlone(std::shared_ptr<HttpRequest> request, std::shared_ptr<HttpResponse> response)
    {
        std::string errorStr;
        processHttpResponse(response, errorStr);

        auto scheduler = Director::getInstance()->getScheduler();

        if (request->getCallback())
        {
            scheduler->performFunctionInCocosThread(
                [response, request]
                {
                    auto & callback = request->getCallback();

                    if (callback)
                    {
                        callback(s_pHttpClient, response);
                    }
                }
            );
        }
    }

    // Process Response
    static void processHttpResponse(std::shared_ptr<HttpResponse> response, std::string& errorStr)
    {
        auto request = response->getHttpRequest();
        long responseCode = -1;
        int retValue = 0;
        HttpConnection xhr;
        bool ok = false;
        bool manualAuthReqd = false;

        // Process the request -> get response packet
        switch (request->getRequestType())
        {
        case HttpRequest::Type::GET: // HTTP GET
            ok = (xhr.init(request) && xhr.open("GET", manualAuthReqd, s_cookieFilename) && xhr.send());
            break;

        case HttpRequest::Type::POST: // HTTP POST
            ok = (xhr.init(request) && xhr.open("POST", manualAuthReqd, s_cookieFilename) && xhr.send());
            break;

        case HttpRequest::Type::PUT: // HTTP PUT
            ok = (xhr.init(request) && xhr.open("PUT", manualAuthReqd, s_cookieFilename) && xhr.send());
            break;

        case HttpRequest::Type::DELETE: // HTTP DELETE
            ok = (xhr.init(request) && xhr.open("DELETE", manualAuthReqd, s_cookieFilename) && xhr.send());
            break;

        default:
            CCASSERT(true, "CCHttpClient: unknown request type, only GET and POST are supported");
            break;
        }

        writeData(xhr.getResponseHeader(), response->getResponseHeader());
        writeData(xhr.getResponseData(), response->getResponseData());
        retValue = ok ? 0 : 1;
        errorStr = xhr.getErrorMessage();
        responseCode = xhr.getStatusCode();

        // write data to HttpResponse
        response->setResponseCode(responseCode);

        if (retValue != 0)
        {
            response->setSucceed(false);
            response->setErrorBuffer(errorStr.c_str());
        }
        else
        {
            response->setSucceed(true);
        }
    }

    // HttpClient implementation
    HttpClient* HttpClient::getInstance()
    {
        if (s_pHttpClient == nullptr) {
            s_pHttpClient = new (std::nothrow) HttpClient();
        }

        return s_pHttpClient;
    }

    void HttpClient::destroyInstance()
    {
        CC_SAFE_DELETE(s_pHttpClient);
    }

    void HttpClient::enableCookies(const char* cookieFile) {
        if (cookieFile) {
            s_cookieFilename = std::string(cookieFile);
        }
        else {
            s_cookieFilename = (FileUtils::getInstance()->getWritablePath() + "cookieFile.txt");
        }
    }

    void HttpClient::setSSLVerification(const std::string& caFile)
    {
        s_sslCaFilename = caFile;
    }

    HttpClient::HttpClient()
        : _timeoutForConnect(30)
        , _timeoutForRead(60)
    {
    }

    HttpClient::~HttpClient()
    {
        {
            std::lock_guard<std::mutex> lock(s_requestQueueMutex);
            s_requestQueue.push_back( std::shared_ptr<HttpRequest>() );
        }
        s_SleepCondition.notify_one();

        s_pHttpClient = nullptr;
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
            std::lock_guard<std::mutex> lock(s_requestQueueMutex);
            s_requestQueue.push_back(request);
        }

        // Notify thread start to work
        s_SleepCondition.notify_one();
    }

    void HttpClient::sendImmediate(std::shared_ptr<HttpRequest> request)
    {
        if (!request)
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
            std::lock_guard<std::mutex> lock(s_responseQueueMutex);
            if (!s_responseQueue.empty())
            {
                response = s_responseQueue.at(0);
                s_responseQueue.erase( s_responseQueue.begin() );
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

    }

} // namespace cocos2d

#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)


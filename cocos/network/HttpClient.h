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

#ifndef __CCHTTPCLIENT_H__
#define __CCHTTPCLIENT_H__

#include "base/CCScheduler.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"
#include "network/HttpCookie.h"

#include <condition_variable>
#include <deque>
#include <thread>

/**
 * @addtogroup network
 * @{
 */

namespace cocos2d {

namespace network {



/** Singleton that handles asynchronous http requests.
 *
 * Once the request completed, a callback will issued in main thread when it provided during make request.
 *
 * @lua NA
 */
class CC_DLL HttpClient
{
public:
    /**
    * The buffer size of _responseMessage
    */
    static const int RESPONSE_BUFFER_SIZE = 256;

    /**
     * Get instance of HttpClient.
     *
     * @return the instance of HttpClient.
     */
    static HttpClient *getInstance();

    /**
     * Release the instance of HttpClient.
     */
    static void destroyInstance();

    /**
     * Enable cookie support.
     *
     * @param cookieFile the filepath of cookie file.
     */
    void enableCookies(const char* cookieFile);

    /**
     * Get the cookie filename
     *
     * @return the cookie filename
     */
    const std::string& getCookieFilename();

    /**
     * Set root certificate path for SSL verification.
     *
     * @param caFile a full path of root certificate.if it is empty, SSL verification is disabled.
     */
    void setSSLVerification(const std::string& caFile);

    /**
     * Get the ssl CA filename
     *
     * @return the ssl CA filename
     */
    const std::string& getSSLVerification();

    /**
     * Add a get request to task queue
     *
     * @param request a HttpRequest object, which includes url, response callback etc.
                      please make sure request->_requestData is clear before calling "send" here.
     */
    void send(std::shared_ptr<HttpRequest> request);

    /**
     * Immediate send a request
     *
     * @param request a HttpRequest object, which includes url, response callback etc.
                      please make sure request->_requestData is clear before calling "sendImmediate" here.
     */
    void sendImmediate(std::shared_ptr<HttpRequest> request);

    /**
     * Set the timeout value for connecting.
     *
     * @param value the timeout value for connecting.
     */
    void setTimeoutForConnect(int value);

    /**
     * Get the timeout value for connecting.
     *
     * @return int the timeout value for connecting.
     */
    int getTimeoutForConnect();

    /**
     * Set the timeout value for reading.
     *
     * @param value the timeout value for reading.
     */
    void setTimeoutForRead(int value);

    /**
     * Get the timeout value for reading.
     *
     * @return int the timeout value for reading.
     */
    int getTimeoutForRead();

    HttpCookie* getCookie() const {return _cookie; }

    std::mutex& getCookieFileMutex() {return _cookieFileMutex;}

    std::mutex& getSSLCaFileMutex() {return _sslCaFileMutex;}
private:
    HttpClient();
    virtual ~HttpClient();
    bool init();

    /**
     * Init pthread mutex, semaphore, and create new thread for http requests
     * @return bool
     */
    bool lazyInitThreadSemphore();
    void networkThread();
    void networkThreadAlone(std::shared_ptr<HttpRequest> request, std::shared_ptr<HttpResponse> response);
    /** Poll function called from main thread to dispatch callbacks when http requests finished **/
    void dispatchResponseCallbacks();

    void processResponse(std::shared_ptr<HttpResponse>, char* responseMessage);
    void increaseThreadCount();
    void decreaseThreadCountAndMayDeleteThis();

private:
    bool _isInited;

    int _timeoutForConnect;
    std::mutex _timeoutForConnectMutex;

    int _timeoutForRead;
    std::mutex _timeoutForReadMutex;

    int  _threadCount;
    std::mutex _threadCountMutex;

    Scheduler* _scheduler;
    std::mutex _schedulerMutex;

    std::deque<std::shared_ptr<HttpRequest>> _requestQueue;
    std::mutex _requestQueueMutex;

    std::deque<std::shared_ptr<HttpResponse>> _responseQueue;
    std::mutex _responseQueueMutex;

    std::string _cookieFilename;
    std::mutex _cookieFileMutex;

    std::string _sslCaFilename;
    std::mutex _sslCaFileMutex;

    HttpCookie* _cookie;

    std::condition_variable_any _sleepCondition;

    char _responseMessage[RESPONSE_BUFFER_SIZE];
};

} // namespace network

} // namespace cocos2d

// end group
/// @}

#endif //__CCHTTPCLIENT_H__


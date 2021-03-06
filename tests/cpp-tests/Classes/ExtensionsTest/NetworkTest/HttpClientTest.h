#ifndef __HTTP_CLIENT_H__
#define __HTTP_CLIENT_H__

#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
#include "BaseTest.h"

DEFINE_TEST_SUITE(HttpClientTests);

class HttpClientTest : public TestCase
{
public:
    static HttpClientTest* create()
    {
        auto ret = new HttpClientTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    HttpClientTest();
    virtual ~HttpClientTest();
    
    //Menu Callbacks
    void onMenuGetTestClicked(cocos2d::Ref *sender, bool isImmediate);
    void onMenuPostTestClicked(cocos2d::Ref *sender, bool isImmediate);
    void onMenuPostBinaryTestClicked(cocos2d::Ref *sender, bool isImmediate);
    void onMenuPutTestClicked(cocos2d::Ref *sender, bool isImmediate);
    void onMenuDeleteTestClicked(cocos2d::Ref *sender, bool isImmediate);
    
    //Http Response Callback
    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, std::shared_ptr<cocos2d::network::HttpResponse> response);

    virtual std::string title() const override { return "Http Request Test"; }

private:
    cocos2d::Label* _labelStatusCode;
};

#endif //__HTTPREQUESTHTTP_H

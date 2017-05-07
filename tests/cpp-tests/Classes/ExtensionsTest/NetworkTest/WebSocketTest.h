//
//  WebSocketTest.h
//  TestCpp
//
//  Created by James Chen on 5/31/13.
//
//

#ifndef __TestCpp__WebSocketTest__
#define __TestCpp__WebSocketTest__

#include "extensions/cocos-ext.h"
#include "network/WebSocket.h"
#include "BaseTest.h"

DEFINE_TEST_SUITE(WebSocketTests);

class WebSocketTest : public TestCase
, public cocos2d::network::WebSocket::Delegate
{
public:
    static WebSocketTest* create()
    {
        auto ret = new WebSocketTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    WebSocketTest();
    virtual ~WebSocketTest();
    
    virtual void onOpen(cocos2d::network::WebSocket* ws)override;
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)override;
    virtual void onClose(cocos2d::network::WebSocket* ws)override;
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)override;
    
    // Menu Callbacks
    void onMenuSendTextClicked(cocos2d::Ref *sender);
    void onMenuSendMultipleTextClicked(cocos2d::Ref *sender);
    void onMenuSendBinaryClicked(cocos2d::Ref *sender);

    virtual std::string title() const override { return "WebSocket Test"; }
    void startTestCallback(cocos2d::Ref* sender);

private:
    cocos2d::network::WebSocket* _wsiSendText;
    cocos2d::network::WebSocket* _wsiSendBinary;
    cocos2d::network::WebSocket* _wsiError;
    
    cocos2d::Label* _sendTextStatus;
    cocos2d::Label* _sendBinaryStatus;
    cocos2d::Label* _errorStatus;
    cocos2d::Menu* _startTestMenu;
    
    int _sendTextTimes;
    int _sendBinaryTimes;
};

class WebSocketCloseTest : public TestCase
    , public cocos2d::network::WebSocket::Delegate
{
public:
    static WebSocketCloseTest* create()
    {
        auto ret = new WebSocketCloseTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onOpen(cocos2d::network::WebSocket* ws)override;
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)override;
    virtual void onClose(cocos2d::network::WebSocket* ws)override;
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)override;

    WebSocketCloseTest();
    virtual ~WebSocketCloseTest();

    virtual std::string title() const override { return "WebSocket close by resetDirector event Test"; }
    std::string subtitle() const override { return "Click close button, Program should close without crash."; }

private:
    cocos2d::network::WebSocket* _wsiTest;
};

#endif /* defined(__TestCpp__WebSocketTest__) */

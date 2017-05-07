#ifndef __MOUSE_TEST_H_
#define __MOUSE_TEST_H_

#include "../BaseTest.h"

namespace cocos2d {
class EventListenerMouse;
}

DEFINE_TEST_SUITE(MouseTests);

class BaseMouseTest : public TestCase
{
public:
    
};

class MouseEventTest : public BaseMouseTest
{
public:
    static MouseEventTest* create()
    {
        auto ret = new MouseEventTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    MouseEventTest();
    ~MouseEventTest();

    void onMouseDown(cocos2d::Event* event);
    void onMouseUp(cocos2d::Event* event);
    void onMouseMove(cocos2d::Event* event);
    void onMouseScroll(cocos2d::Event* event);
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

private:
    cocos2d::Label*   _labelAction;
    cocos2d::Label*   _labelPosition;
    cocos2d::EventListenerMouse* _mouseListener;
};

class HideMouseTest : public BaseMouseTest
{
public:
    static HideMouseTest* create()
    {
        auto ret = new HideMouseTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    HideMouseTest();
    ~HideMouseTest();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
private:
    cocos2d::EventListenerMouse* _lis;
};

#endif

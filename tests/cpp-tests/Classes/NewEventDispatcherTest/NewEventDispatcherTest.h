//
//  NewEventDispatcherTest.h
//  samples
//
//  Created by James Chen on 9/13/13.
//
//

#ifndef __samples__NewEventDispatcherTest__
#define __samples__NewEventDispatcherTest__

#include "../BaseTest.h"

namespace cocos2d {
class Sprite;
}

DEFINE_TEST_SUITE(EventDispatcherTests);

class EventDispatcherTestDemo : public TestCase
{
public:
    virtual std::string title() const override;
};


class TouchableSpriteTest : public EventDispatcherTestDemo
{
public:
    static TouchableSpriteTest* create()
    {
        auto ret = new TouchableSpriteTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class FixedPriorityTest : public EventDispatcherTestDemo
{
public:
    static FixedPriorityTest* create()
    {
        auto ret = new FixedPriorityTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class RemoveListenerWhenDispatching : public EventDispatcherTestDemo
{
public:
    static RemoveListenerWhenDispatching* create()
    {
        auto ret = new RemoveListenerWhenDispatching;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class CustomEventTest : public EventDispatcherTestDemo
{
public:
    static CustomEventTest* create()
    {
        auto ret = new CustomEventTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
private:
    cocos2d::EventListenerCustom* _listener;
    cocos2d::EventListenerCustom* _listener2;
};

class LabelKeyboardEventTest : public EventDispatcherTestDemo
{
public:
    static LabelKeyboardEventTest* create()
    {
        auto ret = new LabelKeyboardEventTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteAccelerationEventTest : public EventDispatcherTestDemo
{
public:
    static SpriteAccelerationEventTest* create()
    {
        auto ret = new SpriteAccelerationEventTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class RemoveAndRetainNodeTest : public EventDispatcherTestDemo
{
public:
    static RemoveAndRetainNodeTest* create()
    {
        auto ret = new RemoveAndRetainNodeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
private:
    cocos2d::Sprite* _sprite;
    bool _spriteSaved;
};

class RemoveListenerAfterAddingTest : public EventDispatcherTestDemo
{
public:
    static RemoveListenerAfterAddingTest* create()
    {
        auto ret = new RemoveListenerAfterAddingTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class DirectorEventTest : public EventDispatcherTestDemo
{
public:
    static DirectorEventTest* create()
    {
        auto ret = new DirectorEventTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    DirectorEventTest();
    virtual void onEnter() override;
    virtual void onExit() override;

    virtual void update(float dt) override;

    void onEvent1(cocos2d::EventCustom* event);
    void onEvent2(cocos2d::EventCustom* event);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    int _count1, _count2, _count3, _count4;
    cocos2d::Label* _label1, *_label2, *_label3, *_label4;
    cocos2d::EventListenerCustom* _event1, *_event2, *_event3, *_event4;
};

class GlobalZTouchTest : public EventDispatcherTestDemo
{
public:
    static GlobalZTouchTest* create()
    {
        auto ret = new GlobalZTouchTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    GlobalZTouchTest();
    
    virtual void update(float dt) override;
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
protected:
    cocos2d::Sprite* _sprite;
    float _accum;
};

class StopPropagationTest : public EventDispatcherTestDemo
{
public:
    static StopPropagationTest* create()
    {
        auto ret = new StopPropagationTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    StopPropagationTest();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
protected:
    bool isPointInNode(cocos2d::Vec2 pt, cocos2d::Node* node);
    bool isPointInTopHalfAreaOfScreen(cocos2d::Vec2 pt);
};

class PauseResumeTargetTest : public EventDispatcherTestDemo
{
public:
    static PauseResumeTargetTest* create()
    {
        auto ret = new PauseResumeTargetTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    PauseResumeTargetTest();
    virtual ~PauseResumeTargetTest();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
private:
};

class Issue4129 : public EventDispatcherTestDemo
{
public:
    static Issue4129* create()
    {
        auto ret = new Issue4129;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Issue4129();
    virtual ~Issue4129();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
private:
    cocos2d::EventListenerCustom* _customlistener;
    bool _bugFixed;
};

class Issue4160 : public EventDispatcherTestDemo
{
public:
    static Issue4160* create()
    {
        auto ret = new Issue4160;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Issue4160();
    virtual ~Issue4160();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
private:
};

class DanglingNodePointersTest : public EventDispatcherTestDemo
{
public:
    static DanglingNodePointersTest* create()
    {
        auto ret = new DanglingNodePointersTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    DanglingNodePointersTest();
    virtual ~DanglingNodePointersTest();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class RegisterAndUnregisterWhileEventHanldingTest : public EventDispatcherTestDemo
{
public:
    static RegisterAndUnregisterWhileEventHanldingTest* create()
    {
        auto ret = new RegisterAndUnregisterWhileEventHanldingTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    RegisterAndUnregisterWhileEventHanldingTest();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class WindowEventsTest : public EventDispatcherTestDemo
{
public:
    static WindowEventsTest* create()
    {
        auto ret = new WindowEventsTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    WindowEventsTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Issue8194 : public EventDispatcherTestDemo
{
public:
    static Issue8194* create()
    {
        auto ret = new Issue8194;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Issue8194();
    virtual ~Issue8194();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
private:
    cocos2d::EventListenerCustom* _listener;
};

class Issue9898 : public EventDispatcherTestDemo
{
public:
    static Issue9898* create()
    {
        auto ret = new Issue9898;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Issue9898();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

private:
    cocos2d::EventListenerCustom* _listener;
};

#endif /* defined(__samples__NewEventDispatcherTest__) */

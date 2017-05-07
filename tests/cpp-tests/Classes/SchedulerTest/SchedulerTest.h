#ifndef _SCHEDULER_TEST_H_
#define _SCHEDULER_TEST_H_

#include "extensions/cocos-ext.h"
#include "../BaseTest.h"

DEFINE_TEST_SUITE(SchedulerTests);

class SchedulerTestLayer : public TestCase
{
public:
};

class SchedulerAutoremove : public SchedulerTestLayer
{
public:
    static SchedulerAutoremove* create()
    {
        auto ret = new SchedulerAutoremove;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void autoremove(float dt);
    void tick(float dt);
private:
    float accum;
};

class SchedulerPauseResume : public SchedulerTestLayer
{
public:
    static SchedulerPauseResume* create()
    {
        auto ret = new SchedulerPauseResume;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void tick1(float dt);
    void tick2(float dt);
    using SchedulerTestLayer::pause;
    void pause(float dt);
};

class SchedulerPauseResumeAll : public SchedulerTestLayer
{
public:
    static SchedulerPauseResumeAll* create()
    {
        auto ret = new SchedulerPauseResumeAll;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SchedulerPauseResumeAll();
    virtual ~SchedulerPauseResumeAll();
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void update(float delta) override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void tick1(float dt);
    void tick2(float dt);
    using SchedulerTestLayer::pause;
    void pause(float dt);
    using SchedulerTestLayer::resume;
    void resume(float dt);
};

class SchedulerPauseResumeAllUser : public SchedulerTestLayer
{
public:
    static SchedulerPauseResumeAllUser* create()
    {
        auto ret = new SchedulerPauseResumeAllUser;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SchedulerPauseResumeAllUser();
    virtual ~SchedulerPauseResumeAllUser();
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void tick1(float dt);
    void tick2(float dt);
    using SchedulerTestLayer::pause;
    void pause(float dt);
    using SchedulerTestLayer::resume;
    void resume(float dt);
};

class SchedulerUnscheduleAll : public SchedulerTestLayer
{
public:
    static SchedulerUnscheduleAll* create()
    {
        auto ret = new SchedulerUnscheduleAll;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void tick1(float dt);
    void tick2(float dt);
    void tick3(float dt);
    void tick4(float dt);
    void unscheduleAll(float dt);
};

class SchedulerUnscheduleAllUserLevel : public SchedulerTestLayer
{
public:
    static SchedulerUnscheduleAllUserLevel* create()
    {
        auto ret = new SchedulerUnscheduleAllUserLevel;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void tick1(float dt);
    void tick2(float dt);
    void tick3(float dt);
    void tick4(float dt);
    void unscheduleAll(float dt);
};

class SchedulerSchedulesAndRemove : public SchedulerTestLayer
{
public:
    static SchedulerSchedulesAndRemove* create()
    {
        auto ret = new SchedulerSchedulesAndRemove;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void tick1(float dt);
    void tick2(float dt);
    void tick3(float dt);
    void tick4(float dt);
    void scheduleAndUnschedule(float dt);
};

class SchedulerUpdate : public SchedulerTestLayer
{
public:
    static SchedulerUpdate* create()
    {
        auto ret = new SchedulerUpdate;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void removeUpdates(float dt);
};

class SchedulerUpdateAndCustom : public SchedulerTestLayer
{
public:
    static SchedulerUpdateAndCustom* create()
    {
        auto ret = new SchedulerUpdateAndCustom;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual float getDuration() const override;

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void update(float dt) override;
    void tick(float dt);
    void stopSelectors(float dt);
};

class SchedulerUpdateFromCustom : public SchedulerTestLayer
{
public:
    static SchedulerUpdateFromCustom* create()
    {
        auto ret = new SchedulerUpdateFromCustom;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void update(float dt) override;
    void schedUpdate(float dt);
    void stopUpdate(float dt);
};

class TestNode : public cocos2d::Node
{
public:
    static TestNode* create()
    {
        auto ret = new TestNode;
        ret->init();
        ret->autorelease();
        return ret;
    }

    ~TestNode();

    void initWithString(const std::string& str, int priority);
    virtual void update(float dt) override;
private:
    std::string _string;
};

class RescheduleSelector : public SchedulerTestLayer
{
public:
    static RescheduleSelector* create()
    {
        auto ret = new RescheduleSelector;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void schedUpdate(float dt);
private:
    float _interval;
    int   _ticks;
};

class SchedulerDelayAndRepeat : public SchedulerTestLayer
{
public:
    static SchedulerDelayAndRepeat* create()
    {
        auto ret = new SchedulerDelayAndRepeat;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void update(float dt) override;
};

class SchedulerSpeedup : public SchedulerTestLayer
{
public:
    static SchedulerSpeedup* create()
    {
        auto ret = new SchedulerSpeedup;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    cocos2d::extension::ControlSlider* sliderCtl();
    void sliderAction(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    cocos2d::extension::ControlSlider* _sliderCtl;
};

class SchedulerIssue2268 : public SchedulerTestLayer
{
public:
    static SchedulerIssue2268* create()
    {
        auto ret = new SchedulerIssue2268;
        ret->init();
        ret->autorelease();
        return ret;
    }

    ~SchedulerIssue2268();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void onEnter() override;
    void update(float dt) override;

private:
    cocos2d::Node* testNode;
};

class ScheduleCallbackTest : public SchedulerTestLayer
{
public:
    static ScheduleCallbackTest* create()
    {
        auto ret = new ScheduleCallbackTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    ~ScheduleCallbackTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void onEnter() override;
    
    void callback(float dt);
    
private:
};

class ScheduleUpdatePriority : public SchedulerTestLayer
{
public:
    static ScheduleUpdatePriority* create()
    {
        auto ret = new ScheduleUpdatePriority;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void onEnter() override;
    void onExit() override;
    
    virtual void update(float dt) override;
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

class SchedulerIssue10232 : public SchedulerTestLayer
{
public:
    static SchedulerIssue10232* create()
    {
        auto ret = new SchedulerIssue10232;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void onEnter() override;
    void update(float dt) override;
};

#endif

#ifndef _ACTION_MANAGER_TEST_H_
#define _ACTION_MANAGER_TEST_H_

#include "../BaseTest.h"

namespace cocos2d {
class TextureAtlas;
}

DEFINE_TEST_SUITE(ActionManagerTests);

class ActionManagerTest : public TestCase
{
protected:
    cocos2d::TextureAtlas* _atlas;

    std::string    _title;

public:
    ActionManagerTest();
    ~ActionManagerTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class CrashTest : public ActionManagerTest
{
public:
    static CrashTest* create()
    {
        auto ret = new CrashTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    void removeThis();
};

class LogicTest : public ActionManagerTest
{
public:
    static LogicTest* create()
    {
        auto ret = new LogicTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    void bugMe(Node* node);
};

class PauseTest : public ActionManagerTest
{
public:
    static PauseTest* create()
    {
        auto ret = new PauseTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    void unpause(float dt);
};

class StopActionTest : public ActionManagerTest
{
public:
    static StopActionTest* create()
    {
        auto ret = new StopActionTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    void stopAction();
};

class StopAllActionsTest : public ActionManagerTest
{
public:
    static StopAllActionsTest* create()
    {
        auto ret = new StopAllActionsTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    void stopAction(float time);
};

class ResumeTest : public ActionManagerTest
{
public:
    static ResumeTest* create()
    {
        auto ret = new ResumeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    void resumeGrossini(float time);
};

class StopActionsByFlagsTest : public ActionManagerTest
{
public:
    static StopActionsByFlagsTest* create()
    {
        auto ret = new StopActionsByFlagsTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    void stopAction(float time);
protected:
    const unsigned int kMoveFlag = 0x01;
    const unsigned int kScaleFlag = 0x02;
    const unsigned int kRotateFlag = 0x04;
    const unsigned int kRepeatForeverFlag = 0x08; // You don't need this for the test, but it's for demonstration how to activate several flags on an action.
};

class Issue14050Test : public ActionManagerTest
{
public:
    static Issue14050Test* create()
    {
        auto ret = new Issue14050Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string subtitle() const override;
    virtual void onEnter() override;
protected:
};

#endif

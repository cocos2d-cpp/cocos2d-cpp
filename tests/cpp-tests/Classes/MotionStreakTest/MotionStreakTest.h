#ifndef _MOTION_STREAK_TEST_H_
#define _MOTION_STREAK_TEST_H_

#include "../BaseTest.h"

namespace cocos2d {
class MotionStreak;
}

DEFINE_TEST_SUITE(MotionStreakTests);

class MotionStreakTest : public TestCase
{
public:
    MotionStreakTest(void);
    ~MotionStreakTest(void);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;

    void modeCallback(cocos2d::Ref* sender);
protected:
    cocos2d::MotionStreak* _streak;
};

class MotionStreakTest1 : public MotionStreakTest
{
protected:
    cocos2d::Node*        _root;
    cocos2d::Node*        _target;

public:
    static MotionStreakTest1* create()
    {
        auto ret = new MotionStreakTest1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    void onUpdate(float delta);
    virtual std::string title() const override;
};

class MotionStreakTest2 : public MotionStreakTest
{
protected:
    cocos2d::Node*        _root;
    cocos2d::Node*        _target;

public:
    static MotionStreakTest2* create()
    {
        auto ret = new MotionStreakTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    virtual std::string title() const override;
};

class Issue1358 : public MotionStreakTest
{
public:
    static Issue1358* create()
    {
        auto ret = new Issue1358;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    virtual void update(float dt) override;
private:
    cocos2d::Vec2 _center;
    float _radius;
    float _angle;
};

class Issue12226 : public MotionStreakTest
{
public:
    static Issue12226* create()
    {
        auto ret = new Issue12226;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};


#endif

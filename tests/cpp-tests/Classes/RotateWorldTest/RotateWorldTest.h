#ifndef _ROTATE_WORLD_TEST_H_
#define _ROTATE_WORLD_TEST_H_

#include "../BaseTest.h"

DEFINE_TEST_SUITE(RotateWorldTests);

class RotateWorldTest : public TestCase
{
public:
    static RotateWorldTest* create()
    {
        auto ret = new RotateWorldTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual bool init() override;
};

class SpriteLayer : public cocos2d::Layer
{
public:
    virtual void onEnter() override;
    static SpriteLayer* create()
    {
        auto ret = new SpriteLayer;
        ret->init();
        ret->autorelease();
        return ret;
    }
};

class TestLayer : public cocos2d::Layer
{
public:
    virtual void onEnter() override;

    static TestLayer* create()
    {
        auto ret = new TestLayer;
        ret->init();
        ret->autorelease();
        return ret;
    }
};

class RotateWorldMainLayer : public cocos2d::Layer
{
public:
    virtual void onEnter() override;

    static RotateWorldMainLayer* create()
    {
        auto ret = new RotateWorldMainLayer;
        ret->init();
        ret->autorelease();
        return ret;
    }
};

#endif

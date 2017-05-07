#ifndef _PARALLAX_TEST_H_
#define _PARALLAX_TEST_H_

#include "../BaseTest.h"

namespace cocos2d {
class MotionStreak;
class ParallaxNode;
class TextureAtlas;
class Sprite;
}

DEFINE_TEST_SUITE(ParallaxTests);

class ParallaxDemo : public TestCase
{
protected:
    cocos2d::TextureAtlas* _atlas;
};

class Parallax1 : public ParallaxDemo
{
protected:
    cocos2d::Node*        _root;
    cocos2d::Node*        _target;
    cocos2d::MotionStreak*        _streak;

public:
    static Parallax1* create()
    {
        auto ret = new Parallax1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Parallax1();
    virtual std::string title() const override;
};

class Parallax2 : public ParallaxDemo
{
protected:
    cocos2d::Node*        _root;
    cocos2d::Node*        _target;
    cocos2d::MotionStreak*        _streak;

public:
    static Parallax2* create()
    {
        auto ret = new Parallax2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Parallax2();
    
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

    virtual std::string title() const override;
};

class Issue2572 : public ParallaxDemo
{
protected:
    cocos2d::ParallaxNode* _paraNode;
    float _moveTimer;
    float _addTimer;
    std::vector<cocos2d::node_ptr<cocos2d::Sprite>> _childList;
    size_t _preListSize;
    int _printCount;
    
    float _addChildStep;
    float _wholeMoveTime;
    cocos2d::Vec2 _wholeMoveSize;

public:
    static Issue2572* create()
    {
        auto ret = new Issue2572;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Issue2572();
    
    virtual void update(float dt) override;
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

#endif

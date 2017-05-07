#ifndef _EFFECT_ADVANCED_TEST_H_
#define _EFFECT_ADVANCED_TEST_H_

#include "../BaseTest.h"

namespace cocos2d {
class TextureAtlas;
}

DEFINE_TEST_SUITE(EffectAdvanceTests);

class EffectAdvanceBaseTest: public TestCase
{
protected:
    cocos2d::TextureAtlas* _atlas;

    std::string    _title;

    cocos2d::Node* _bgNode;
    cocos2d::Node* _target1;
    cocos2d::Node* _target2;

public:
    virtual void onEnter() override;
    ~EffectAdvanceBaseTest(void);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Effect1 : public EffectAdvanceBaseTest
{
public:
    static Effect1* create()
    {
        auto ret = new Effect1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
};

class Effect2 : public EffectAdvanceBaseTest
{
public:
    static Effect2* create()
    {
        auto ret = new Effect2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
};

class Effect3 : public EffectAdvanceBaseTest
{
public:
    static Effect3* create()
    {
        auto ret = new Effect3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
};

class Effect4 : public EffectAdvanceBaseTest
{
public:
    static Effect4* create()
    {
        auto ret = new Effect4;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
};

class Effect5 : public EffectAdvanceBaseTest
{
public:
    static Effect5* create()
    {
        auto ret = new Effect5;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
};

class Issue631 : public EffectAdvanceBaseTest
{
public:
    static Issue631* create()
    {
        auto ret = new Issue631;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

#endif

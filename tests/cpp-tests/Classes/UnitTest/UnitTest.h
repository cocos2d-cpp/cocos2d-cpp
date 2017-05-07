#ifndef __UNIT_TEST__
#define __UNIT_TEST__

#include "../BaseTest.h"

#include <vector>

DEFINE_TEST_SUITE(UnitTests);

class UnitTestDemo : public TestCase
{
public:
    virtual std::string title() const override;
};

//-------------------------------------

class ValueTest : public UnitTestDemo
{
public:
    static ValueTest* create()
    {
        auto ret = new ValueTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
    void constFunc(const cocos2d::Value& value) const;
};

class UTFConversionTest : public UnitTestDemo
{
public:
    static UTFConversionTest* create()
    {
        auto ret = new UTFConversionTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class UIHelperSubStringTest : public UnitTestDemo
{
public:
    static UIHelperSubStringTest* create()
    {
        auto ret = new UIHelperSubStringTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class MathUtilTest : public UnitTestDemo
{
public:
    static MathUtilTest* create()
    {
        auto ret = new MathUtilTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

#endif /* __UNIT_TEST__ */

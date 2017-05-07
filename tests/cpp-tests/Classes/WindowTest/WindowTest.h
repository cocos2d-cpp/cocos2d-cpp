#ifndef __WINDOWTEST_H__
#define __WINDOWTEST_H__

#include "../BaseTest.h"

DEFINE_TEST_SUITE(WindowTests);

class WindowTest : public TestCase
{
public:
    virtual std::string title() const override;
};

class WindowTestWindowed1 : public WindowTest
{
public:
    static WindowTestWindowed1* create()
    {
        auto ret = new WindowTestWindowed1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class WindowTestWindowed2 : public WindowTest
{
public:
    static WindowTestWindowed2* create()
    {
        auto ret = new WindowTestWindowed2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class WindowTestFullscreen1 : public WindowTest
{
public:
    static WindowTestFullscreen1* create()
    {
        auto ret = new WindowTestFullscreen1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class WindowTestFullscreen2 : public WindowTest
{
public:
    static WindowTestFullscreen2* create()
    {
        auto ret = new WindowTestFullscreen2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

#endif /* __WINDOWTEST_H__ */

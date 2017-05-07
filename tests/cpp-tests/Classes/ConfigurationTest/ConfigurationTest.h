#ifndef __CONFIGURATIONTEST_H__
#define __CONFIGURATIONTEST_H__

#include "../BaseTest.h"

DEFINE_TEST_SUITE(ConfigurationTests);

class ConfigurationBase : public TestCase
{
protected:

public:
    virtual std::string title() const override;
};

class ConfigurationLoadConfig : public ConfigurationBase
{
public:
    static ConfigurationLoadConfig* create()
    {
        auto ret = new ConfigurationLoadConfig;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ConfigurationQuery : public ConfigurationBase
{
public:
    static ConfigurationQuery* create()
    {
        auto ret = new ConfigurationQuery;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ConfigurationInvalid : public ConfigurationBase
{
public:
    static ConfigurationInvalid* create()
    {
        auto ret = new ConfigurationInvalid;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ConfigurationDefault : public ConfigurationBase
{
public:
    static ConfigurationDefault* create()
    {
        auto ret = new ConfigurationDefault;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ConfigurationSet : public ConfigurationBase
{
public:
    static ConfigurationSet* create()
    {
        auto ret = new ConfigurationSet;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};


#endif // __CONFIGURATIONTEST_H__

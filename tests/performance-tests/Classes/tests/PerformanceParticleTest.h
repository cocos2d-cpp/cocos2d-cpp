#ifndef __PERFORMANCE_PARTICLE_TEST_H__
#define __PERFORMANCE_PARTICLE_TEST_H__

#include "BaseTest.h"

DEFINE_TEST_SUITE(PerformceParticleTests);

class ParticleMainScene : public TestCase
{
public:
    ParticleMainScene();
    virtual bool init() override;
    virtual void initWithSubTest(int subtest, int particles);

    void step(float dt);
    void createParticleSystem();
    void testNCallback(cocos2d::Ref* sender);
    void updateQuantityLabel();
    void updateTitle();
    virtual void doTest();

    // overrides
    virtual void onExitTransitionDidStart() override;
    virtual void onEnterTransitionDidFinish() override;
    void beginStat(float dt);
    void endStat(float dt);
    void doAutoTest();

protected:
    int        lastRenderedCount;
    static int quantityParticles;
    static int subtestNumber;
    int        particleSize;

    bool       isStating;
    int        autoTestIndex;
    int        statCount;
    float      totalStatTime;
    float      minFrameRate;
    float      maxFrameRate;
};

class ParticlePerformTest1 : public ParticleMainScene
{
public:
    static ParticlePerformTest1* create()
    {
        auto ret = new ParticlePerformTest1;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual void initWithSubTest(int subtest, int particles) override;
};

class ParticlePerformTest2 : public ParticleMainScene
{
public:
    static ParticlePerformTest2* create()
    {
        auto ret = new ParticlePerformTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual void initWithSubTest(int subtest, int particles) override;
};

class ParticlePerformTest3 : public ParticleMainScene
{
public:
    static ParticlePerformTest3* create()
    {
        auto ret = new ParticlePerformTest3;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual void initWithSubTest(int subtest, int particles) override;
};

class ParticlePerformTest4 : public ParticleMainScene
{
public:
    static ParticlePerformTest4* create()
    {
        auto ret = new ParticlePerformTest4;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual void initWithSubTest(int subtest, int particles) override;
};

#endif

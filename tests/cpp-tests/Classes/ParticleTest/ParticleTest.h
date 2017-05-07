#ifndef _PARTICLE_TEST_H_
#define _PARTICLE_TEST_H_

#include "../BaseTest.h"

namespace cocos2d {
class ParticleBatchNode;
class ParticleSystemQuad;
class Sprite;
}

DEFINE_TEST_SUITE(ParticleTests);

class ParticleDemo : public TestCase
{
protected:
    cocos2d::ParticleSystemQuad* _emitter;
    cocos2d::Sprite* _background;
    cocos2d::LayerColor* _color;

public:
    ~ParticleDemo();

    virtual void onEnter()override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void toggleCallback(cocos2d::Ref* sender);

    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

    virtual void update(float dt) override;
    void setEmitterPosition();
};

class DemoFirework : public ParticleDemo
{
public:
    static DemoFirework* create()
    {
        auto ret = new DemoFirework;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoFire : public ParticleDemo
{
public:
    static DemoFire* create()
    {
        auto ret = new DemoFire;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoSun : public ParticleDemo
{
public:
    static DemoSun* create()
    {
        auto ret = new DemoSun;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoGalaxy : public ParticleDemo
{
public:
    static DemoGalaxy* create()
    {
        auto ret = new DemoGalaxy;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoFlower : public ParticleDemo
{
public:
    static DemoFlower* create()
    {
        auto ret = new DemoFlower;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoBigFlower : public ParticleDemo
{
public:
    static DemoBigFlower* create()
    {
        auto ret = new DemoBigFlower;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoRotFlower : public ParticleDemo
{
public:
    static DemoRotFlower* create()
    {
        auto ret = new DemoRotFlower;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoMeteor : public ParticleDemo
{
public:
    static DemoMeteor* create()
    {
        auto ret = new DemoMeteor;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoSpiral : public ParticleDemo
{
public:
    static DemoSpiral* create()
    {
        auto ret = new DemoSpiral;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoExplosion : public ParticleDemo
{
public:
    static DemoExplosion* create()
    {
        auto ret = new DemoExplosion;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoSmoke : public ParticleDemo
{
public:
    static DemoSmoke* create()
    {
        auto ret = new DemoSmoke;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoSnow : public ParticleDemo
{
public:
    static DemoSnow* create()
    {
        auto ret = new DemoSnow;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoRain : public ParticleDemo
{
public:
    static DemoRain* create()
    {
        auto ret = new DemoRain;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoModernArt : public ParticleDemo
{
public:
    static DemoModernArt* create()
    {
        auto ret = new DemoModernArt;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoRing : public ParticleDemo
{
public:
    static DemoRing* create()
    {
        auto ret = new DemoRing;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ParallaxParticle : public ParticleDemo
{
public:
    static ParallaxParticle* create()
    {
        auto ret = new ParallaxParticle;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoParticleFromFile : public ParticleDemo
{
public:
    static DemoParticleFromFile* create(const std::string& file)
    {
        auto ret = new (std::nothrow) DemoParticleFromFile;
        if (ret && ret->init())
        {
            ret->_title = file;
            ret->autorelease();
        }
        else
        {
            delete ret;
            ret = nullptr;
        }

        return ret;
    }

    std::string _title;
    
    virtual void onEnter() override;
    virtual std::string subtitle() const override
    {
        return _title;
    }
};

class RadiusMode1 : public ParticleDemo
{
public:
    static RadiusMode1* create()
    {
        auto ret = new RadiusMode1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class RadiusMode2 : public ParticleDemo
{
public:
    static RadiusMode2* create()
    {
        auto ret = new RadiusMode2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class Issue704 : public ParticleDemo
{
public:
    static Issue704* create()
    {
        auto ret = new Issue704;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Issue870 : public ParticleDemo
{
public:
    static Issue870* create()
    {
        auto ret = new Issue870;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void updateQuads(float dt);

private:
    int _index;
};

class Issue1201 : public ParticleDemo
{
public:
    static Issue1201* create()
    {
        auto ret = new Issue1201;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class ParticleBatchHybrid : public ParticleDemo
{
public:
    static ParticleBatchHybrid* create()
    {
        auto ret = new ParticleBatchHybrid;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    void switchRender(float dt);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
private:
    cocos2d::Node* _parent1;
    cocos2d::Node* _parent2;
};

class ParticleBatchMultipleEmitters : public ParticleDemo
{
public:
    static ParticleBatchMultipleEmitters* create()
    {
        auto ret = new ParticleBatchMultipleEmitters;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class ParticleReorder : public ParticleDemo
{
public:
    static ParticleReorder* create()
    {
        auto ret = new ParticleReorder;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    void reorderParticles(float dt);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
private:
    unsigned int _order;
};

class MultipleParticleSystems : public ParticleDemo
{
public:
    static MultipleParticleSystems* create()
    {
        auto ret = new MultipleParticleSystems;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void update(float dt) override;
};

class MultipleParticleSystemsBatched : public ParticleDemo
{
public:
    static MultipleParticleSystemsBatched* create()
    {
        auto ret = new MultipleParticleSystemsBatched;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual void update(float dt) override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class AddAndDeleteParticleSystems : public ParticleDemo
{
public:
    static AddAndDeleteParticleSystems* create()
    {
        auto ret = new AddAndDeleteParticleSystems;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual void update(float dt) override;
    void removeSystem(float dt);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
private:
    cocos2d::ParticleBatchNode* _batchNode;
};

class ReorderParticleSystems : public ParticleDemo
{
public:
    static ReorderParticleSystems* create()
    {
        auto ret = new ReorderParticleSystems;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    void reorderSystem(float time);
    virtual void update(float dt) override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
private:
    cocos2d::ParticleBatchNode* _batchNode;
};

class PremultipliedAlphaTest : public ParticleDemo
{
    bool _hasEmitter;
public:
    static PremultipliedAlphaTest* create()
    {
        auto ret = new PremultipliedAlphaTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    void readdPaticle(float delta);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class PremultipliedAlphaTest2 : public ParticleDemo
{
public:
    static PremultipliedAlphaTest2* create()
    {
        auto ret = new PremultipliedAlphaTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Issue3990 : public ParticleDemo
{
public:
    static Issue3990* create()
    {
        auto ret = new Issue3990;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class ParticleVisibleTest : public ParticleDemo
{
public:
    static ParticleVisibleTest* create()
    {
        auto ret = new ParticleVisibleTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void callback(float delta);
};

class ParticleAutoBatching : public ParticleDemo
{
public:
    static ParticleAutoBatching* create()
    {
        auto ret = new ParticleAutoBatching;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class ParticleResetTotalParticles : public ParticleDemo
{
public:
    static ParticleResetTotalParticles* create()
    {
        auto ret = new ParticleResetTotalParticles;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class ParticleIssue12310 : public ParticleDemo
{
public:
    static ParticleIssue12310* create()
    {
        auto ret = new ParticleIssue12310;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class DemoPause : public ParticleDemo
{
public:
    static DemoPause* create()
    {
        auto ret = new DemoPause;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
    void pauseEmitter(float time);
};

#endif

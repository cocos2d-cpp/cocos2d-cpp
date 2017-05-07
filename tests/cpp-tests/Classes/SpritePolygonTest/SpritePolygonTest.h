#ifndef __cocos2d_tests__SpritePolygonTest__

#include "../BaseTest.h"

#include "2d/CCSprite.h"
#include "ui/UISlider.h"

namespace cocos2d {
class DrawNode;
}

DEFINE_TEST_SUITE(SpritePolygonTest);

class SpritePolygonTestCase : public TestCase
{
protected:
    std::string _title;
    std::string _subtitle;
    bool _isDebugDraw;
    bool _isNeedDebugMenu;
    std::vector<cocos2d::node_ptr<cocos2d::DrawNode>> _drawNodes;
    virtual std::string title() const override {return _title;};
    virtual std::string subtitle() const override {return _subtitle;};
    virtual bool init() override;
    virtual void onEnter()override;
    virtual void onExit()override;
    SpritePolygonTestCase();
    ~SpritePolygonTestCase();
    void updateDrawNode();
};

class SpritePolygonTestDemo : public SpritePolygonTestCase
{
protected:
    cocos2d::Sprite* _polygonSprite;
    cocos2d::Sprite* _normalSprite;
    virtual bool init() override;
    virtual void initSprites(){};
    void initTouches();
};

class SpritePolygonTest1 : public SpritePolygonTestDemo
{
public:
    static SpritePolygonTest1* create()
    {
        auto ret = new SpritePolygonTest1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpritePolygonTest1();
    void initSprites() override;
};

class SpritePolygonTest2 : public SpritePolygonTestDemo
{
public:
    static SpritePolygonTest2* create()
    {
        auto ret = new SpritePolygonTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpritePolygonTest2();
    virtual void initSprites() override;
};

class SpritePolygonTestSlider : public SpritePolygonTestCase
{
protected:
    cocos2d::Label *_epsilonLabel;
    int _tagIndex;
    cocos2d::Sprite* makeSprite(const std::string& filename, const cocos2d::Vec2& pos);
    virtual bool init() override;
    void initSliders();
    virtual void initSprites(){};
    void makeSprites(const std::string* list, const int count, const float y);
    void changeEpsilon(Ref *pSender, cocos2d::ui::Slider::EventType type);
    void updateLabel(const cocos2d::Sprite* sp, const cocos2d::PolygonInfo &pinfo);
};

class SpritePolygonTest3 : public SpritePolygonTestSlider
{
public:
    static SpritePolygonTest3* create()
    {
        auto ret = new SpritePolygonTest3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpritePolygonTest3();
    void initSprites() override;
};

class SpritePolygonTest4 : public SpritePolygonTestSlider
{
public:
    static SpritePolygonTest4* create()
    {
        auto ret = new SpritePolygonTest4;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpritePolygonTest4();
    void initSprites() override;
};

class SpritePolygonTest5 : public SpritePolygonTestCase
{
public:
    static SpritePolygonTest5* create()
    {
        auto ret = new SpritePolygonTest5;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpritePolygonTest5();
    void update(float dt) override;
protected:
    virtual bool init() override;
    void initTouch();
    void loadDefaultSprites();
    void addSpritePolygon(const cocos2d::Vec2& pos);
private:
    cocos2d::PolygonInfo _polygonInfo;
    int _tagIndex;
};

class SpritePolygonPerformance : public SpritePolygonTestCase
{
public:
    static SpritePolygonPerformance* create()
    {
        auto ret = new SpritePolygonPerformance;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpritePolygonPerformance();
    virtual void update(float dt) override;
protected:
    int _spriteCount;
    int _vertCount;
    int _triCount;
    int _pixelCount;
    float _elapsedTime;
    cocos2d::Label * _perfLabel;
    int _continuousLowDt;
    float _continuousHighDtTime;
    float _waitingTime;

    int _posX;
    int _posY;
    int _leftX;
    int _rightX;
    bool goRight;
    bool ended;
    
    float prevDt;
    
    unsigned int _incVert;
    unsigned int _incTri;
    unsigned int _incPix;

    void updateLabel();
    virtual bool init() override;
    virtual Node* makeSprite();
    void incrementStats();
    virtual void initIncrementStats(){};
};

class SpritePolygonPerformanceTestDynamic : public SpritePolygonPerformance
{
public:
    static SpritePolygonPerformanceTestDynamic* create()
    {
        auto ret = new SpritePolygonPerformanceTestDynamic;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpritePolygonPerformanceTestDynamic();
protected:
    cocos2d::Sprite* makeSprite() override;
    cocos2d::PolygonInfo _pinfo;
    virtual void initIncrementStats() override;
};

class SpritePerformanceTestDynamic : public SpritePolygonPerformance
{
public:
    static SpritePerformanceTestDynamic* create()
    {
        auto ret = new SpritePerformanceTestDynamic;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpritePerformanceTestDynamic();
protected:
    virtual cocos2d::Sprite* makeSprite() override;
    virtual void initIncrementStats() override;
};

class SpritePolygonTestNoCrash : public SpritePolygonTestDemo
{
public:
    static SpritePolygonTestNoCrash* create()
    {
        auto ret = new SpritePolygonTestNoCrash;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpritePolygonTestNoCrash();
    virtual void initSprites() override;
};

class SpritePolygonTestTPIsland : public SpritePolygonTestDemo
{
public:
    static SpritePolygonTestTPIsland* create()
    {
        auto ret = new SpritePolygonTestTPIsland;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpritePolygonTestTPIsland();
    virtual void initSprites() override;
};

class SpritePolygonTestAutoPolyIsland : public SpritePolygonTestDemo
{
public:
    static SpritePolygonTestAutoPolyIsland* create()
    {
        auto ret = new SpritePolygonTestAutoPolyIsland;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpritePolygonTestAutoPolyIsland();
    virtual void initSprites() override;
};

class SpritePolygonTestFrameAnim : public SpritePolygonTestDemo
{
public:
    static SpritePolygonTestFrameAnim* create()
    {
        auto ret = new SpritePolygonTestFrameAnim;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpritePolygonTestFrameAnim();
    virtual void initSprites() override;
};

class Issue14017Test : public SpritePolygonTestDemo
{
public:
    static Issue14017Test* create()
    {
        auto ret = new Issue14017Test;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Issue14017Test();
    virtual void initSprites() override;
};

#endif /* defined(__cocos2d_tests__SpritePolygonTest__) */

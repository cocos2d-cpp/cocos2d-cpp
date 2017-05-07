#ifndef _LAYER_TEST_H_
#define _LAYER_TEST_H_

#include "../BaseTest.h"

DEFINE_TEST_SUITE(LayerTests);

class LayerTest : public TestCase
{
public:
    virtual std::string title() const override;
protected:
    std::string    _title;
};

class LayerTestCascadingOpacityA : public LayerTest
{
public:
    static LayerTestCascadingOpacityA* create()
    {
        auto ret = new LayerTestCascadingOpacityA;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class LayerTestCascadingOpacityB : public LayerTest
{
public:
    static LayerTestCascadingOpacityB* create()
    {
        auto ret = new LayerTestCascadingOpacityB;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class LayerTestCascadingOpacityC : public LayerTest
{
public:
    static LayerTestCascadingOpacityC* create()
    {
        auto ret = new LayerTestCascadingOpacityC;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class LayerTestCascadingColorA : public LayerTest
{
public:
    static LayerTestCascadingColorA* create()
    {
        auto ret = new LayerTestCascadingColorA;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class LayerTestCascadingColorB : public LayerTest
{
public:
    static LayerTestCascadingColorB* create()
    {
        auto ret = new LayerTestCascadingColorB;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class LayerTestCascadingColorC : public LayerTest
{
public:
    static LayerTestCascadingColorC* create()
    {
        auto ret = new LayerTestCascadingColorC;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};


class LayerTest1 : public LayerTest
{
public:
    static LayerTest1* create()
    {
        auto ret = new LayerTest1;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;

    void updateSize(cocos2d::Vec2 &touchLocation);

    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
};

class LayerTest2 : public LayerTest
{
public:
    static LayerTest2* create()
    {
        auto ret = new LayerTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};


class LayerTestBlend : public LayerTest
{
public:
    static LayerTestBlend* create()
    {
        auto ret = new LayerTestBlend;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LayerTestBlend();
    void newBlend(float dt);
    virtual std::string subtitle() const override;
};

class LayerGradientTest : public LayerTest
{
public:
    static LayerGradientTest* create()
    {
        auto ret = new LayerGradientTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    LayerGradientTest();
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void toggleItem(cocos2d::Ref *sender);
};

class LayerGradientTest2 : public LayerTest
{
public:
    static LayerGradientTest2* create()
    {
        auto ret = new LayerGradientTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    LayerGradientTest2();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LayerIgnoreAnchorPointPos : public LayerTest
{
public:
    static LayerIgnoreAnchorPointPos* create()
    {
        auto ret = new LayerIgnoreAnchorPointPos;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    void onToggle(cocos2d::Ref* pObject);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LayerIgnoreAnchorPointRot : public LayerTest
{
public:
    static LayerIgnoreAnchorPointRot* create()
    {
        auto ret = new LayerIgnoreAnchorPointRot;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    void onToggle(cocos2d::Ref* pObject);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LayerIgnoreAnchorPointScale : public LayerTest
{
public:
    static LayerIgnoreAnchorPointScale* create()
    {
        auto ret = new LayerIgnoreAnchorPointScale;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    void onToggle(cocos2d::Ref* pObject);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LayerExtendedBlendOpacityTest : public LayerTest
{
public:
    static LayerExtendedBlendOpacityTest* create()
    {
        auto ret = new LayerExtendedBlendOpacityTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    LayerExtendedBlendOpacityTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LayerBug3162A : public LayerTest
{
public:
    static LayerBug3162A* create()
    {
        auto ret = new LayerBug3162A;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void step(float dt);
    
private:
    cocos2d::LayerColor* _layer[3];
};

class LayerBug3162B : public LayerTest
{
public:
    static LayerBug3162B* create()
    {
        auto ret = new LayerBug3162B;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void step(float dt);
    
private:
    cocos2d::LayerColor* _layer[3];
};

class LayerColorOccludeBug : public LayerTest
{
public:
    static LayerColorOccludeBug* create()
    {
        auto ret = new LayerColorOccludeBug;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
private:
    cocos2d::LayerColor* _layer;
};


#endif

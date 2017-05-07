#ifndef _SHADER_TEST_H_
#define _SHADER_TEST_H_

#include "../BaseTest.h"

#include "GUI/CCControlExtension/CCControl.h"

namespace cocos2d {
namespace extension {
class ControlSlider;
}
namespace ui {
class Slider;
}
}

DEFINE_TEST_SUITE(ShaderTests);

class ShaderTestDemo : public TestCase
{
public:
};

class ShaderMonjori : public ShaderTestDemo
{
public:
    static ShaderMonjori* create()
    {
        auto ret = new ShaderMonjori;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShaderMonjori();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init() override;
};

class ShaderMandelbrot : public ShaderTestDemo
{
public:
    static ShaderMandelbrot* create()
    {
        auto ret = new ShaderMandelbrot;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShaderMandelbrot();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init() override;
};

class ShaderJulia : public ShaderTestDemo
{
public:
    static ShaderJulia* create()
    {
        auto ret = new ShaderJulia;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShaderJulia();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init() override;
};

class ShaderHeart : public ShaderTestDemo
{
public:
    static ShaderHeart* create()
    {
        auto ret = new ShaderHeart;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShaderHeart();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init() override;
};

class ShaderFlower : public ShaderTestDemo
{
public:
    static ShaderFlower* create()
    {
        auto ret = new ShaderFlower;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShaderFlower();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init() override;
};

class ShaderPlasma : public ShaderTestDemo
{
public:
    static ShaderPlasma* create()
    {
        auto ret = new ShaderPlasma;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShaderPlasma();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init() override;
};

class SpriteBlur;
class ShaderBlur : public ShaderTestDemo
{
public:
    static ShaderBlur* create()
    {
        auto ret = new ShaderBlur;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShaderBlur();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init() override;
    void createSliderCtls();
    void onRadiusChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    void onSampleNumChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);
    
protected:
    SpriteBlur* _blurSprite;
    cocos2d::extension::ControlSlider* _sliderRadiusCtl;
    cocos2d::extension::ControlSlider* _sliderNumCtrl;
};

class ShaderRetroEffect : public ShaderTestDemo
{
public:
    static ShaderRetroEffect* create()
    {
        auto ret = new ShaderRetroEffect;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShaderRetroEffect();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    bool init() override;
    void update(float dt) override;
protected:
    cocos2d::Label* _label;
    float           _accum;
};

class ShaderNode : public cocos2d::Node
{
public:
    static ShaderNode* create()
    {
        auto ret = new ShaderNode;
        ret->init();
        ret->autorelease();
        return ret;
    }
    static ShaderNode* shaderNodeWithVertex(const std::string &vert, const std::string &frag);

    virtual void update(float dt) override;
    virtual void setPosition(const cocos2d::Vec2 &newPosition) override;
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

protected:
    ShaderNode();
    ~ShaderNode();

    bool initWithVertex(const std::string &vert, const std::string &frag);
    void loadShaderVertex(const std::string &vert, const std::string &frag);

    void onDraw(const cocos2d::Mat4& transform, uint32_t flags);

    cocos2d::Vec2 _center;
    cocos2d::Vec2 _resolution;
    float      _time;
    std::string _vertFileName;
    std::string _fragFileName;
    cocos2d::CustomCommand _customCommand;
};

class ShaderLensFlare : public ShaderTestDemo
{
public:
    static ShaderLensFlare* create()
    {
        auto ret = new ShaderLensFlare;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShaderLensFlare();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init() override;
};

class ShaderGlow : public ShaderTestDemo
{
public:
    static ShaderGlow* create()
    {
        auto ret = new ShaderGlow;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShaderGlow();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init() override;
};

class ShaderMultiTexture : public ShaderTestDemo
{
    static const int rightSpriteTag = 2014;
public:
    static ShaderMultiTexture* create()
    {
        auto ret = new ShaderMultiTexture;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShaderMultiTexture();
    cocos2d::ui::Slider* createSliderCtl();
    void changeTexture(cocos2d::Ref*);
    int _changedTextureId;
    cocos2d::Sprite* _sprite;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual bool init() override;
};

#endif

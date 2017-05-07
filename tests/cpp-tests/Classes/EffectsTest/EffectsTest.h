#ifndef _EFFECTS_TEST_H_
#define _EFFECTS_TEST_H_

#include "../BaseTest.h"

#include "2d/CCNodeGrid.h"

DEFINE_TEST_SUITE(EffectTests);

class EffectBaseTest : public TestCase
{
public:
    EffectBaseTest();
    virtual ~EffectBaseTest();
protected:
    virtual bool init() override;
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float /*t*/) {return nullptr;}
    void checkAnim(float dt);
    virtual bool isRectEffect(){return false;}
    virtual std::string title() const override {return _title;};
    virtual std::string subtitle() const override {return _subtitle;};
    std::string _title;
    std::string _subtitle;
    cocos2d::NodeGrid* _gridNodeTarget;
};

class Shaky3DDemo : public EffectBaseTest
{
public:
    static Shaky3DDemo* create()
    {
        auto ret = new Shaky3DDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Shaky3DDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class Waves3DDemo : public EffectBaseTest
{
public:
    static Waves3DDemo* create()
    {
        auto ret = new Waves3DDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Waves3DDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class FlipX3DDemo : public EffectBaseTest
{
public:
    static FlipX3DDemo* create()
    {
        auto ret = new FlipX3DDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    FlipX3DDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class FlipY3DDemo : public EffectBaseTest
{
public:
    static FlipY3DDemo* create()
    {
        auto ret = new FlipY3DDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    FlipY3DDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class Lens3DDemo : public EffectBaseTest
{
public:
    static Lens3DDemo* create()
    {
        auto ret = new Lens3DDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Lens3DDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class Ripple3DDemo : public EffectBaseTest
{
public:
    static Ripple3DDemo* create()
    {
        auto ret = new Ripple3DDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Ripple3DDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class LiquidDemo : public EffectBaseTest
{
public:
    static LiquidDemo* create()
    {
        auto ret = new LiquidDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    LiquidDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class WavesDemo : public EffectBaseTest
{
public:
    static WavesDemo* create()
    {
        auto ret = new WavesDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    WavesDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class TwirlDemo : public EffectBaseTest
{
public:
    static TwirlDemo* create()
    {
        auto ret = new TwirlDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TwirlDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class ShakyTiles3DDemo : public EffectBaseTest
{
public:
    static ShakyTiles3DDemo* create()
    {
        auto ret = new ShakyTiles3DDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShakyTiles3DDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class ShatteredTiles3DDemo : public EffectBaseTest
{
public:
    static ShatteredTiles3DDemo* create()
    {
        auto ret = new ShatteredTiles3DDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShatteredTiles3DDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class ShuffleTilesDemo : public EffectBaseTest
{
public:
    static ShuffleTilesDemo* create()
    {
        auto ret = new ShuffleTilesDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ShuffleTilesDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class FadeOutTRTilesDemo : public EffectBaseTest
{
public:
    static FadeOutTRTilesDemo* create()
    {
        auto ret = new FadeOutTRTilesDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    FadeOutTRTilesDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class FadeOutBLTilesDemo : public EffectBaseTest
{
public:
    static FadeOutBLTilesDemo* create()
    {
        auto ret = new FadeOutBLTilesDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    FadeOutBLTilesDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class FadeOutUpTilesDemo : public EffectBaseTest
{
public:
    static FadeOutUpTilesDemo* create()
    {
        auto ret = new FadeOutUpTilesDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    FadeOutUpTilesDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class FadeOutDownTilesDemo : public EffectBaseTest
{
public:
    static FadeOutDownTilesDemo* create()
    {
        auto ret = new FadeOutDownTilesDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    FadeOutDownTilesDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class TurnOffTilesDemo : public EffectBaseTest
{
public:
    static TurnOffTilesDemo* create()
    {
        auto ret = new TurnOffTilesDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TurnOffTilesDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class WavesTiles3DDemo : public EffectBaseTest
{
public:
    static WavesTiles3DDemo* create()
    {
        auto ret = new WavesTiles3DDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    WavesTiles3DDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class JumpTiles3DDemo : public EffectBaseTest
{
public:
    static JumpTiles3DDemo* create()
    {
        auto ret = new JumpTiles3DDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    JumpTiles3DDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class SplitRowsDemo : public EffectBaseTest
{
public:
    static SplitRowsDemo* create()
    {
        auto ret = new SplitRowsDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SplitRowsDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class SplitColsDemo : public EffectBaseTest
{
public:
    static SplitColsDemo* create()
    {
        auto ret = new SplitColsDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SplitColsDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class PageTurn3DDemo : public EffectBaseTest
{
public:
    static PageTurn3DDemo* create()
    {
        auto ret = new PageTurn3DDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    PageTurn3DDemo();
protected:
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

class PageTurn3DRectDemo : public EffectBaseTest
{
public:
    static PageTurn3DRectDemo* create()
    {
        auto ret = new PageTurn3DRectDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }
    PageTurn3DRectDemo();
protected:
    virtual bool isRectEffect() override {return true;}
    virtual std::unique_ptr<cocos2d::ActionInterval> createEffect(float t) override;
};

#endif

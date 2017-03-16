#include "EffectsTest.h"
#include "../testResource.h"

#include "2d/CCActionTiledGrid.h"
#include "2d/CCActionPageTurn3D.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"

using namespace cocos2d;

EffectTests::EffectTests()
{
    ADD_TEST_CASE(Shaky3DDemo);
    ADD_TEST_CASE(Waves3DDemo);
    ADD_TEST_CASE(FlipX3DDemo);
    ADD_TEST_CASE(FlipY3DDemo);
    ADD_TEST_CASE(Lens3DDemo);
    ADD_TEST_CASE(Ripple3DDemo);
    ADD_TEST_CASE(LiquidDemo);
    ADD_TEST_CASE(WavesDemo);
    ADD_TEST_CASE(TwirlDemo);
    ADD_TEST_CASE(ShakyTiles3DDemo);
    ADD_TEST_CASE(ShatteredTiles3DDemo);
    ADD_TEST_CASE(ShuffleTilesDemo);
    ADD_TEST_CASE(FadeOutTRTilesDemo);
    ADD_TEST_CASE(FadeOutBLTilesDemo);
    ADD_TEST_CASE(FadeOutUpTilesDemo);
    ADD_TEST_CASE(FadeOutDownTilesDemo);
    ADD_TEST_CASE(TurnOffTilesDemo);
    ADD_TEST_CASE(WavesTiles3DDemo);
    ADD_TEST_CASE(JumpTiles3DDemo);
    ADD_TEST_CASE(SplitRowsDemo);
    ADD_TEST_CASE(SplitColsDemo);
    ADD_TEST_CASE(PageTurn3DDemo);
    ADD_TEST_CASE(PageTurn3DRectDemo);
}

Shaky3DDemo::Shaky3DDemo()
{
    _title = "Shaky3D";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> Shaky3DDemo::createEffect(float t)
{
    return std::make_unique<Shaky3D>(t, Size(15,10), 5, false);
}

Waves3DDemo::Waves3DDemo()
{
    _title = "Waves3D";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> Waves3DDemo::createEffect(float t)
{
    return std::make_unique<Waves3D>(t, Size(15,10), 5, 40);
}

std::unique_ptr<cocos2d::ActionInterval> FlipX3DDemo::createEffect(float t)
{
    auto flipx  = std::make_unique<FlipX3D>(t);
    auto flipx_back = std::unique_ptr<GridAction>(flipx->reverse());
    auto delay = std::make_unique<DelayTime>(2);
    return std::make_unique<Sequence>( std::move(flipx), std::move(delay), std::move(flipx_back) );
}

FlipX3DDemo::FlipX3DDemo()
{
    _title = "FlipX3D";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> FlipY3DDemo::createEffect(float t)
{
    auto flipy  = std::make_unique<FlipY3D>(t);
    auto flipy_back = std::unique_ptr<GridAction>(flipy->reverse());
    auto delay = std::make_unique<DelayTime>(2);
    
    return std::make_unique<Sequence>( std::move(flipy), std::move( delay), std::move( flipy_back) );
}

FlipY3DDemo::FlipY3DDemo()
{
    _title = "FlipY3D";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> Lens3DDemo::createEffect(float t)
{
    auto size = Director::getInstance()->getWinSize();
    return std::make_unique<Lens3D>(t, Size(15,10), Vec2(size.width/2,size.height/2), 240);
}

Lens3DDemo::Lens3DDemo()
{
    _title = "Lens3D";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> Ripple3DDemo::createEffect(float t)
{
    auto size = Director::getInstance()->getWinSize();
    return std::make_unique<Ripple3D>(t, Size(32,24), Vec2(size.width/2,size.height/2), 240, 4, 160);
}

Ripple3DDemo::Ripple3DDemo()
{
    _title = "Ripple3D";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> LiquidDemo::createEffect(float t)
{
    return std::make_unique<Liquid>(t, Size(16,12), 4, 20);
}

LiquidDemo::LiquidDemo()
{
    _title = "Liquid";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> WavesDemo::createEffect(float t)
{
    return std::make_unique<Waves>(t, Size(16,12), 4, 20, true, true);
}

WavesDemo::WavesDemo()
{
    _title = "Waves";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> TwirlDemo::createEffect(float t)
{
    auto size = Director::getInstance()->getWinSize();
    return std::make_unique<Twirl>(t, Size(12,8), Vec2(size.width/2, size.height/2), 1, 2.5f);
}

TwirlDemo::TwirlDemo()
{
    _title = "Twirl";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> ShakyTiles3DDemo::createEffect(float t)
{
    return std::make_unique<ShakyTiles3D>(t, Size(16,12), 5, false) ;
}

ShakyTiles3DDemo::ShakyTiles3DDemo()
{
    _title = "ShakyTiles3D";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> ShatteredTiles3DDemo::createEffect(float t)
{
    return std::make_unique<ShatteredTiles3D>(t, Size(16,12), 5, false);
}

ShatteredTiles3DDemo::ShatteredTiles3DDemo()
{
    _title = "ShatteredTiles3D";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> ShuffleTilesDemo::createEffect(float t)
{
    auto shuffle = std::make_unique<ShuffleTiles>(t, Size(16,12), 25);
    auto shuffle_back = std::unique_ptr<GridAction>(shuffle->reverse());
    auto delay = std::make_unique<DelayTime>(2);
    
    return std::make_unique<Sequence>( std::move(shuffle), std::move( delay), std::move( shuffle_back) );
}

ShuffleTilesDemo::ShuffleTilesDemo()
{
    _title = "ShuffleTiles";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> FadeOutTRTilesDemo::createEffect(float t)
{
    auto fadeout = std::make_unique<FadeOutTRTiles>(t, Size(16,12));
    auto back = std::unique_ptr<GridAction>(fadeout->reverse());
    auto delay = std::make_unique<DelayTime>(0.5f);
    
    return std::make_unique<Sequence>( std::move(fadeout), std::move( delay), std::move( back) );
}

FadeOutTRTilesDemo::FadeOutTRTilesDemo()
{
    _title = "FadeOutTRTiles";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> FadeOutBLTilesDemo::createEffect(float t)
{
    auto fadeout = std::make_unique<FadeOutBLTiles>(t, Size(16,12));
    auto back = std::unique_ptr<GridAction>(fadeout->reverse());
    auto delay = std::make_unique<DelayTime>(0.5f);
    
    return std::make_unique<Sequence>( std::move(fadeout), std::move( delay), std::move( back) );
}

FadeOutBLTilesDemo::FadeOutBLTilesDemo()
{
    _title = "FadeOutBLTiles";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> FadeOutUpTilesDemo::createEffect(float t)
{
    auto fadeout = std::make_unique<FadeOutUpTiles>(t, Size(16,12));
    auto back = std::unique_ptr<GridAction>(fadeout->reverse());
    auto delay = std::make_unique<DelayTime>(0.5f);
    
    return std::make_unique<Sequence>( std::move(fadeout), std::move( delay), std::move( back) );
}

FadeOutUpTilesDemo::FadeOutUpTilesDemo()
{
    _title = "FadeOutUpTiles";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> FadeOutDownTilesDemo::createEffect(float t)
{
    auto fadeout = std::make_unique<FadeOutDownTiles>(t, Size(16,12));
    auto back = std::unique_ptr<GridAction>(fadeout->reverse());
    auto delay = std::make_unique<DelayTime>(0.5f);
    
    return std::make_unique<Sequence>( std::move(fadeout), std::move( delay), std::move( back) );
}

FadeOutDownTilesDemo::FadeOutDownTilesDemo()
{
    _title = "FadeOutDownTiles";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> TurnOffTilesDemo::createEffect(float t)
{
    auto fadeout = std::make_unique<TurnOffTiles>(t, Size(48,32), 25);
    auto back = std::unique_ptr<GridAction>(fadeout->reverse());
    auto delay = std::make_unique<DelayTime>(0.5f);
    
    return std::make_unique<Sequence>( std::move(fadeout), std::move( delay), std::move( back) );
}

TurnOffTilesDemo::TurnOffTilesDemo()
{
    _title = "TurnOffTiles";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> WavesTiles3DDemo::createEffect(float t)
{
    return std::make_unique<WavesTiles3D>(t, Size(15,10), 4, 120);
}

WavesTiles3DDemo::WavesTiles3DDemo()
{
    _title = "WavesTiles3D";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> JumpTiles3DDemo::createEffect(float t)
{
    return std::make_unique<JumpTiles3D>(t, Size(15,10), 2, 30);
}

JumpTiles3DDemo::JumpTiles3DDemo()
{
    _title = "JumpTiles3D";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> SplitRowsDemo::createEffect(float t)
{
    return std::make_unique<SplitRows>(t, 9);
}

SplitRowsDemo::SplitRowsDemo()
{
    _title = "SplitRows";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> SplitColsDemo::createEffect(float t)
{
    return std::make_unique<SplitCols>(t, 9);
}

SplitColsDemo::SplitColsDemo()
{
    _title = "SplitCols";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> PageTurn3DDemo::createEffect(float t)
{
    return std::make_unique<PageTurn3D>(t, Size(15,10));
}

PageTurn3DDemo::PageTurn3DDemo()
{
    _title = "PageTurn3D";
    _subtitle = "";
}

std::unique_ptr<cocos2d::ActionInterval> PageTurn3DRectDemo::createEffect(float t)
{
    return std::make_unique<PageTurn3D>(t, Size(15,10));
}

PageTurn3DRectDemo::PageTurn3DRectDemo()
{
    _title = "PageTurn3D-Rect";
    _subtitle = "";
}

#define SID_RESTART        1

EffectBaseTest::EffectBaseTest()
: _gridNodeTarget(nullptr)
{
    
}

bool EffectBaseTest::init()
{
    if(TestCase::init())
    {
        LayerColor *background = LayerColor::create( Color4B(32,128,32,255) );
        this->addChild(background,-20);
        if(isRectEffect())
        {
            Size visibleSize = Director::getInstance()->getVisibleSize();
            Rect gridRect = Rect(visibleSize.width * 0.2,
                                 visibleSize.height * 0.2,
                                 visibleSize.width * 0.6,
                                 visibleSize.height * 0.6);
            _gridNodeTarget = NodeGrid::create(gridRect);
        }
        else
        {
            _gridNodeTarget = NodeGrid::create();
        }
        addChild(_gridNodeTarget, 0);
        _gridNodeTarget->runAction(createEffect(3));
        
        auto bg = Sprite::create(s_back3);
        _gridNodeTarget->addChild(bg, 0);
        bg->setPosition(VisibleRect::center());
        
        auto grossini = Sprite::create(s_pathSister2);
        _gridNodeTarget->addChild(grossini, 1);
        grossini->setPosition(VisibleRect::left().x+VisibleRect::getVisibleRect().size.width/3,VisibleRect::center().y);

        auto sc = std::make_unique<ScaleBy>(2, 5);
        auto sc_back = std::unique_ptr<ScaleBy>(sc->reverse());
        grossini->runAction(
            std::make_unique<RepeatForever>(
                std::make_unique<Sequence>(
                    std::move(sc),
                    std::move(sc_back)
                )));
        
        auto tamara = Sprite::create(s_pathSister1);
        _gridNodeTarget->addChild(tamara, 1);
        tamara->setPosition(VisibleRect::left().x+2*VisibleRect::getVisibleRect().size.width/3,VisibleRect::center().y);

        auto sc2 = std::make_unique<ScaleBy>(2, 5);
        auto sc2_back = std::unique_ptr<ScaleBy>(sc2->reverse());
        tamara->runAction(
            std::make_unique<RepeatForever>(
                std::make_unique<Sequence>(
                    std::move(sc2),
                    std::move(sc2_back)
                )));
        
        Director::getInstance()->getScheduler().schedule(
            TimedJob(this, &EffectBaseTest::checkAnim)
                .paused(isPaused())
        );
        
        return true;
    }
    return false;
}

void EffectBaseTest::checkAnim(float /*dt*/)
{
    if ( _gridNodeTarget->getNumberOfRunningActions() == 0 && _gridNodeTarget->getGrid() != nullptr)
        _gridNodeTarget->setGrid(nullptr);
}

EffectBaseTest::~EffectBaseTest(void)
{
}

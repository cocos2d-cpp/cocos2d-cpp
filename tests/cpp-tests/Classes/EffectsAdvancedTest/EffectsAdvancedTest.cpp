#include "EffectsAdvancedTest.h"

#include "2d/CCActionGrid3D.h"
#include "2d/CCActionTiledGrid.h"
#include "2d/CCNodeGrid.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "renderer/CCTextureAtlas.h"

using namespace cocos2d;

enum 
{
    kTagTextLayer = 1,

    kTagSprite1 = 1,
    kTagSprite2 = 2,

    kTagBackground = 1,
    kTagLabel = 2,
};

EffectAdvanceTests::EffectAdvanceTests()
{
    ADD_TEST_CASE(Effect3);
    ADD_TEST_CASE(Effect2);
    ADD_TEST_CASE(Effect1);
    ADD_TEST_CASE(Effect4);
    ADD_TEST_CASE(Effect5);
    ADD_TEST_CASE(Issue631);
}

//------------------------------------------------------------------
//
// Effect1
//
//------------------------------------------------------------------
void Effect1::onEnter()
{
    EffectAdvanceBaseTest::onEnter();

    //auto target = getChildByTag(kTagBackground);
    
    // To reuse a grid the grid size and the grid type must be the same.
    // in this case:
    //     Lens3D is Grid3D and it's size is (15,10)
    //     Waves3D is Grid3D and it's size is (15,10)
    
    auto size = Director::getInstance()->getWinSize();
    auto lens = std::make_unique<Lens3D>(0.0f, Size(15,10), Vec2(size.width/2,size.height/2), 240);
    auto waves = std::make_unique<Waves3D>(10, Size(15,10), 18, 15);

    auto reuse = std::make_unique<ReuseGrid>(1);
    auto delay = std::make_unique<DelayTime>(8);

    _bgNode->runAction( std::make_unique<Sequence>( std::move(lens), std::move( delay), std::move( reuse), std::move( waves) ) );
}

std::string Effect1::title() const
{
    return "Lens + Waves3d and OrbitCamera";
}

//------------------------------------------------------------------
//
// Effect2
//
//------------------------------------------------------------------
void Effect2::onEnter()
{
    EffectAdvanceBaseTest::onEnter();

    //auto target = getChildByTag(kTagBackground);
    
    // To reuse a grid the grid size and the grid type must be the same.
    // in this case:
    //     ShakyTiles is TiledGrid3D and it's size is (15,10)
    //     Shuffletiles is TiledGrid3D and it's size is (15,10)
    //       TurnOfftiles is TiledGrid3D and it's size is (15,10)
    auto shaky = std::make_unique<ShakyTiles3D>(5, Size(15,10), 4, false);
    auto shuffle = std::make_unique<ShuffleTiles>(0, Size(15,10), 3);
    auto turnoff = std::make_unique<TurnOffTiles>(0, Size(15,10), 3);
    auto turnon = std::unique_ptr<GridAction>(turnoff->reverse());
    
    // reuse 2 times:
    //   1 for shuffle
    //   2 for turn off
    //   turnon tiles will use a new grid
    auto reuse = std::make_unique<ReuseGrid>(2);

    auto delay = std::make_unique<DelayTime>(1);
    auto delay_clone = std::unique_ptr<DelayTime>(delay->clone());
    
    _bgNode->runAction(
        std::make_unique<Sequence>(
            std::move(shaky),
            std::move(delay),
            std::move(reuse),
            std::move(shuffle),
            std::move(delay_clone),
            std::move(turnoff),
            std::move(turnon)
        ));
}

std::string Effect2::title() const
{
    return "ShakyTiles + ShuffleTiles + TurnOffTiles";
}


//------------------------------------------------------------------
//
// Effect3
//
//------------------------------------------------------------------
void Effect3::onEnter()
{
    EffectAdvanceBaseTest::onEnter();

    auto waves = std::make_unique<Waves>(5, Size(15,10), 5, 20, true, false);
    auto shaky = std::make_unique<Shaky3D>(5, Size(15,10), 4, false);
    
    _target1->runAction( std::make_unique<RepeatForever>( std::move(waves) ) );
    _target2->runAction( std::make_unique<RepeatForever>( std::move(shaky) ) );
    
    // moving background. Testing issue #244
    auto move = std::make_unique<MoveBy>(3, Vec2(200,0) );
    auto move_reverse = std::unique_ptr<MoveBy>(move->reverse());
    _bgNode->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(move),
                std::move(move_reverse)
            )));    
}

std::string Effect3::title() const
{
    return "Effects on 2 sprites";
}


//------------------------------------------------------------------
//
// Effect4
//
//------------------------------------------------------------------

class Lens3DTarget : public Node
{
public:
    virtual void setPosition(const Vec2& var)
    {
        _lens3D->setPosition(var);
    }
    
    virtual const Vec2& getPosition() const
    {
        return _lens3D->getPosition();
    }
    
    static Lens3DTarget* create(Lens3D* pAction)
    {
        Lens3DTarget* pRet = new (std::nothrow) Lens3DTarget();
        pRet->_lens3D = pAction;
        pRet->autorelease();
        return pRet;
    }
private:

    Lens3DTarget()
        : _lens3D(nullptr)
    {}

    Lens3D* _lens3D;
};

void Effect4::onEnter()
{
    EffectAdvanceBaseTest::onEnter();
    //Node* gridNode = NodeGrid::create();
    
    auto lens = std::make_unique<Lens3D>(10, Size(32,24), Vec2(100,180), 150);
    auto move = std::make_unique<JumpBy>(5, Vec2(380,0), 100, 4);
    auto move_back = std::unique_ptr<JumpBy>(move->reverse());
    auto seq = std::make_unique<Sequence>( std::move( move), std::move( move_back) );

    /* In cocos2d-iphone, the type of action's target is 'id', so it supports using the instance of 'Lens3D' as its target.
        While in cocos2d-x, the target of action only supports Node or its subclass,
        so we make an encapsulation for Lens3D to achieve that.
    */

    auto pTarget = Lens3DTarget::create(lens.get());

    // Please make sure the target been added to its parent.
    this->addChild(pTarget);

    pTarget->runAction( std::move(seq) );
    
    _bgNode->runAction( std::move(lens) );
}

std::string Effect4::title() const
{
    return "Jumpy Lens3D";
}

//------------------------------------------------------------------
//
// Effect5
//
//------------------------------------------------------------------
void Effect5::onEnter()
{
    EffectAdvanceBaseTest::onEnter();

    //CCDirector::getInstance()->setProjection(DirectorProjection2D);
    
    auto effect = std::make_unique<Liquid>(2, Size(32,24), 1, 20);    

    auto stopEffect = std::make_unique<Sequence>(
        std::move(effect),
        std::make_unique<DelayTime>(2),
        std::make_unique<StopGrid>()
    );
    
    _bgNode->runAction( std::move( stopEffect));
}

std::string Effect5::title() const
{
    return "Test Stop-Copy-Restar";
}

void Effect5::onExit()
{
    EffectAdvanceBaseTest::onExit();

    Director::getInstance()->setProjection(Director::Projection::_3D);
}

//------------------------------------------------------------------
//
// Effect5
//
//------------------------------------------------------------------
void Issue631::onEnter()
{
    EffectAdvanceBaseTest::onEnter();
        
    auto effect = std::make_unique<Sequence>(
        std::make_unique<DelayTime>(2.0f),
        std::make_unique<Shaky3D>(5.0f, Size(5, 5), 16, false)
    );

    // cleanup
    //auto bg = getChildByTag(kTagBackground);
    removeChild(_bgNode, true);

    // background
    auto layer = LayerColor::create( Color4B(255,0,0,255) );
    addChild(layer, -10);
    auto sprite = Sprite::create("Images/grossini.png");
    sprite->setPosition(50,80);
    layer->addChild(sprite, 10);
    
    // foreground
    auto layer2BaseGrid = NodeGrid::create();
    auto layer2 = LayerColor::create(Color4B( 0, 255,0,255 ) );
    auto fog = Sprite::create("Images/Fog.png");

    BlendFunc bf = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
    fog->setBlendFunc(bf);
    layer2->addChild(fog, 1);
    addChild(layer2BaseGrid, 1);
    layer2BaseGrid->addChild(layer2);
    
    layer2BaseGrid->runAction( std::make_unique<RepeatForever>( std::move(effect)));
}

std::string Issue631::title() const
{
    return "Testing Opacity";
}

std::string Issue631::subtitle() const
{
    return "Effect image should be 100% opaque. Testing issue #631";
}

//------------------------------------------------------------------
//
// EffectAdvanceBaseTest
//
//------------------------------------------------------------------

void EffectAdvanceBaseTest::onEnter(void)
{
    TestCase::onEnter();
    
    _bgNode = NodeGrid::create();
    _bgNode->setAnchorPoint(Vec2(0.5,0.5));
    addChild(_bgNode);
    //_bgNode->setPosition( VisibleRect::center() );
    auto bg = Sprite::create("Images/background3.png");
    bg->setPosition( VisibleRect::center() );
    //addChild(bg, 0, kTagBackground);
    //bg->setPosition( VisibleRect::center() );
    _bgNode->addChild(bg);

    _target1 = NodeGrid::create();
    _target1->setAnchorPoint(Vec2(0.5,0.5));
    auto grossini = Sprite::create("Images/grossinis_sister2.png");
    _target1->addChild(grossini);
    _bgNode->addChild(_target1);
    _target1->setPosition(VisibleRect::left().x+VisibleRect::getVisibleRect().size.width/3.0f, VisibleRect::bottom().y+ 200);
    auto sc = std::make_unique<ScaleBy>(2, 5);
    auto sc_back = std::unique_ptr<ScaleBy>(sc->reverse());
    _target1->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(sc),
                std::move(sc_back)
            )));


    _target2 = NodeGrid::create();
    _target2->setAnchorPoint(Vec2(0.5,0.5));
    auto tamara = Sprite::create("Images/grossinis_sister1.png");
    _target2->addChild(tamara);
    _bgNode->addChild(_target2);
    _target2->setPosition(VisibleRect::left().x+2*VisibleRect::getVisibleRect().size.width/3.0f,VisibleRect::bottom().y+200);
    auto sc2 = std::make_unique<ScaleBy>(2, 5);
    auto sc2_back = std::unique_ptr<ScaleBy>(sc2->reverse());
    _target2->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(sc2),
                std::move(sc2_back)
            )));    

}

EffectAdvanceBaseTest::~EffectAdvanceBaseTest(void)
{
}

std::string EffectAdvanceBaseTest::title() const
{
    return "No title";
}

std::string EffectAdvanceBaseTest::subtitle() const
{
    return "";
}

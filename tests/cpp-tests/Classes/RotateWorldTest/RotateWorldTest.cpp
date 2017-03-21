#include "RotateWorldTest.h"
#include "../testResource.h"

#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"

using namespace cocos2d;

RotateWorldTests::RotateWorldTests()
{
    ADD_TEST_CASE(RotateWorldTest);
}

//------------------------------------------------------------------
//
// TestLayer
//
//------------------------------------------------------------------
void TestLayer::onEnter()
{
    Layer::onEnter();

    float x,y;
    
    auto size = Director::getInstance()->getWinSize();
    x = size.width;
    y = size.height;

    //auto array = [UIFont familyNames];
    //for( String *s in array )
    //    NSLog( s );
    auto label = Label::createWithSystemFont("cocos2d", "Tahoma", 64);

    label->setPosition( Vec2(x/2,y/2) );
    
    addChild(label);
}

//------------------------------------------------------------------
//
// SpriteLayer
//
//------------------------------------------------------------------
void SpriteLayer::onEnter()
{
    Layer::onEnter();

    float x,y;
    
    auto size = Director::getInstance()->getWinSize();
    x = size.width;
    y = size.height;
    
    auto sprite = Sprite::create(s_pathGrossini);
    auto spriteSister1 = Sprite::create(s_pathSister1);
    auto spriteSister2 = Sprite::create(s_pathSister2);
    
    sprite->setScale(1.5f);
    spriteSister1->setScale(1.5f);
    spriteSister2->setScale(1.5f);
    
    sprite->setPosition(Vec2(x/2,y/2));
    spriteSister1->setPosition(Vec2(40,y/2));
    spriteSister2->setPosition(Vec2(x-40,y/2));

    addChild(sprite);
    addChild(spriteSister1);
    addChild(spriteSister2);
    
    sprite->runAction( std::make_unique<RotateBy>(16, -3600));

    auto jump1 = std::make_unique<JumpBy>(4, Vec2(-400,0), 100, 4);
    auto jump1_clone = std::unique_ptr<JumpBy>(jump1->clone());
    auto jump2 = std::unique_ptr<JumpBy>(jump1->reverse());
    auto jump2_clone = std::unique_ptr<JumpBy>(jump1->clone());
    
    auto rot1 = std::make_unique<RotateBy>(4, 360 * 2);
    auto rot1_clone = std::unique_ptr<RotateBy>(rot1->clone());
    auto rot2 = std::unique_ptr<RotateBy>(rot1->reverse());
    auto rot2_clone = std::unique_ptr<RotateBy>(rot2->clone());
    
    spriteSister1->runAction(
        std::make_unique<Repeat>(
            std::make_unique<Sequence>(
                std::move(jump2),
                std::move(jump1)
            ),
            5
        ));
    spriteSister2->runAction(
        std::make_unique<Repeat>(
            std::make_unique<Sequence>(
                std::move(jump1_clone),
                std::move( jump2_clone)
            ),
            5
        ));
    
    spriteSister1->runAction(
        std::make_unique<Repeat>(
            std::make_unique<Sequence>(
                std::move(rot1),
                std::move(rot2)
            ),
            5
        ));
    spriteSister2->runAction(
        std::make_unique<Repeat>(
            std::make_unique<Sequence>(
                std::move(rot2_clone),
                std::move(rot1_clone)
            ),
            5
        ));
}

//------------------------------------------------------------------
//
// RotateWorldMainLayer
//
//------------------------------------------------------------------

void RotateWorldMainLayer::onEnter()
{
    Layer::onEnter();

    float x,y;
    
    auto size = Director::getInstance()->getWinSize();
    x = size.width;
    y = size.height;
    
    auto blue =  LayerColor::create(Color4B(0,0,255,255));
    auto red =   LayerColor::create(Color4B(255,0,0,255));
    auto green = LayerColor::create(Color4B(0,255,0,255));
    auto white = LayerColor::create(Color4B(255,255,255,255));

    blue->setScale(0.5f);
    blue->setPosition(Vec2(-x/4,-y/4));
    blue->addChild( SpriteLayer::create() );
    
    red->setScale(0.5f);
    red->setPosition(Vec2(x/4,-y/4));

    green->setScale(0.5f);
    green->setPosition(Vec2(-x/4,y/4));
    green->addChild(TestLayer::create());

    white->setScale(0.5f);
    white->setPosition(Vec2(x/4,y/4));
    white->setIgnoreAnchorPointForPosition(false);
    white->setPosition(Vec2(x/4*3,y/4*3));

    addChild(blue, -1);
    addChild(white);
    addChild(green);
    addChild(red);

    auto rot = std::make_unique<RotateBy>(8, 720);
    
    blue->runAction( std::unique_ptr<RotateBy>(rot->clone()) );
    red->runAction( std::unique_ptr<RotateBy>(rot->clone()) );
    green->runAction( std::unique_ptr<RotateBy>(rot->clone()) );
    white->runAction( std::unique_ptr<RotateBy>(rot->clone()) );
}

bool RotateWorldTest::init()
{
    if (TestCase::init())
    {
        auto layer = RotateWorldMainLayer::create();

        addChild(layer);
        runAction( std::make_unique<RotateBy>(4, -360));

        return true;
    }

    return false;
}

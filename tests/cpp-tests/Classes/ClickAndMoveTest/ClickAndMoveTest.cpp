#include "ClickAndMoveTest.h"
#include "../testResource.h"

#include "2d/CCSprite.h"
#include "base/CCEventDispatcher.h"

using namespace cocos2d;

enum
{
    kTagSprite = 1,
};

ClickAndMoveTest::ClickAndMoveTest()
{
    ADD_TEST_CASE(ClickAndMoveTestCase);
}

ClickAndMoveTestCase::ClickAndMoveTestCase()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(ClickAndMoveTestCase::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(ClickAndMoveTestCase::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto sprite = Sprite::create(s_pathGrossini);
    
    auto layer = LayerColor::create(Color4B(255,255,0,255));
    addChild(layer, -1);
        
    addChild(sprite, 0, kTagSprite);
    sprite->setPosition(20,150);
    
    sprite->runAction( std::make_unique<JumpTo>(4, Vec2(300,48), 100, 4) );
    
    layer->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::make_unique<FadeIn>(1),
                std::make_unique<FadeOut>(1)
            )
        )
    ); 
}

bool ClickAndMoveTestCase::onTouchBegan(Touch*, Event*)
{
    return true;
}

void ClickAndMoveTestCase::onTouchEnded(Touch* touch, Event*)
{
    auto location = touch->getLocation();

    auto s = getChildByTag(kTagSprite);
    s->stopAllActions();
    s->runAction( std::make_unique<MoveTo>(1, Vec2(location.x, location.y) ) );
    float o = location.x - s->getPosition().x;
    float a = location.y - s->getPosition().y;
    float at = (float) CC_RADIANS_TO_DEGREES( atanf( o/a) );
    
    if( a < 0 ) 
    {
        if(  o < 0 )
            at = 180 + fabs(at);
        else
            at = 180 - fabs(at);    
    }
    
    s->runAction( std::make_unique<RotateTo>(1, at) );
}

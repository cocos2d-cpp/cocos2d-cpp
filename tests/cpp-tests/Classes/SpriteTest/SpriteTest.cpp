/****************************************************************************
 Copyright (c) 2012 cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017      Iakov Sergeev <yahont@github>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "SpriteTest.h"

#include "2d/CCActionCamera.h"
#include "2d/CCActionInstant.h"
#include "2d/CCAnimationCache.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteBatchNode.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramCache.h"
#include "renderer/CCTexture2D.h"
#include "renderer/CCTextureCache.h"
#include "platform/CCFileUtils.h"

#include <vector>
#include <cmath>
#include <algorithm>

using namespace cocos2d;

enum 
{
    kTagTileMap = 1,
    kTagSpriteBatchNode = 1,
    kTagNode = 2,
    kTagAnimation1 = 1,
    kTagSpriteLeft,
    kTagSpriteRight,
};

enum 
{
    kTagSprite1,
    kTagSprite2,
    kTagSprite3,
    kTagSprite4,
    kTagSprite5,
    kTagSprite6,
    kTagSprite7,
    kTagSprite8,
};

SpriteTests::SpriteTests()
{
    ADD_TEST_CASE(Sprite1);
    ADD_TEST_CASE(Sprite1ETC1Alpha);
    ADD_TEST_CASE(SpriteBatchNode1);
    ADD_TEST_CASE(SpriteAnchorPoint);
    ADD_TEST_CASE(SpriteBatchNodeAnchorPoint);
    ADD_TEST_CASE(SpriteAnchorPointFromFile);
    ADD_TEST_CASE(SpriteOffsetAnchorRotation);
    ADD_TEST_CASE(SpriteBatchNodeOffsetAnchorRotation);
    ADD_TEST_CASE(SpriteOffsetAnchorScale);
    ADD_TEST_CASE(SpriteBatchNodeOffsetAnchorScale);
    ADD_TEST_CASE(SpriteOffsetAnchorSkew);
    ADD_TEST_CASE(SpriteBatchNodeOffsetAnchorSkew);
    ADD_TEST_CASE(SpriteOffsetAnchorRotationalSkew);
    ADD_TEST_CASE(SpriteBatchNodeOffsetAnchorRotationalSkew);
    ADD_TEST_CASE(SpriteOffsetAnchorSkewScale);
    ADD_TEST_CASE(SpriteBatchNodeOffsetAnchorSkewScale);
    ADD_TEST_CASE(SpriteOffsetAnchorRotationalSkewScale);
    ADD_TEST_CASE(SpriteBatchNodeOffsetAnchorRotationalSkewScale);
    ADD_TEST_CASE(SpriteSkewNegativeScaleChildren);
    ADD_TEST_CASE(SpriteBatchNodeSkewNegativeScaleChildren);
    ADD_TEST_CASE(SpriteRotationalSkewNegativeScaleChildren);
    ADD_TEST_CASE(SpriteBatchNodeRotationalSkewNegativeScaleChildren);
    ADD_TEST_CASE(SpriteOffsetAnchorFlip);
    ADD_TEST_CASE(SpriteBatchNodeOffsetAnchorFlip);
    ADD_TEST_CASE(SpriteChildrenChildren);
    ADD_TEST_CASE(SpriteBatchNodeChildrenChildren);
    ADD_TEST_CASE(SpriteChildrenAnchorPoint);
    ADD_TEST_CASE(SpriteBatchNodeChildrenAnchorPoint);
    ADD_TEST_CASE(SpriteColorOpacity);
    ADD_TEST_CASE(SpriteBatchNodeColorOpacity);
    ADD_TEST_CASE(SpriteZOrder);
    ADD_TEST_CASE(SpriteBatchNodeZOrder);
    ADD_TEST_CASE(SpriteZVertex);
    ADD_TEST_CASE(SpriteBatchNodeZVertex);
    ADD_TEST_CASE(SpriteAliased);
    ADD_TEST_CASE(SpriteBatchNodeAliased);
    ADD_TEST_CASE(SpriteNewTexture);
    ADD_TEST_CASE(SpriteBatchNodeNewTexture);
    ADD_TEST_CASE(SpriteFlip);
    ADD_TEST_CASE(SpriteBatchNodeFlip);
    ADD_TEST_CASE(SpriteAnimationSplit);
    ADD_TEST_CASE(SpriteFrameTest);
    ADD_TEST_CASE(SpriteFrameAliasNameTest);
    ADD_TEST_CASE(SpriteFramesFromFileContent);
    ADD_TEST_CASE(SpritePolygonFromFileContent);
    ADD_TEST_CASE(SpriteBatchNodeReorder);
    ADD_TEST_CASE(SpriteBatchNodeReorderIssue744);
    ADD_TEST_CASE(SpriteBatchNodeReorderIssue766);
    ADD_TEST_CASE(SpriteBatchNodeReorderIssue767);
    ADD_TEST_CASE(SpriteBatchNodeReorderSameIndex);
    ADD_TEST_CASE(SpriteBatchNodeReorderOneChild);
    ADD_TEST_CASE(NodeSort);
    ADD_TEST_CASE(Sprite6);
    ADD_TEST_CASE(SpriteHybrid);
    ADD_TEST_CASE(SpriteBatchNodeChildren);
    ADD_TEST_CASE(SpriteBatchNodeChildrenZ);
    ADD_TEST_CASE(SpriteChildrenVisibility);
    ADD_TEST_CASE(SpriteChildrenVisibilityIssue665);
    ADD_TEST_CASE(SpriteBatchNodeChildrenScale);
    ADD_TEST_CASE(SpriteNilTexture);
    ADD_TEST_CASE(SpriteSubclass);
    ADD_TEST_CASE(SpriteDoubleResolution);
    ADD_TEST_CASE(SpriteBatchBug1217);
    ADD_TEST_CASE(AnimationCacheTest);
    ADD_TEST_CASE(AnimationCacheFile);
    ADD_TEST_CASE(SpriteCullTest1);
    ADD_TEST_CASE(SpriteCullTest2);
    ADD_TEST_CASE(Sprite3DRotationTest);
    ADD_TEST_CASE(SpriteGetSpriteFrameTest);
    ADD_TEST_CASE(SpriteSlice9Test1);
    ADD_TEST_CASE(SpriteSlice9Test2);
    ADD_TEST_CASE(SpriteSlice9Test3);
    ADD_TEST_CASE(SpriteSlice9Test4);
    ADD_TEST_CASE(SpriteSlice9Test5);
    ADD_TEST_CASE(SpriteSlice9Test6);
    ADD_TEST_CASE(SpriteSlice9Test7);
    ADD_TEST_CASE(SpriteSlice9Test8);
    ADD_TEST_CASE(SpriteSlice9Test9);
    ADD_TEST_CASE(SpriteSlice9Test10);
};

static void run_grossini_dance_animation_forever(cocos2d::Sprite* sprite)
{
    std::vector<retaining_ptr<SpriteFrame>> animFrames;
    animFrames.reserve(14);
    char tmp[50];

    for (int i = 1; i < 15; i++)
    {            
        sprintf(tmp, "grossini_dance_%02d.png", i);
        auto frame = Director::getInstance()->getSpriteFrameCache().getSpriteFrameByName(tmp);
        animFrames.push_back(to_retaining_ptr(frame));
    }

    std::unique_ptr<Animation> animation(new Animation( std::move(animFrames), 0.3f));

    sprite->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Animate>( std::move( animation))
        ));
}

//------------------------------------------------------------------
//
// Sprite1
//
//------------------------------------------------------------------

Sprite1::Sprite1()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(Sprite1::onTouchesEnded, this);
    _director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto s = Director::getInstance()->getWinSize();
    addNewSpriteWithCoords( Vec2(s.width/2, s.height/2) );
}

void Sprite1::addNewSpriteWithCoords(Vec2 p)
{
    int idx = (int)(CCRANDOM_0_1() * 1400.0f / 100.0f);
    int x = (idx%5) * 85;
    int y = (idx/5) * 121;
    
    std::unique_ptr<FiniteTimeAction> action;
    float random = CCRANDOM_0_1();
    
    if( random < 0.20 )
        action.reset(new ScaleBy(3, 2));
    else if(random < 0.40)
        action.reset(new RotateBy(3, 360));
    else if( random < 0.60)
        action.reset(new Blink(1, 3));
    else if( random < 0.8 )
        action.reset(new TintBy(2, 0, -255, -255));
    else 
        action.reset(new FadeOut(2));

    auto action_back = std::unique_ptr<FiniteTimeAction>(action->reverse());
    
    auto sprite = make_node_ptr<Sprite>("Images/grossini_dance_atlas.png", Rect(x,y,85,121) );

    sprite->setPosition( Vec2( p.x, p.y) );
    
    sprite->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(action),
                std::move(action_back)
            )));
    
    addChild( std::move(sprite) );
}

void Sprite1::onTouchesEnded(const std::vector<Touch*>& touches, Event*)
{
    for (auto touch: touches)
    {
        auto location = touch->getLocation();
    
        addNewSpriteWithCoords( location );
    }
}

std::string Sprite1::title() const
{
    return "Testing Sprite";
}

std::string Sprite1::subtitle() const
{
    return "Tap screen to add more sprites";
}

//------------------------------------------------------------------
//
// Sprite1ETC1Alpha
//
//------------------------------------------------------------------

Sprite1ETC1Alpha::Sprite1ETC1Alpha()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(Sprite1ETC1Alpha::onTouchesEnded, this);
    _director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool Sprite1ETC1Alpha::init()
{
    if (!SpriteTestDemo::init())
        return false;

    _background = Sprite::create("Images/background2.png");
    auto s = Director::getInstance()->getWinSize();
    _background->setPosition(Vec2(s.width / 2, s.height / 2));
    this->addChild(_background);

    addNewSpriteWithCoords(Vec2(s.width / 2, s.height / 2));
    return true;
}

void Sprite1ETC1Alpha::addNewSpriteWithCoords(Vec2 p)
{
    std::unique_ptr<FiniteTimeAction> action;
    float random = CCRANDOM_0_1();

    if (random < 0.20)
        action.reset(new ScaleBy(3, 2));
    else if (random < 0.40)
        action.reset(new RotateBy(3, 360));
    else if (random < 0.60)
        action.reset(new Blink(1, 3));
    else if (random < 0.8)
        action.reset(new TintBy(2, 0, -255, -255));
    else
        action.reset(new FadeOut(2));
    
    auto action_back = std::unique_ptr<FiniteTimeAction>(action->reverse());

    auto sprite = make_node_ptr<Sprite>("Images/grossini_dance_08.png");
    Texture2D *etcTexture = _director->getTextureCache()->addImage("Images/etc1-alpha.pkm");
    sprite->setTexture(etcTexture);
    sprite->setPosition(Vec2(p.x, p.y));
    sprite->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(action),
                std::move(action_back)
            )));
    _background->addChild( std::move(sprite) );
}

void Sprite1ETC1Alpha::onTouchesEnded(const std::vector<Touch*>& touches, Event*)
{
    for (auto touch : touches)
    {
        auto location = touch->getLocation();

        addNewSpriteWithCoords(location);
    }
}

std::string Sprite1ETC1Alpha::title() const
{
    return "Testing Sprite ETC1 Alpha support";
}

std::string Sprite1ETC1Alpha::subtitle() const
{
    return "Tap screen to add more sprites";
}

//------------------------------------------------------------------
//
// SpriteBatchNode1
//
//------------------------------------------------------------------

SpriteBatchNode1::SpriteBatchNode1()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(SpriteBatchNode1::onTouchesEnded, this);
    _director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    addChild(make_node_ptr<SpriteBatchNode>("Images/grossini_dance_atlas.png", 50),
             0,
             kTagSpriteBatchNode);
    
    auto s = Director::getInstance()->getWinSize();
    addNewSpriteWithCoords( Vec2(s.width/2, s.height/2) );
}

void SpriteBatchNode1::addNewSpriteWithCoords(Vec2 p)
{
    auto BatchNode = static_cast<SpriteBatchNode*>( getChildByTag(kTagSpriteBatchNode) );
    
    int idx = CCRANDOM_0_1() * 1400 / 100;
    int x = (idx%5) * 85;
    int y = (idx/5) * 121;
    
    std::unique_ptr<ActionInterval> action;
    float random = CCRANDOM_0_1();
    
    if( random < 0.20 )
        action.reset(new ScaleBy(3, 2));
    else if(random < 0.40)
        action.reset(new RotateBy(3, 360));
    else if( random < 0.60)
        action.reset(new Blink(1, 3));
    else if( random < 0.8 )
        action.reset(new TintBy(2, 0, -255, -255));
    else 
        action.reset(new FadeOut(2));

    auto action_back = std::unique_ptr<ActionInterval>(action->reverse());
    
    auto sprite = make_node_ptr<Sprite>(BatchNode->getTexture(), Rect(x, y, 85, 121));
    sprite->setPosition( Vec2( p.x, p.y) );

    sprite->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(action),
                std::move(action_back)
            )));

    BatchNode->addChild( std::move(sprite) );
}

void SpriteBatchNode1::onTouchesEnded(const std::vector<Touch*>& touches, Event*)
{
    for (auto &touch: touches)
    {
        auto location = touch->getLocation();
            
        addNewSpriteWithCoords( location );
    }

}

std::string SpriteBatchNode1::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNode1::subtitle() const
{
    return "SpriteBatchNode: Tap screen to add more sprites";
}


//------------------------------------------------------------------
//
// SpriteColorOpacity
//
//------------------------------------------------------------------

SpriteColorOpacity::SpriteColorOpacity()
{
    auto sprite1 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*0, 121*1, 85, 121));
    auto sprite2 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*1, 85, 121));
    auto sprite3 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*2, 121*1, 85, 121));
    auto sprite4 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*3, 121*1, 85, 121));
    
    auto sprite5 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*0, 121*1, 85, 121));
    auto sprite6 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*1, 85, 121));
    auto sprite7 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*2, 121*1, 85, 121));
    auto sprite8 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*3, 121*1, 85, 121));
    
    auto s = Director::getInstance()->getWinSize();
    sprite1->setPosition( Vec2( (s.width/5)*1, (s.height/3)*1) );
    sprite2->setPosition( Vec2( (s.width/5)*2, (s.height/3)*1) );
    sprite3->setPosition( Vec2( (s.width/5)*3, (s.height/3)*1) );
    sprite4->setPosition( Vec2( (s.width/5)*4, (s.height/3)*1) );
    sprite5->setPosition( Vec2( (s.width/5)*1, (s.height/3)*2) );
    sprite6->setPosition( Vec2( (s.width/5)*2, (s.height/3)*2) );
    sprite7->setPosition( Vec2( (s.width/5)*3, (s.height/3)*2) );
    sprite8->setPosition( Vec2( (s.width/5)*4, (s.height/3)*2) );
    
    auto action = std::make_unique<FadeIn>(2);
    auto action_back = std::unique_ptr<FadeTo>(action->reverse());
    auto fade = std::make_unique<RepeatForever>( std::make_unique<Sequence>( std::move( action), std::move( action_back) ) );
    
    auto tintred = std::make_unique<TintBy>(2, 0, -255, -255);
    auto tintred_back = std::unique_ptr<TintBy>(tintred->reverse());
    auto red = std::make_unique<RepeatForever>( std::make_unique<Sequence>( std::move( tintred), std::move( tintred_back) ) );
    
    auto tintgreen = std::make_unique<TintBy>(2, -255, 0, -255);
    auto tintgreen_back = std::unique_ptr<TintBy>(tintgreen->reverse());
    auto green = std::make_unique<RepeatForever>( std::make_unique<Sequence>( std::move( tintgreen), std::move( tintgreen_back) ) );
    
    auto tintblue = std::make_unique<TintBy>(2, -255, -255, 0);
    auto tintblue_back = std::unique_ptr<TintBy>(tintblue->reverse());
    auto blue = std::make_unique<RepeatForever>( std::make_unique<Sequence>( std::move( tintblue), std::move( tintblue_back) ) );
    
    sprite5->runAction( std::move( red));
    sprite6->runAction( std::move( green));
    sprite7->runAction( std::move( blue));
    sprite8->runAction( std::move( fade));
    
    // late add: test dirtyColor and dirtyPosition
    addChild(sprite1, 0, kTagSprite1);
    addChild(sprite2, 0, kTagSprite2);
    addChild(sprite3, 0, kTagSprite3);
    addChild(sprite4, 0, kTagSprite4);
    addChild(sprite5, 0, kTagSprite5);
    addChild(sprite6, 0, kTagSprite6);
    addChild(sprite7, 0, kTagSprite7);
    addChild(sprite8, 0, kTagSprite8);
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteColorOpacity::removeAndAddSprite, 0)
            .delay(2.0f)
            .interval(2.0f)
            .paused(isPaused())
    );
}

// this function test if remove and add works as expected:
//   color array and vertex array should be reindexed
void SpriteColorOpacity::removeAndAddSprite(float)
{
    auto sprite = static_cast<Sprite*>( getChildByTag(kTagSprite5) );
    sprite->retain();
    
    removeChild(sprite, false);
    addChild(sprite, 0, kTagSprite5);
    
    sprite->release();
}

std::string SpriteColorOpacity::title() const
{
    return "Testing Sprite";
}

std::string SpriteColorOpacity::subtitle() const
{
    return "Color & Opacity";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeColorOpacity
//
//------------------------------------------------------------------

SpriteBatchNodeColorOpacity::SpriteBatchNodeColorOpacity()
{
    // small capacity. Testing resizing.
    // Don't use capacity=1 in your real game. It is expensive to resize the capacity
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 1);
    addChild(batch, 0, kTagSpriteBatchNode);        
    
    auto sprite1 = Sprite::create(batch->getTexture(), Rect(85*0, 121*1, 85, 121));
    auto sprite2 = Sprite::create(batch->getTexture(), Rect(85*1, 121*1, 85, 121));
    auto sprite3 = Sprite::create(batch->getTexture(), Rect(85*2, 121*1, 85, 121));
    auto sprite4 = Sprite::create(batch->getTexture(), Rect(85*3, 121*1, 85, 121));
    
    auto sprite5 = Sprite::create(batch->getTexture(), Rect(85*0, 121*1, 85, 121));
    auto sprite6 = Sprite::create(batch->getTexture(), Rect(85*1, 121*1, 85, 121));
    auto sprite7 = Sprite::create(batch->getTexture(), Rect(85*2, 121*1, 85, 121));
    auto sprite8 = Sprite::create(batch->getTexture(), Rect(85*3, 121*1, 85, 121));
    
    
    auto s = Director::getInstance()->getWinSize();
    sprite1->setPosition( Vec2( (s.width/5)*1, (s.height/3)*1) );
    sprite2->setPosition( Vec2( (s.width/5)*2, (s.height/3)*1) );
    sprite3->setPosition( Vec2( (s.width/5)*3, (s.height/3)*1) );
    sprite4->setPosition( Vec2( (s.width/5)*4, (s.height/3)*1) );
    sprite5->setPosition( Vec2( (s.width/5)*1, (s.height/3)*2) );
    sprite6->setPosition( Vec2( (s.width/5)*2, (s.height/3)*2) );
    sprite7->setPosition( Vec2( (s.width/5)*3, (s.height/3)*2) );
    sprite8->setPosition( Vec2( (s.width/5)*4, (s.height/3)*2) );

    auto action = std::make_unique<FadeIn>(2);
    auto action_back = std::unique_ptr<FadeTo>(action->reverse());
    auto fade = std::make_unique<RepeatForever>( std::make_unique<Sequence>( std::move( action), std::move( action_back) ) );

    auto tintred = std::make_unique<TintBy>(2, 0, -255, -255);
    auto tintred_back = std::unique_ptr<TintBy>(tintred->reverse());
    auto red = std::make_unique<RepeatForever>( std::make_unique<Sequence>( std::move( tintred), std::move( tintred_back) ) );

    auto tintgreen = std::make_unique<TintBy>(2, -255, 0, -255);
    auto tintgreen_back = std::unique_ptr<TintBy>(tintgreen->reverse());
    auto green = std::make_unique<RepeatForever>( std::make_unique<Sequence>( std::move( tintgreen), std::move( tintgreen_back) ) );

    auto tintblue = std::make_unique<TintBy>(2, -255, -255, 0);
    auto tintblue_back = std::unique_ptr<TintBy>(tintblue->reverse());
    auto blue = std::make_unique<RepeatForever>( std::make_unique<Sequence>( std::move( tintblue), std::move( tintblue_back) ) );
    
    
    sprite5->runAction( std::move( red));
    sprite6->runAction( std::move( green));
    sprite7->runAction( std::move( blue));
    sprite8->runAction( std::move( fade));
    
    // late add: test dirtyColor and dirtyPosition
    batch->addChild(sprite1, 0, kTagSprite1);
    batch->addChild(sprite2, 0, kTagSprite2);
    batch->addChild(sprite3, 0, kTagSprite3);
    batch->addChild(sprite4, 0, kTagSprite4);
    batch->addChild(sprite5, 0, kTagSprite5);
    batch->addChild(sprite6, 0, kTagSprite6);
    batch->addChild(sprite7, 0, kTagSprite7);
    batch->addChild(sprite8, 0, kTagSprite8);
    
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteBatchNodeColorOpacity::removeAndAddSprite, 0)
            .delay(2.0f)
            .interval(2.0f)
            .paused(isPaused())
    );
}

// this function test if remove and add works as expected:
//   color array and vertex array should be reindexed
void SpriteBatchNodeColorOpacity::removeAndAddSprite(float)
{
    auto batch= static_cast<SpriteBatchNode*>( getChildByTag(kTagSpriteBatchNode) );
    auto sprite = static_cast<Sprite*>( batch->getChildByTag(kTagSprite5) );
    
    sprite->retain();

    batch->removeChild(sprite, false);
    batch->addChild(sprite, 0, kTagSprite5);
    
    sprite->release();
}

std::string SpriteBatchNodeColorOpacity::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeColorOpacity::subtitle() const
{
    return "Color & Opacity";
}


//------------------------------------------------------------------
//
// SpriteZOrder
//
//------------------------------------------------------------------

SpriteZOrder::SpriteZOrder()
{
    _dir = 1;
            
    auto s = Director::getInstance()->getWinSize();
    
    float step = s.width/11;
    for(int i=0;i<5;i++) 
    {
        auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*0, 121*1, 85, 121));
        sprite->setPosition( Vec2( (i+1)*step, s.height/2) );
        addChild(sprite, i);
    }
    
    for(int i=5;i<10;i++) 
    {
        auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*0, 85, 121));
        sprite->setPosition( Vec2( (i+1)*step, s.height/2) );
        addChild(sprite, 14-i);
    }
    
    auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*3, 121*0, 85, 121));
    addChild(sprite, -1, kTagSprite1);
    sprite->setPosition( Vec2(s.width/2, s.height/2 - 20) );
    sprite->setScaleX( 6 );
    sprite->setColor(Color3B::RED);
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteZOrder::reorderSprite, 0)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );
}

void SpriteZOrder::reorderSprite(float)
{
    auto sprite = static_cast<Sprite*>( getChildByTag(kTagSprite1) );
    
    int z = sprite->getLocalZOrder();
    
    if( z < -1 )
        _dir = 1;
    if( z > 10 )
        _dir = -1;
    
    z += _dir * 3;
    
    reorderChild(sprite, z);
    
}

std::string SpriteZOrder::title() const
{
    return "Testing Sprite";
}

std::string SpriteZOrder::subtitle() const
{
    return "Z order";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeZOrder
//
//------------------------------------------------------------------

SpriteBatchNodeZOrder::SpriteBatchNodeZOrder()
{
    _dir = 1;
    
    // small capacity. Testing resizing.
    // Don't use capacity=1 in your real game. It is expensive to resize the capacity
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 1);
    addChild(batch, 0, kTagSpriteBatchNode);        
    
    auto s = Director::getInstance()->getWinSize();

    float step = s.width/11;
    for(int i=0;i<5;i++) 
    {
        auto sprite = Sprite::create(batch->getTexture(), Rect(85*0, 121*1, 85, 121));
        sprite->setPosition( Vec2( (i+1)*step, s.height/2) );
        batch->addChild(sprite, i);
    }
    
    for(int i=5;i<10;i++) 
    {
        auto sprite = Sprite::create(batch->getTexture(), Rect(85*1, 121*0, 85, 121));
        sprite->setPosition( Vec2( (i+1)*step, s.height/2) );
        batch->addChild(sprite, 14-i);
    }
    
    auto sprite = Sprite::create(batch->getTexture(), Rect(85*3, 121*0, 85, 121));
    batch->addChild(sprite, -1, kTagSprite1);
    sprite->setPosition( Vec2(s.width/2, s.height/2 - 20) );
    sprite->setScaleX( 6 );
    sprite->setColor(Color3B::RED);
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteBatchNodeZOrder::reorderSprite, 0)
            .delay(2.0f)
            .interval(2.0f)
            .paused(isPaused())
    );
}

void SpriteBatchNodeZOrder::reorderSprite(float)
{
    auto batch= static_cast<SpriteBatchNode*>( getChildByTag( kTagSpriteBatchNode ));
    auto sprite = static_cast<Sprite*>(batch->getChildByTag(kTagSprite1));
    
    int z = sprite->getLocalZOrder();
    
    if( z < -1 )
        _dir = 1;
    if( z > 10 )
        _dir = -1;
    
    z += _dir * 3;

    batch->reorderChild(sprite, z);
}

std::string SpriteBatchNodeZOrder::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeZOrder::subtitle() const
{
    return "Z order";
}


//------------------------------------------------------------------
//
// SpriteBatchNodeReorder
//
//------------------------------------------------------------------

SpriteBatchNodeReorder::SpriteBatchNodeReorder()
{
    std::vector<Node*> v{10, nullptr};

    auto asmtest = SpriteBatchNode::create("animations/ghosts.png");
    
    for (auto & p : v)
    {
        auto s1 = Sprite::create(asmtest->getTexture(), Rect(0, 0, 50, 50));
        p = s1;
        asmtest->addChild(s1, 10);
    }
    
    for (auto p : v)
    {
        if (p != v[5])
        {
            asmtest->reorderChild(p, 9);
        }
    }
    
    ssize_t CC_UNUSED prev = -1;
    
    auto& children = asmtest->getChildren();

    for(const auto &obj : children)
    {
        auto child = static_cast<Sprite*>(obj.get());

        ssize_t currentIndex = child->getAtlasIndex();
        CCASSERT( prev == currentIndex-1, "Child order failed");
        ////----CCLOG("children %x - atlasIndex:%d", child, currentIndex);
        prev = currentIndex;
    }
    
    prev = -1;
    auto& descendants = asmtest->getDescendants();
    for(const auto &sprite : descendants) {
        ssize_t currentIndex = sprite->getAtlasIndex();
        CCASSERT( prev == currentIndex-1, "Child order failed");
        ////----CCLOG("descendant %x - atlasIndex:%d", child, currentIndex);
        prev = currentIndex;
    }
}

std::string SpriteBatchNodeReorder::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeReorder::subtitle() const
{
    return "reorder #1. Should not crash";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeReorderIssue744
//
//------------------------------------------------------------------

SpriteBatchNodeReorderIssue744::SpriteBatchNodeReorderIssue744()
{
    auto s = Director::getInstance()->getWinSize();
    

    // Testing issue #744
    // http://code.google.com/p/cocos2d-iphone/issues/detail?id=744
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 15);
    addChild(batch, 0, kTagSpriteBatchNode);        

    auto sprite = Sprite::create(batch->getTexture(),Rect(0, 0, 85, 121));
    sprite->setPosition( Vec2(s.width/2, s.height/2) );
    batch->addChild(sprite, 3);
    batch->reorderChild(sprite, 1);
}

std::string SpriteBatchNodeReorderIssue744::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeReorderIssue744::subtitle() const
{
    return "reorder issue #744. Should not crash";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeReorderIssue766
//
//------------------------------------------------------------------

Sprite* SpriteBatchNodeReorderIssue766::makeSpriteZ(int aZ)
{
    Rect rcw(128,0,64,64);
    rcw = CC_RECT_PIXELS_TO_POINTS(rcw);
    auto sprite = Sprite::create(batchNode->getTexture(), rcw);
    sprite->setScale(CC_CONTENT_SCALE_FACTOR());
    batchNode->addChild(sprite, aZ+1, 0);

    //children
    Rect rc1(0,0,64,64);
    rc1 = CC_RECT_PIXELS_TO_POINTS(rc1);
    auto spriteShadow = Sprite::create(batchNode->getTexture(), rc1);
    spriteShadow->setOpacity(128);
    sprite->setScale(CC_CONTENT_SCALE_FACTOR());
    sprite->addChild(spriteShadow, aZ, 3);

    Rect rc2(64,0,64,64);
    rc2 = CC_RECT_PIXELS_TO_POINTS(rc2);
    auto spriteTop = Sprite::create(batchNode->getTexture(), rc2);
    sprite->setScale(CC_CONTENT_SCALE_FACTOR());
    sprite->addChild(spriteTop, aZ+2, 3);

    return sprite;
}

void SpriteBatchNodeReorderIssue766::reorderSprite(float)
{
    Director::getInstance()->getScheduler().unscheduleTimedJob(this, 0);

    batchNode->reorderChild(sprite1, 4);
}

// on "init" you need to initialize your instance
SpriteBatchNodeReorderIssue766::SpriteBatchNodeReorderIssue766()
{
    batchNode = SpriteBatchNode::create("Images/piece.png", 15);
    addChild(batchNode, 1, 0);

    sprite1 = makeSpriteZ(2);
    sprite1->setPosition(Vec2(200,160));

    sprite2 = makeSpriteZ(3);
    sprite2->setPosition(Vec2(264,160));

    sprite3 = makeSpriteZ(4);
    sprite3->setPosition(Vec2(328,160));

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteBatchNodeReorderIssue766::reorderSprite, 0)
            .delay(2.0f)
            .interval(2.0f)
            .paused(isPaused())
    );
}

std::string SpriteBatchNodeReorderIssue766::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeReorderIssue766::subtitle() const
{
    return "reorder issue #766. In 2 seconds 1 sprite will be reordered";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeReorderIssue767
//
//------------------------------------------------------------------
SpriteBatchNodeReorderIssue767::SpriteBatchNodeReorderIssue767()
{
    auto s = Director::getInstance()->getWinSize();        

    auto & cache = _director->getSpriteFrameCache();

    cache.addSpriteFramesWithFile("animations/ghosts.plist", "animations/ghosts.png");
    Node *aParent;
    Sprite *l1, *l2a, *l2b, *l3a1, *l3a2, *l3b1, *l3b2;

    //
    // SpriteBatchNode: 3 levels of children
    //
    aParent = SpriteBatchNode::create("animations/ghosts.png");
    addChild(aParent, 0, kTagSprite1);

    // parent
    l1 = Sprite::create(cache.getSpriteFrameByName("father.gif"));
    l1->setPosition(Vec2( s.width/2, s.height/2));
    aParent->addChild(l1, 0, kTagSprite2);
    auto l1Size = l1->getContentSize();

    // child left
    l2a = Sprite::create(cache.getSpriteFrameByName("sister1.gif"));
    l2a->setPosition(Vec2( -25 + l1Size.width/2, 0 + l1Size.height/2));
    l1->addChild(l2a, -1, kTagSpriteLeft);
    auto l2aSize = l2a->getContentSize();        


    // child right
    l2b = Sprite::create(cache.getSpriteFrameByName("sister2.gif"));
    l2b->setPosition(Vec2( +25 + l1Size.width/2, 0 + l1Size.height/2));
    l1->addChild(l2b, 1, kTagSpriteRight);
    auto l2bSize = l2a->getContentSize();


    // child left bottom
    l3a1 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3a1->setScale(0.65f);
    l3a1->setPosition(Vec2(0+l2aSize.width/2,-50+l2aSize.height/2));
    l2a->addChild(l3a1, -1);

    // child left top
    l3a2 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3a2->setScale(0.65f);
    l3a2->setPosition(Vec2(0+l2aSize.width/2,+50+l2aSize.height/2));
    l2a->addChild(l3a2, 1);

    // child right bottom
    l3b1 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3b1->setScale(0.65f);
    l3b1->setPosition(Vec2(0+l2bSize.width/2,-50+l2bSize.height/2));
    l2b->addChild(l3b1, -1);

    // child right top
    l3b2 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3b2->setScale(0.65f);
    l3b2->setPosition(Vec2(0+l2bSize.width/2,+50+l2bSize.height/2));
    l2b->addChild(l3b2, 1);

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteBatchNodeReorderIssue767::reorderSprites, 0)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );
}

std::string SpriteBatchNodeReorderIssue767::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeReorderIssue767::subtitle() const
{
    return "reorder issue #767. Should not crash";
}

void SpriteBatchNodeReorderIssue767::reorderSprites(float)
{
    auto spritebatch = static_cast<SpriteBatchNode*>( getChildByTag(kTagSprite1) );
    auto father = static_cast<Sprite*>( spritebatch->getChildByTag(kTagSprite2) );
    auto left = static_cast<Sprite*>( father->getChildByTag(kTagSpriteLeft) );
    auto right = static_cast<Sprite*>( father->getChildByTag(kTagSpriteRight) );

    int newZLeft = 1;

    if( left->getLocalZOrder() == 1 )
        newZLeft = -1;

    father->reorderChild(left, newZLeft);
    father->reorderChild(right, -newZLeft);
}

//------------------------------------------------------------------
//
// SpriteZVertex
//
//------------------------------------------------------------------

void SpriteZVertex::onEnter()
{
    SpriteTestDemo::onEnter();
    
    Director::getInstance()->setProjection(Director::Projection::_3D);
}

void SpriteZVertex::onExit()
{
    Director::getInstance()->setProjection(Director::Projection::DEFAULT);
    SpriteTestDemo::onExit();
}

SpriteZVertex::SpriteZVertex()
{
    //
    // This test tests z-order
    // If you are going to use it is better to use a 3D projection
    //
    // WARNING:
    // The developer is resposible for ordering its sprites according to its Z if the sprite has
    // transparent parts.
    //

    //
    // Configure shader to mimic glAlphaTest
    //
    auto alphaTestShader = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST);
    GLint alphaValueLocation = glGetUniformLocation(alphaTestShader->getProgram(), GLProgram::UNIFORM_NAME_ALPHA_TEST_VALUE);

    // set alpha test value
    // NOTE: alpha test shader is hard-coded to use the equivalent of a glAlphaFunc(GL_GREATER) comparison
    if (getGLProgram())
    {
        getGLProgram()->setUniformLocationWith1f(alphaValueLocation, 0.0f);
    }
    
    
    _dir = 1;
    _time = 0;

    auto s = Director::getInstance()->getWinSize();
    float step = s.width/12;
    
    auto node = Node::create();
    // camera uses the center of the image as the pivoting point
    node->setContentSize( Size(s.width,s.height) );
    node->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
    node->setPosition( Vec2(s.width/2, s.height/2));

    addChild(node, 0);

    for(int i=0;i<5;i++) 
    {
        auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*0, 121*1, 85, 121));
        sprite->setPosition( Vec2((i+1)*step, s.height/2) );
        sprite->setPositionZ( 10 + i*40 );
        sprite->setGLProgram(alphaTestShader);
        node->addChild(sprite, 0);
        
    }
    
    for(int i=5;i<11;i++) 
    {
        auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*0, 85, 121));
        sprite->setPosition( Vec2( (i+1)*step, s.height/2) );
        sprite->setPositionZ( 10 + (10-i)*40 );
        sprite->setGLProgram(alphaTestShader);
        node->addChild(sprite, 0);
    }

    node->runAction( std::make_unique<OrbitCamera>(10, 1, 0, 0, 360, 0, 0) );
}

std::string SpriteZVertex::title() const
{
    return "Testing Sprite";
}

std::string SpriteZVertex::subtitle() const
{
    return "openGL Z vertex";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeZVertex
//
//------------------------------------------------------------------

void SpriteBatchNodeZVertex::onEnter()
{
    SpriteTestDemo::onEnter();

    Director::getInstance()->setProjection(Director::Projection::_3D);
}

void SpriteBatchNodeZVertex::onExit()
{
    Director::getInstance()->setProjection(Director::Projection::DEFAULT);
    SpriteTestDemo::onExit();
}

SpriteBatchNodeZVertex::SpriteBatchNodeZVertex()
{
    //
    // This test tests z-order
    // If you are going to use it is better to use a 3D projection
    //
    // WARNING:
    // The developer is resposible for ordering its sprites according to its Z if the sprite has
    // transparent parts.
    //

    //
    // Configure shader to mimic glAlphaTest
    //
    auto alphaTestShader = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_ALPHA_TEST);
    GLint alphaValueLocation = glGetUniformLocation(alphaTestShader->getProgram(), GLProgram::UNIFORM_NAME_ALPHA_TEST_VALUE);

    // set alpha test value
    // NOTE: alpha test shader is hard-coded to use the equivalent of a glAlphaFunc(GL_GREATER) comparison
    if (getGLProgram())
    {
        getGLProgram()->setUniformLocationWith1f(alphaValueLocation, 0.0f);
    }
    
    auto s = Director::getInstance()->getWinSize();
    float step = s.width/12;
    
    // small capacity. Testing resizing.
    // Don't use capacity=1 in your real game. It is expensive to resize the capacity
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 1);
    // camera uses the center of the image as the pivoting point
    batch->setContentSize( Size(s.width,s.height));
    batch->setAnchorPoint( Vec2::ANCHOR_MIDDLE);
    batch->setPosition( Vec2(s.width/2, s.height/2));
    
    batch->setGLProgram(alphaTestShader);
    addChild(batch, 0, kTagSpriteBatchNode);        
    
    for(int i=0;i<5;i++) 
    {
        auto sprite = Sprite::create(batch->getTexture(), Rect(85*0, 121*1, 85, 121));
        sprite->setPosition( Vec2( (i+1)*step, s.height/2) );
        sprite->setPositionZ(  10 + i*40 );
        batch->addChild(sprite, 0);
        
    }
    
    for(int i=5;i<11;i++) {
        auto sprite = Sprite::create(batch->getTexture(), Rect(85*1, 121*0, 85, 121));
        sprite->setPosition( Vec2( (i+1)*step, s.height/2) );
        sprite->setPositionZ(  10 + (10-i)*40 );
        batch->addChild(sprite, 0);
    }
    
    batch->runAction(std::make_unique<OrbitCamera>(10, 1, 0, 0, 360, 0, 0) );
}

std::string SpriteBatchNodeZVertex::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeZVertex::subtitle() const
{
    return "openGL Z vertex";
}

//------------------------------------------------------------------
//
// SpriteAnchorPoint
//
//------------------------------------------------------------------

SpriteAnchorPoint::SpriteAnchorPoint()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto action = std::make_unique<RepeatForever>( std::make_unique<RotateBy>(10, 360));
    
    for(int i=0;i<3;i++) 
    {
        auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*i, 121*1, 85, 121) );
        sprite->setPosition( Vec2( s.width/4*(i+1), s.height/2) );
        
        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 10);
        
        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
                break;
            case 1:
                sprite->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
                break;
            case 2:
                sprite->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
                break;
        }
        
        point->setPosition( sprite->getPosition() );

        sprite->runAction( std::unique_ptr<RepeatForever>(action->clone()) );
        addChild(sprite, i);
    }        
}

std::string SpriteAnchorPoint::title() const
{
    return "Testing Sprite";
}

std::string SpriteAnchorPoint::subtitle() const
{
    return "anchor point";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeAnchorPoint
//
//------------------------------------------------------------------
SpriteBatchNodeAnchorPoint::SpriteBatchNodeAnchorPoint()
{
    // small capacity. Testing resizing.
    // Don't use capacity=1 in your real game. It is expensive to resize the capacity
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 1);
    addChild(batch, 0, kTagSpriteBatchNode);        
    
    auto s = Director::getInstance()->getWinSize();
    
    auto action = std::make_unique<RepeatForever>( std::make_unique<RotateBy>(10, 360));

    for(int i=0;i<3;i++) 
    {
        auto sprite = Sprite::create(batch->getTexture(), Rect(85*i, 121*1, 85, 121));
        sprite->setPosition( Vec2( s.width/4*(i+1), s.height/2) );
        
        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 1);

        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
                break;
            case 1:
                sprite->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
                break;
            case 2:
                sprite->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
                break;
        }

        point->setPosition( sprite->getPosition() );

        sprite->runAction( std::unique_ptr<RepeatForever>(action->clone()) );
        batch->addChild(sprite, i);
    }
}

std::string SpriteBatchNodeAnchorPoint::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeAnchorPoint::subtitle() const
{
    return "anchor point";
}

//------------------------------------------------------------------
//
// SpriteAnchorPointFromFile
//
//------------------------------------------------------------------

void SpriteAnchorPointFromFile::onEnter()
{
    SpriteTestDemo::onEnter();
    auto screen = Director::getInstance()->getWinSize();
    
    auto action = std::make_unique<RepeatForever>( std::make_unique<RotateBy>(10, 360));
    char str[100] = {0};

    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini_anchors.plist");

    Sprite *sprite;
    for(int i=0;i<10;i++)
    {
        sprintf(str, "grossini_dance_%02d.png", i+1);
        sprite = Sprite::create(cache.getSpriteFrameByName(str));

        sprite->setPosition(Vec2(screen.width/6*(i%5+1), screen.height*2/3 - screen.height*(i/5)/3));
        
        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.1f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 10);
        
        sprite->runAction( std::unique_ptr<RepeatForever>(action->clone()) );
        addChild(sprite, i);
    }
    
    std::vector<retaining_ptr<SpriteFrame>> animFrames;
    animFrames.reserve(5);
    char tmp[50];

    for (int i = 10; i < 15; i++)
    {            
        sprintf(tmp, "grossini_dance_%02d.png", i);
        auto frame = _director->getSpriteFrameCache().getSpriteFrameByName(tmp);
        animFrames.push_back(to_retaining_ptr(frame));
    }

    std::unique_ptr<Animation> animation(new Animation( std::move(animFrames), 0.3f));

    sprite->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Animate>( std::move( animation))
        ));

}

void SpriteAnchorPointFromFile::onExit()
{
    _director->getSpriteFrameCache().removeSpriteFramesFromFile("animations/grossini_anchors.plist");
    SpriteTestDemo::onExit();
}

std::string SpriteAnchorPointFromFile::title() const
{
    return "Testing SpriteFrame";
}

std::string SpriteAnchorPointFromFile::subtitle() const
{
    return "anchor point";
}

//------------------------------------------------------------------
//
// Sprite6
//
//------------------------------------------------------------------

Sprite6::Sprite6()
{
    // small capacity. Testing resizing
    // Don't use capacity=1 in your real game. It is expensive to resize the capacity
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 1);
    addChild(batch, 0, kTagSpriteBatchNode);
    batch->setIgnoreAnchorPointForPosition( true );

    auto s = Director::getInstance()->getWinSize();

    batch->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    batch->setContentSize( Size(s.width, s.height) );
    
    
    // SpriteBatchNode actions
    auto rotate1 = std::make_unique<RotateBy>(5, 360);
    auto rotate2 = std::unique_ptr<RotateBy>(rotate1->clone());
    auto rotate_back = std::unique_ptr<RotateBy>(rotate1->reverse());

    auto action = std::make_unique<RepeatForever>( std::move( rotate1));

    // SpriteBatchNode actions
    auto rotate_seq = std::make_unique<Sequence>( std::move(rotate2), std::move( rotate_back) );
    auto rotate_forever = std::make_unique<RepeatForever>( std::move(rotate_seq) );
    
    auto scale = std::make_unique<ScaleBy>(5, 1.5f);
    auto scale_back = std::unique_ptr<ScaleBy>(scale->reverse());
    auto scale_seq = std::make_unique<Sequence>( std::move( scale), std::move( scale_back) );
    auto scale_forever = std::make_unique<RepeatForever>( std::move( scale_seq));

    float step = s.width/4;

    for(int i=0;i<3;i++) 
    {
        auto sprite = Sprite::create(batch->getTexture(), Rect(85*i, 121*1, 85, 121));
        sprite->setPosition( Vec2( (i+1)*step, s.height/2) );

        sprite->runAction( std::unique_ptr<RepeatForever>(action->clone()) );
        batch->addChild(sprite, i);
    }
    
    batch->runAction( std::move( scale_forever));
    batch->runAction( std::move( rotate_forever));
}

std::string Sprite6::title() const
{
    return "SpriteBatchNode transformation";
}

//------------------------------------------------------------------
//
// SpriteFlip
//
//------------------------------------------------------------------
SpriteFlip::SpriteFlip()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto sprite1 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*1, 85, 121));
    sprite1->setPosition( Vec2( s.width/2 - 100, s.height/2 ) );
    addChild(sprite1, 0, kTagSprite1);
    
    auto sprite2 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*1, 85, 121));
    sprite2->setPosition( Vec2( s.width/2 + 100, s.height/2 ) );
    addChild(sprite2, 0, kTagSprite2);
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteFlip::flipSprites, 0)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );
}

void SpriteFlip::flipSprites(float)
{
    auto sprite1 = static_cast<Sprite*>(getChildByTag(kTagSprite1));
    auto sprite2 = static_cast<Sprite*>(getChildByTag(kTagSprite2));
    
    bool x = sprite1->isFlippedX();
    bool y = sprite2->isFlippedY();
    
    CCLOG("Pre: %g", sprite1->getContentSize().height);
    sprite1->setFlippedX(!x);
    sprite2->setFlippedY(!y);
    CCLOG("Post: %g", sprite1->getContentSize().height);
}

std::string SpriteFlip::title() const
{
    return "Testing Sprite";
}

std::string SpriteFlip::subtitle() const
{
    return "Flip X & Y";
}


//------------------------------------------------------------------
//
// SpriteBatchNodeFlip
//
//------------------------------------------------------------------
SpriteBatchNodeFlip::SpriteBatchNodeFlip()
{
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 10);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    auto s = Director::getInstance()->getWinSize();
    
    auto sprite1 = Sprite::create(batch->getTexture(), Rect(85*1, 121*1, 85, 121));
    sprite1->setPosition( Vec2( s.width/2 - 100, s.height/2 ) );
    batch->addChild(sprite1, 0, kTagSprite1);
    
    auto sprite2 = Sprite::create(batch->getTexture(), Rect(85*1, 121*1, 85, 121));
    sprite2->setPosition( Vec2( s.width/2 + 100, s.height/2 ) );
    batch->addChild(sprite2, 0, kTagSprite2);
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteBatchNodeFlip::flipSprites, 0)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );
}

void SpriteBatchNodeFlip::flipSprites(float)
{
    auto batch= static_cast<SpriteBatchNode*>(getChildByTag( kTagSpriteBatchNode ));
    auto sprite1 = static_cast<Sprite*>(batch->getChildByTag(kTagSprite1));
    auto sprite2 = static_cast<Sprite*>(batch->getChildByTag(kTagSprite2));
    
    bool x = sprite1->isFlippedX();
    bool y = sprite2->isFlippedY();
    
    CCLOG("Pre: %g", sprite1->getContentSize().height);
    sprite1->setFlippedX(!x);
    sprite2->setFlippedY(!y);
    CCLOG("Post: %g", sprite1->getContentSize().height);
}

std::string SpriteBatchNodeFlip::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeFlip::subtitle() const
{
    return "Flip X & Y";
}


//------------------------------------------------------------------
//
// SpriteAliased
//
//------------------------------------------------------------------

SpriteAliased::SpriteAliased()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto sprite1 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*1, 85, 121));
    sprite1->setPosition( Vec2( s.width/2 - 100, s.height/2 ) );
    addChild(sprite1, 0, kTagSprite1);
    
    auto sprite2 = Sprite::create("Images/grossini_dance_atlas.png", Rect(85*1, 121*1, 85, 121));
    sprite2->setPosition( Vec2( s.width/2 + 100, s.height/2 ) );
    addChild(sprite2, 0, kTagSprite2);
    
    auto scale = std::make_unique<ScaleBy>(2, 5);
    auto scale_back = std::unique_ptr<ScaleBy>(scale->reverse());
    auto seq = std::make_unique<Sequence>( std::move( scale), std::move( scale_back) );
    auto repeat = std::make_unique<RepeatForever>( std::move( seq));
    
    auto repeat2 = std::unique_ptr<RepeatForever>(repeat->clone());
    
    sprite1->runAction( std::move( repeat));
    sprite2->runAction( std::move( repeat2));
    
}

void SpriteAliased::onEnter()
{
    SpriteTestDemo::onEnter();
    
    //
    // IMPORTANT:
    // This change will affect every sprite that uses the same texture
    // So sprite1 and sprite2 will be affected by this change
    //
    auto sprite = static_cast<Sprite*>( getChildByTag(kTagSprite1) );
    sprite->getTexture()->setAliasTexParameters();
}

void SpriteAliased::onExit()
{
    // restore the tex parameter to AntiAliased.
    auto sprite = static_cast<Sprite*>( getChildByTag(kTagSprite1) );
    sprite->getTexture()->setAntiAliasTexParameters();
    SpriteTestDemo::onExit();
}

std::string SpriteAliased::title() const
{
    return "Testing Sprite";
}

std::string SpriteAliased::subtitle() const
{
    return "AliasTexParameters()";
}


//------------------------------------------------------------------
//
// SpriteBatchNodeAliased
//
//------------------------------------------------------------------

SpriteBatchNodeAliased::SpriteBatchNodeAliased()
{
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 10);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    auto s = Director::getInstance()->getWinSize();

    auto sprite1 = Sprite::create(batch->getTexture(), Rect(85*1, 121*1, 85, 121));
    sprite1->setPosition( Vec2( s.width/2 - 100, s.height/2 ) );
    batch->addChild(sprite1, 0, kTagSprite1);
    
    auto sprite2 = Sprite::create(batch->getTexture(), Rect(85*1, 121*1, 85, 121));
    sprite2->setPosition( Vec2( s.width/2 + 100, s.height/2 ) );
    batch->addChild(sprite2, 0, kTagSprite2);
    
    auto scale = std::make_unique<ScaleBy>(2, 5);
    auto scale_back = std::unique_ptr<ScaleBy>(scale->reverse());
    auto seq = std::make_unique<Sequence>( std::move( scale), std::move( scale_back) );
    auto repeat = std::make_unique<RepeatForever>( std::move( seq));
    
    auto repeat2 = std::unique_ptr<RepeatForever>(repeat->clone());
    
    sprite1->runAction( std::move( repeat));
    sprite2->runAction( std::move( repeat2));
        
}
void SpriteBatchNodeAliased::onEnter()
{
    SpriteTestDemo::onEnter();
    auto batch = static_cast<SpriteBatchNode*>( getChildByTag(kTagSpriteBatchNode) );
    batch->getTexture()->setAliasTexParameters();
}

void SpriteBatchNodeAliased::onExit()
{
    // restore the tex parameter to AntiAliased.
    auto batch = static_cast<SpriteBatchNode*>( getChildByTag(kTagSpriteBatchNode) );
    batch->getTexture()->setAntiAliasTexParameters();
    SpriteTestDemo::onExit();
}

std::string SpriteBatchNodeAliased::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeAliased::subtitle() const
{
    return "AliasTexParameters()";
}


//------------------------------------------------------------------
//
// SpriteNewTexture
//
//------------------------------------------------------------------

SpriteNewTexture::SpriteNewTexture()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(SpriteNewTexture::onTouchesEnded, this);
    _director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto node = Node::create();
    addChild(node, 0, kTagSpriteBatchNode);

    _texture1 = Director::getInstance()->getTextureCache()->addImage("Images/grossini_dance_atlas.png");
    _texture1->retain();
    _texture2 = Director::getInstance()->getTextureCache()->addImage("Images/grossini_dance_atlas-mono.png");
    _texture2->retain();
    
    _usingTexture1 = true;

    for(int i=0;i<30;i++)
        addNewSprite();
}

SpriteNewTexture::~SpriteNewTexture()
{
    _texture1->release();
    _texture2->release();
}

void SpriteNewTexture::addNewSprite()
{
    auto s = Director::getInstance()->getWinSize();

    auto p = Vec2( CCRANDOM_0_1() * s.width, CCRANDOM_0_1() * s.height);

    int idx = CCRANDOM_0_1() * 1400 / 100;
    int x = (idx%5) * 85;
    int y = (idx/5) * 121;
    
    
    auto node = getChildByTag( kTagSpriteBatchNode );
    auto sprite = Sprite::create(_texture1, Rect(x,y,85,121));
    node->addChild(sprite);
    
    sprite->setPosition( Vec2( p.x, p.y) );
    
    std::unique_ptr<ActionInterval> action;
    float random = CCRANDOM_0_1();
    
    if( random < 0.20 )
        action.reset(new ScaleBy(3, 2));
    else if(random < 0.40)
        action.reset(new RotateBy(3, 360));
    else if( random < 0.60)
        action.reset(new Blink(1, 3));
    else if( random < 0.8 )
        action.reset(new TintBy(2, 0, -255, -255));
    else 
        action.reset(new FadeOut(2));

    auto action_back = std::unique_ptr<ActionInterval>(action->reverse());

    auto seq = std::make_unique<Sequence>(
        std::move(action),
        std::move(action_back)
    );

    sprite->runAction( std::make_unique<RepeatForever>( std::move( seq)));
}

void SpriteNewTexture::onTouchesEnded(const std::vector<Touch*>&, Event*)
{

    auto node = getChildByTag( kTagSpriteBatchNode );

    auto& children = node->getChildren();
    Sprite* sprite;

    if( _usingTexture1 )                          //--> win32 : Let's it make just simple sentence
    {
        for(const auto &obj : children) {
            sprite = static_cast<Sprite*>( obj.get() );
            sprite->setTexture(_texture2);
        }

        _usingTexture1 = false;
    } 
    else 
    {
        for(const auto &obj : children) {
            sprite = static_cast<Sprite*>( obj.get() );
            sprite->setTexture(_texture1);
        }

        _usingTexture1 = true;
    }
}

std::string SpriteNewTexture::title() const
{
    return "Testing Sprite";
}

std::string SpriteNewTexture::subtitle() const
{
    return "setTexture() (tap / touch the screen)";
}


//------------------------------------------------------------------
//
// SpriteBatchNodeNewTexture
//
//------------------------------------------------------------------

SpriteBatchNodeNewTexture::SpriteBatchNodeNewTexture()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(SpriteBatchNodeNewTexture::onTouchesEnded, this);
    _director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto batch = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    _texture1 = batch->getTexture(); _texture1->retain();
    _texture2 = Director::getInstance()->getTextureCache()->addImage("Images/grossini_dance_atlas-mono.png");
    _texture2->retain();
    
    for(int i=0;i<30;i++)
        addNewSprite();
}

SpriteBatchNodeNewTexture::~SpriteBatchNodeNewTexture()
{
    _texture1->release();
    _texture2->release();
}

void SpriteBatchNodeNewTexture::addNewSprite()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto p = Vec2( CCRANDOM_0_1() * s.width, CCRANDOM_0_1() * s.height);
    
    auto batch = static_cast<SpriteBatchNode*>( getChildByTag( kTagSpriteBatchNode ) );
    
    int idx = CCRANDOM_0_1() * 1400 / 100;
    int x = (idx%5) * 85;
    int y = (idx/5) * 121;
    
    
    auto sprite = Sprite::create(batch->getTexture(), Rect(x,y,85,121));
    batch->addChild(sprite);
    
    sprite->setPosition( Vec2( p.x, p.y) );
    
    std::unique_ptr<ActionInterval> action;
    float random = CCRANDOM_0_1();
    
    if( random < 0.20 )
        action.reset(new ScaleBy(3, 2));
    else if(random < 0.40)
        action.reset(new RotateBy(3, 360));
    else if( random < 0.60)
        action.reset(new Blink(1, 3));
    else if( random < 0.8 )
        action.reset(new TintBy(2, 0, -255, -255));
    else 
        action.reset(new FadeOut(2));

    auto action_back = std::unique_ptr<ActionInterval>(action->reverse());

    sprite->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(action),
                std::move(action_back)
            )));
}

void SpriteBatchNodeNewTexture::onTouchesEnded(const std::vector<Touch*>&, Event*)
{
    auto batch = static_cast<SpriteBatchNode*>( getChildByTag( kTagSpriteBatchNode) );
    
    if( batch->getTexture() == _texture1 )
        batch->setTexture(_texture2);
    else
        batch->setTexture(_texture1);    
}

std::string SpriteBatchNodeNewTexture::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeNewTexture::subtitle() const
{
    return "setTexture() (tap / touch the screen)";
}


//------------------------------------------------------------------
//
// SpriteFrameTest
//
//------------------------------------------------------------------

void SpriteFrameTest::onEnter()
{
    SpriteTestDemo::onEnter();
    auto s = Director::getInstance()->getWinSize();

    // IMPORTANT:
    // The sprite frames will be cached AND RETAINED, and they won't be released unless you call
    //     _director->getSpriteFrameCache().removeUnusedSpriteFrames);
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    cache.addSpriteFramesWithFile("animations/grossini_blue.plist", "animations/grossini_blue.png");

    //
    // Animation using Sprite BatchNode
    //
    _sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
    _sprite1->setPosition( Vec2( s.width/2-80, s.height/2) );

    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    spritebatch->addChild(_sprite1);
    addChild(spritebatch);

    run_grossini_dance_animation_forever(_sprite1);

    // to test issue #732, uncomment the following line
    _sprite1->setFlippedX(false);
    _sprite1->setFlippedY(false);

    //
    // Animation using standard Sprite
    //
    _sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
    _sprite2->setPosition( Vec2( s.width/2 + 80, s.height/2) );
    addChild(_sprite2);

    std::vector<retaining_ptr<SpriteFrame>> moreFrames;
    moreFrames.reserve(20);

    char str[100];
    for(int i = 1; i < 15; i++) 
    {
        sprintf(str, "grossini_dance_gray_%02d.png",i);
        auto frame = cache.getSpriteFrameByName(str);
        moreFrames.push_back(to_retaining_ptr(frame));
    }


    for( int i = 1; i < 5; i++) {
        sprintf(str, "grossini_blue_%02d.png",i);
        auto frame = cache.getSpriteFrameByName(str);
        moreFrames.push_back(to_retaining_ptr(frame));
    }

    // append frames from another batch
    for(int i = 1; i < 15; i++) 
    {
        sprintf(str, "grossini_dance_%02d.png", i);
        auto frame = cache.getSpriteFrameByName( str );
        moreFrames.push_back(to_retaining_ptr(frame));
    }

    std::unique_ptr<Animation> animMixed(new Animation(moreFrames, 0.3f));
    _sprite2->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Animate>( std::move(animMixed))
        ));

    // to test issue #732, uncomment the following line
    _sprite2->setFlippedX(false);
    _sprite2->setFlippedY(false);

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteFrameTest::startIn05Secs, 0)
            .delay(0.5f)
            .interval(0.5f)
            .paused(isPaused())
    );
    _counter = 0;
}

void SpriteFrameTest::onExit()
{
    SpriteTestDemo::onExit();
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_blue.plist");
}

std::string SpriteFrameTest::title() const
{
    return "Sprite vs. SpriteBatchNode";
}

std::string SpriteFrameTest::subtitle() const
{
    return "Animation. Testing issue #792";
}

void SpriteFrameTest::startIn05Secs(float)
{
    Director::getInstance()->getScheduler().unscheduleTimedJob(this, 0);
    Director::getInstance()->getScheduler().schedule(
            TimedJob(this, &SpriteFrameTest::flipSprites, 1)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );

}

void SpriteFrameTest::flipSprites(float)
{
    _counter++;

    bool fx = false;
    bool fy = false;
    int  i  = _counter % 4;

    switch ( i ) {
        case 0:
            fx = false;
            fy = false;
            break;
        case 1:
            fx = true;
            fy = false;
            break;
        case 2:
            fx = false;
            fy = true;
            break;
        case 3:
            fx = true;
            fy = true;
            break;
    }

    _sprite1->setFlippedX(fx);
    _sprite1->setFlippedY(fy);
    _sprite2->setFlippedX(fx);
    _sprite2->setFlippedY(fy);
    //NSLog(@"flipX:%d, flipY:%d", fx, fy);
}

//------------------------------------------------------------------
//
// SpriteFrameAliasNameTest
//
//------------------------------------------------------------------
void SpriteFrameAliasNameTest::onEnter()
{
    SpriteTestDemo::onEnter();
    auto s = Director::getInstance()->getWinSize();

    // IMPORTANT:
    // The sprite frames will be cached AND RETAINED, and they won't be released unless you call
    //     _director->getSpriteFrameCache().removeUnusedSpriteFrames();
    //
    // SpriteFrameCache is a cache of SpriteFrames
    // SpriteFrames each contain a texture id and a rect (frame).

    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini-aliases.plist", "animations/grossini-aliases.png");

    //
    // Animation using Sprite batch
    //
    // A SpriteBatchNode can reference one and only one texture (one .png file)
    // Sprites that are contained in that texture can be instantiatied as Sprites and then added to the SpriteBatchNode
    // All Sprites added to a SpriteBatchNode are drawn in one OpenGL ES draw call
    // If the Sprites are not added to a SpriteBatchNode then an OpenGL ES draw call will be needed for each one, which is less efficient
    //
    // When you animate a sprite, Animation changes the frame of the sprite using setSpriteFrame: (this is why the animation must be in the same texture)
    // When setSpriteFrame: is used in the Animation it changes the frame to one specified by the SpriteFrames that were added to the animation,
    // but texture id is still the same and so the sprite is still a child of the SpriteBatchNode, 
    // and therefore all the animation sprites are also drawn as part of the SpriteBatchNode
    //

    auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
    sprite->setPosition(Vec2(s.width * 0.5f, s.height * 0.5f));

    auto spriteBatch = SpriteBatchNode::create("animations/grossini-aliases.png");
    spriteBatch->addChild(sprite);
    addChild(spriteBatch);

    std::vector<retaining_ptr<SpriteFrame>> animFrames;
    animFrames.reserve(15);
    char str[100] = {0};

    for(int i = 1; i < 15; i++)
    {
        // Obtain frames by alias name
        sprintf(str, "dance_%02d", i);
        auto frame = cache.getSpriteFrameByName(str);
        animFrames.push_back(to_retaining_ptr(frame));
    }

    std::unique_ptr<Animation> animation(new Animation( std::move(animFrames), 0.3f));
    sprite->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Animate>( std::move(animation))
        ));

    cache.reloadTexture("animations/grossini-aliases.plist");
}

void SpriteFrameAliasNameTest::onExit()
{
    SpriteTestDemo::onExit();
    _director->getSpriteFrameCache().removeSpriteFramesFromFile("animations/grossini-aliases.plist");
}

std::string SpriteFrameAliasNameTest::title() const
{
    return "SpriteFrame Alias Name";
}

std::string SpriteFrameAliasNameTest::subtitle() const
{
    return "SpriteFrames are obtained using the alias name";
}

//------------------------------------------------------------------
//
// SpriteFramesFromFileContent
//
//------------------------------------------------------------------
void SpriteFramesFromFileContent::onEnter()
{
	SpriteTestDemo::onEnter();
	auto s = Director::getInstance()->getWinSize();

	std::string plist_content = FileUtils::getInstance()->getStringFromFile(sheetName() + ".plist");
	Data image_content = FileUtils::getInstance()->getDataFromFile(sheetName() + ".png");

    Image* image = new (std::nothrow) Image();
	image->initWithImageData((const uint8_t*)image_content.getBytes(), image_content.getSize());
	Texture2D* texture = new (std::nothrow) Texture2D();
	texture->initWithImage(image);
	texture->autorelease();
    
    CC_SAFE_RELEASE(image);

	auto & cache = _director->getSpriteFrameCache();
	cache.addSpriteFramesWithFileContent(plist_content, texture);

	//
	// Animation using Sprite BatchNode
	//
	Sprite * sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
	sprite->setPosition( Vec2( s.width/2-80, s.height/2) );
	addChild(sprite);

	std::vector<retaining_ptr<SpriteFrame>> animFrames;
    animFrames.reserve(15);
	char str[100] = {0};

	for(int i = 1; i < 15; i++) 
	{
		sprintf(str, "grossini_dance_%02d.png", i);
		auto frame = cache.getSpriteFrameByName( str );
		animFrames.push_back(to_retaining_ptr(frame));
	}

    std::unique_ptr<Animation> animation(new Animation( std::move(animFrames), 0.3f));
    sprite->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Animate>( std::move(animation))
        ));
}

void SpriteFramesFromFileContent::onExit()
{
	SpriteTestDemo::onExit();

	std::string plist_content = FileUtils::getInstance()->getStringFromFile("animations/grossini.plist");

	_director->getSpriteFrameCache().removeSpriteFramesFromFileContent(plist_content);
}

std::string SpriteFramesFromFileContent::title() const
{
	return "SpriteFrameCache load form file content";
}

std::string SpriteFramesFromFileContent::subtitle() const
{
	return "SpriteFrameCache load from plist file content";
}

std::string SpriteFramesFromFileContent::sheetName() const
{
    return "animations/grossini";
}

//------------------------------------------------------------------
//
// SpritePolygonFromFileContent
//
//------------------------------------------------------------------

std::string SpritePolygonFromFileContent::subtitle() const
{
    return "SpriteFrameCache load polygon info from plist file";
}

std::string SpritePolygonFromFileContent::sheetName() const
{
    return "animations/grossini_polygon";
}

//------------------------------------------------------------------
//
// SpriteOffsetAnchorRotation
//
//------------------------------------------------------------------
void SpriteOffsetAnchorRotation::onEnter()
{
    SpriteTestDemo::onEnter();

    auto s = Director::getInstance()->getWinSize();        
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    for(int i=0;i<3;i++) 
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2( s.width/4*(i+1), s.height/2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 1);

        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
                break;
            case 1:
                sprite->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
                break;
            case 2:
                sprite->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
                break;
        }
        
        point->setPosition( sprite->getPosition() );
        
        run_grossini_dance_animation_forever(sprite);

        sprite->runAction( std::make_unique<RepeatForever>( std::make_unique<RotateBy>(10, 360) ) );

        addChild(sprite, 0);
    }        
}


void SpriteOffsetAnchorRotation::onExit()
{
    SpriteTestDemo::onExit();
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteOffsetAnchorRotation::title() const
{
    return "Testing Sprite";
}

std::string SpriteOffsetAnchorRotation::subtitle() const
{
    return "offset + anchor + rotation";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeOffsetAnchorRotation
//
//------------------------------------------------------------------

SpriteBatchNodeOffsetAnchorRotation::SpriteBatchNodeOffsetAnchorRotation()
{
    auto s = Director::getInstance()->getWinSize(); 
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);
    
    for(int i=0;i<3;i++) 
    {
        //
        // Animation using Sprite BatchNode
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition( Vec2( s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 200);
        
        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
                break;
            case 1:
                sprite->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
                break;
            case 2:
                sprite->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
                break;
        }
        
        point->setPosition( sprite->getPosition() );
        
        std::vector<retaining_ptr<SpriteFrame>> animFrames;
        animFrames.reserve(14);
        char str[100] = {0};

        for(int k = 0; k < 14; k++) 
        {
            sprintf(str, "grossini_dance_%02d.png",(k+1));
            auto frame = cache.getSpriteFrameByName(str);
            animFrames.push_back(to_retaining_ptr(frame));
        }

        std::unique_ptr<Animation> animation(new Animation( std::move(animFrames), 0.3f));
        sprite->runAction(
            std::make_unique<RepeatForever>(
                std::make_unique<Animate>( std::move(animation))
            ));

        sprite->runAction(
            std::make_unique<RepeatForever>(
                std::make_unique<RotateBy>(10, 360)
            ));
        
        spritebatch->addChild(sprite, i);
    }        
}


void SpriteBatchNodeOffsetAnchorRotation::onExit()
{
    SpriteTestDemo::onExit();
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteBatchNodeOffsetAnchorRotation::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeOffsetAnchorRotation::subtitle() const
{
    return "offset + anchor + rotation";
}

//------------------------------------------------------------------
//
// SpriteOffsetAnchorScale
//
//------------------------------------------------------------------


SpriteOffsetAnchorScale::SpriteOffsetAnchorScale()
{
    auto s = Director::getInstance()->getWinSize();   
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    for(int i=0;i<3;i++) 
    {
        //
        // Animation using Sprite BatchNode
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition( Vec2( s.width/4*(i+1), s.height/2) );
        
        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 1);
        
        switch(i) 
        {
            case 0:
                sprite->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
                break;
            case 1:
                sprite->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
                break;
            case 2:
                sprite->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
                break;
        }
        
        point->setPosition( sprite->getPosition() );
        
        std::vector<retaining_ptr<SpriteFrame>> animFrames;
        animFrames.reserve(14);
        char str[100] = {0};

        for(int i = 0; i < 14; i++) 
        {
            sprintf(str, "grossini_dance_%02d.png",(i+1));
            auto frame = cache.getSpriteFrameByName(str);
            animFrames.push_back(to_retaining_ptr(frame));
        }

        std::unique_ptr<Animation> animation(new Animation( std::move(animFrames), 0.3f));
        sprite->runAction(
            std::make_unique<RepeatForever>(
                std::make_unique<Animate>( std::move(animation))
            ));
        
        auto scale = std::make_unique<ScaleBy>(2, 2);
        auto scale_back = std::unique_ptr<ScaleBy>(scale->reverse());
        auto seq_scale = std::make_unique<Sequence>( std::move(scale), std::move(scale_back) );
        sprite->runAction( std::make_unique<RepeatForever>( std::move(seq_scale) ));
        
        addChild(sprite, 0);
    }        
}

void SpriteOffsetAnchorScale::onExit()
{
    SpriteTestDemo::onExit();
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteOffsetAnchorScale::title() const
{
    return "Testing Sprite";
}

std::string SpriteOffsetAnchorScale::subtitle() const
{
    return "offset + anchor + scale";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeOffsetAnchorScale
//
//------------------------------------------------------------------
SpriteBatchNodeOffsetAnchorScale::SpriteBatchNodeOffsetAnchorScale()
{
    auto s = Director::getInstance()->getWinSize(); 
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritesheet = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritesheet);
    
    for(int i=0;i<3;i++) 
    {
        //
        // Animation using Sprite BatchNode
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition( Vec2( s.width/4*(i+1), s.height/2) );
        
        auto point = Sprite::create("Images/r1.png");
        point->setScale( 0.25f );
        point->setPosition( sprite->getPosition() );
        addChild(point, 200);
        
        switch(i) {
            case 0:
                sprite->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
                break;
            case 1:
                sprite->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
                break;
            case 2:
                sprite->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
                break;
        }
        
        point->setPosition( sprite->getPosition() );
        
        std::vector<retaining_ptr<SpriteFrame>> animFrames;
        animFrames.reserve(14);
        char str[100] = {0};

        for(int k = 0; k < 14; k++) 
        {
            sprintf(str, "grossini_dance_%02d.png",(k+1));
            auto frame = cache.getSpriteFrameByName(str);
            animFrames.push_back(to_retaining_ptr(frame));
        }

        std::unique_ptr<Animation> animation(new Animation( std::move(animFrames), 0.3f));
        sprite->runAction(
            std::make_unique<RepeatForever>(
                std::make_unique<Animate>( std::move(animation))
            ));

        auto scale = std::make_unique<ScaleBy>(2, 2);
        auto scale_back = std::unique_ptr<ScaleBy>(scale->reverse());
        auto seq_scale = std::make_unique<Sequence>( std::move(scale), std::move( scale_back) );
        sprite->runAction( std::make_unique<RepeatForever>( std::move(seq_scale)));
        
        spritesheet->addChild(sprite, i);
    }        
}

void SpriteBatchNodeOffsetAnchorScale::onExit()
{
    SpriteTestDemo::onExit();
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteBatchNodeOffsetAnchorScale::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeOffsetAnchorScale::subtitle() const
{
    return "offset + anchor + scale";
}


//------------------------------------------------------------------
//
// SpriteAnimationSplit
//
//------------------------------------------------------------------

SpriteAnimationSplit::SpriteAnimationSplit()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto texture = Director::getInstance()->getTextureCache()->addImage("animations/dragon_animation.png");
    
    // manually add frames to the frame cache
    auto frame0 = SpriteFrame::createWithTexture(texture, Rect(132*0, 132*0, 132, 132));
    auto frame1 = SpriteFrame::createWithTexture(texture, Rect(132*1, 132*0, 132, 132));
    auto frame2 = SpriteFrame::createWithTexture(texture, Rect(132*2, 132*0, 132, 132));
    auto frame3 = SpriteFrame::createWithTexture(texture, Rect(132*3, 132*0, 132, 132));
    auto frame4 = SpriteFrame::createWithTexture(texture, Rect(132*0, 132*1, 132, 132));
    auto frame5 = SpriteFrame::createWithTexture(texture, Rect(132*1, 132*1, 132, 132));
    
    
    //
    // Animation using Sprite BatchNode
    //
    auto sprite = Sprite::create(frame0);
    sprite->setPosition( Vec2( s.width/2-80, s.height/2) );
    addChild(sprite);
            
    std::vector<retaining_ptr<SpriteFrame>> animFrames;
    animFrames.reserve(6);

    animFrames.push_back(to_retaining_ptr(frame0));
    animFrames.push_back(to_retaining_ptr(frame1));
    animFrames.push_back(to_retaining_ptr(frame2));
    animFrames.push_back(to_retaining_ptr(frame3));
    animFrames.push_back(to_retaining_ptr(frame4));
    animFrames.push_back(to_retaining_ptr(frame5));
            
    std::unique_ptr<Animation> animation(new Animation( std::move(animFrames), 0.2f));
    auto animate = std::make_unique<Animate>( std::move( animation));
    auto animate_clone = std::unique_ptr<Animate>(animate->clone());
    auto seq = std::make_unique<Sequence>(
        std::move(animate),
        std::make_unique<FlipX>(true),
        std::move(animate_clone),
        std::make_unique<FlipX>(false)
    );
    
    sprite->runAction(std::make_unique<RepeatForever>( std::move(seq) ) );
}

void SpriteAnimationSplit::onExit()
{
    SpriteTestDemo::onExit();
    _director->getSpriteFrameCache().removeUnusedSpriteFrames();
}

std::string SpriteAnimationSplit::title() const
{
    return "Sprite: Animation + flip";
}

//------------------------------------------------------------------
//
// SpriteHybrid
//
//------------------------------------------------------------------
SpriteHybrid::SpriteHybrid()
{
    auto s = Director::getInstance()->getWinSize();

    // parents
    auto parent1 = Node::create();
    auto parent2 = SpriteBatchNode::create("animations/grossini.png", 50);
    
    addChild(parent1, 0, kTagNode);
    addChild(parent2, 0, kTagSpriteBatchNode);
    
    
    // IMPORTANT:
    // The sprite frames will be cached AND RETAINED, and they won't be released unless you call
    //     _director->getSpriteFrameCache().removeUnusedSpriteFrames);
    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini.plist");
    
    
    // create 250 sprites
    // only show 80% of them
    for(int i = 0; i < 250; i++) 
    {
        int spriteIdx = CCRANDOM_0_1() * 14;
        char str[25] = {0};
        sprintf(str, "grossini_dance_%02d.png", (spriteIdx+1));
        auto frame = _director->getSpriteFrameCache().getSpriteFrameByName(str);
        auto sprite = Sprite::create(frame);
        parent1->addChild(sprite, i, i);
        
        float x=-1000;
        float y=-1000;
        if( CCRANDOM_0_1() < 0.2f ) 
        {
            x = CCRANDOM_0_1() * s.width;
            y = CCRANDOM_0_1() * s.height;
        }
        sprite->setPosition( Vec2(x,y) );
            
        auto action = std::make_unique<RotateBy>(4, 360);
        sprite->runAction( std::make_unique<RepeatForever>( std::move(action)));
    }
    
    _usingSpriteBatchNode = false;
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteHybrid::reparentSprite, 0)
            .delay(2.0f)
            .interval(2.0f)
            .paused(isPaused())
    );
}

void SpriteHybrid::reparentSprite(float)
{
    auto p1 = getChildByTag(kTagNode);
    auto p2 = getChildByTag( kTagSpriteBatchNode );
    
    if( _usingSpriteBatchNode )
        std::swap(p1, p2);

    auto & p1Children = p1->getChildren();

    std::vector<node_ptr<Node>> retArray;
    retArray.reserve(p1Children.size());

    for (auto & node : p1Children)
    {
        // FIXME
        retArray.push_back(to_node_ptr(node.get()));
    }

    int i=0;
    p1->removeAllChildrenWithCleanup(false);

    for (const auto & node : retArray)
    {
        p2->addChild(node.get(), i, i);
        i++;
    }

    _usingSpriteBatchNode = ! _usingSpriteBatchNode;
}

void SpriteHybrid::onExit()
{
    SpriteTestDemo::onExit();
    _director->getSpriteFrameCache().removeSpriteFramesFromFile("animations/grossini.plist");
}

std::string SpriteHybrid::title() const
{
    return "HybridSprite* sprite Test";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeChildren
//
//------------------------------------------------------------------

SpriteBatchNodeChildren::SpriteBatchNodeChildren()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();

    // parents
    auto batch = SpriteBatchNode::create("animations/grossini.png", 50);
    
    addChild(batch, 0, kTagSpriteBatchNode);
    
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    
    auto sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
    sprite1->setPosition(Vec2( s.width/3, s.height/2));
    
    auto sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(50,50));
    
    auto sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-50,-50));
    
    batch->addChild(sprite1);
    sprite1->addChild(sprite2);
    sprite1->addChild(sprite3);
    
    // BEGIN NEW CODE
    std::vector<retaining_ptr<SpriteFrame>> animFrames;
    animFrames.reserve(14);
    char str[100] = {0};

    for(int i = 1; i < 15; i++) 
    {
        sprintf(str, "grossini_dance_%02d.png",i);
        auto frame = _director->getSpriteFrameCache().getSpriteFrameByName(str);
        animFrames.push_back(to_retaining_ptr(frame));
    }
    
    std::unique_ptr<Animation> animation(new Animation( std::move(animFrames), 0.2f));
    sprite1->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Animate>( std::move(animation))
        ));
    // END NEW CODE
    
    auto action = std::make_unique<MoveBy>(2, Vec2(200,0));
    auto action_back = std::unique_ptr<MoveBy>(action->reverse());
    auto action_rot = std::make_unique<RotateBy>(2, 360);
    auto action_s = std::make_unique<ScaleBy>(2, 2);
    auto action_s_back = std::unique_ptr<ScaleBy>(action_s->reverse());
    
    auto seq2 = std::unique_ptr<RotateBy>(action_rot->reverse());
    sprite2->runAction( std::make_unique<RepeatForever>( std::move(seq2) ) );
    
    sprite1->runAction( std::make_unique<RepeatForever>( std::move( action_rot)));
    sprite1->runAction( std::make_unique<RepeatForever>(std::make_unique<Sequence>( std::move(action), std::move( action_back) )) );
    sprite1->runAction( std::make_unique<RepeatForever>(std::make_unique<Sequence>( std::move(action_s), std::move( action_s_back) )) );

}

void SpriteBatchNodeChildren::onExit()
{
    SpriteTestDemo::onExit();
    _director->getSpriteFrameCache().removeUnusedSpriteFrames();
}

std::string SpriteBatchNodeChildren::title() const
{
    return "SpriteBatchNode Grand Children";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeChildrenZ
//
//------------------------------------------------------------------
SpriteBatchNodeChildrenZ::SpriteBatchNodeChildrenZ()
{
    auto s = Director::getInstance()->getWinSize();
    
    // parents
    SpriteBatchNode* batch;
    Sprite* sprite1, *sprite2, *sprite3;

    auto & cache = _director->getSpriteFrameCache();
    
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    
    // test 1
    batch = SpriteBatchNode::create("animations/grossini.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
    sprite1->setPosition(Vec2( s.width/3, s.height/2));
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(20,30));
    
    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-20,30));
    
    batch->addChild(sprite1);
    sprite1->addChild(sprite2, 2);
    sprite1->addChild(sprite3, -2);
    
    // test 2
    batch = SpriteBatchNode::create("animations/grossini.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
    sprite1->setPosition(Vec2( 2*s.width/3, s.height/2));
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(20,30));
    
    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-20,30));
    
    batch->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);
    
    // test 3
    batch = SpriteBatchNode::create("animations/grossini.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
    sprite1->setPosition(Vec2( s.width/2 - 90, s.height/4));
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2( s.width/2 - 60,s.height/4));
    
    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2( s.width/2 - 30, s.height/4));
    
    batch->addChild(sprite1, 10);
    batch->addChild(sprite2, -10);
    batch->addChild(sprite3, -5);

    // test 4
    batch = SpriteBatchNode::create("animations/grossini.png", 50);
    addChild(batch, 0, kTagSpriteBatchNode);
    
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
    sprite1->setPosition(Vec2( s.width/2 +30, s.height/4));
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2( s.width/2 +60,s.height/4));
    
    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2( s.width/2 +90, s.height/4));
    
    batch->addChild(sprite1, -10);
    batch->addChild(sprite2, -5);
    batch->addChild(sprite3, -2);
}

void SpriteBatchNodeChildrenZ::onExit()
{
    SpriteTestDemo::onExit();
    _director->getSpriteFrameCache().removeUnusedSpriteFrames();
}

std::string SpriteBatchNodeChildrenZ::title() const
{
    return "SpriteBatchNode Children Z";
}

//------------------------------------------------------------------
//
// SpriteChildrenVisibility
//
//------------------------------------------------------------------

SpriteChildrenVisibility::SpriteChildrenVisibility()
{
    auto s = Director::getInstance()->getWinSize();

    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini.plist");

    Node *aParent;
    Sprite* sprite1, *sprite2, *sprite3;
    //
    // SpriteBatchNode
    //
    // parents
    aParent = SpriteBatchNode::create("animations/grossini.png", 50);
    aParent->setPosition( Vec2(s.width/3, s.height/2) );
    addChild(aParent, 0);
    
    auto & cache = _director->getSpriteFrameCache();
    
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
    sprite1->setPosition(Vec2(0,0));
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(20,30));
    
    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-20,30));
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);
    
    sprite1->runAction(std::make_unique<Blink>(5, 10));
    
    //
    // Sprite
    //
    aParent = Node::create();
    aParent->setPosition( Vec2(2*s.width/3, s.height/2) );
    addChild(aParent, 0);

    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
    sprite1->setPosition(Vec2(0,0));
            
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(20,30));
    
    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-20,30));
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);
    
    sprite1->runAction(std::make_unique<Blink>(5, 10));
}

void SpriteChildrenVisibility::onExit()
{
    SpriteTestDemo::onExit();
    _director->getSpriteFrameCache().removeUnusedSpriteFrames();
}

std::string SpriteChildrenVisibility::title() const
{
    return "Sprite & SpriteBatchNode Visibility";
}

//------------------------------------------------------------------
//
// SpriteChildrenVisibilityIssue665
//
//------------------------------------------------------------------
SpriteChildrenVisibilityIssue665::SpriteChildrenVisibilityIssue665()
{
    auto s = Director::getInstance()->getWinSize();

    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini.plist");

    Node *aParent;
    Sprite *sprite1, *sprite2, *sprite3;
    //
    // SpriteBatchNode
    //
    // parents
    aParent = SpriteBatchNode::create("animations/grossini.png", 50);
    aParent->setPosition(Vec2(s.width/3, s.height/2));
    addChild(aParent, 0);

    auto & cache = _director->getSpriteFrameCache();

    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
    sprite1->setPosition(Vec2(0,0));

    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(20,30));

    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-20,30));

    // test issue #665
    sprite1->setVisible(false);

    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);

    //
    // Sprite
    //
    aParent = Node::create();
    aParent->setPosition(Vec2(2*s.width/3, s.height/2));
    addChild(aParent, 0);

    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
    sprite1->setPosition(Vec2(0,0));

    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(20,30));

    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-20,30));

    // test issue #665
    sprite1->setVisible(false);

    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, 2);
}

SpriteChildrenVisibilityIssue665::~SpriteChildrenVisibilityIssue665()
{
    _director->getSpriteFrameCache().removeUnusedSpriteFrames();
}

std::string SpriteChildrenVisibilityIssue665::title() const
{
    return "Sprite & SpriteBatchNode Visibility";
}

std::string SpriteChildrenVisibilityIssue665::subtitle() const
{
    return "No sprites should be visible";
}

//------------------------------------------------------------------
//
// SpriteChildrenAnchorPoint
//
//------------------------------------------------------------------
SpriteChildrenAnchorPoint::SpriteChildrenAnchorPoint()
{
    auto s = Director::getInstance()->getWinSize();
    
    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini.plist");
    
    Node *aParent;
    Sprite* sprite1, *sprite2, *sprite3, *sprite4, *point;
    //
    // SpriteBatchNode
    //
    // parents
    
    aParent = Node::create();
    addChild(aParent, 0);
    
    auto & cache = _director->getSpriteFrameCache();

    // anchor (0,0)
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_08.png"));
    sprite1->setPosition(Vec2(s.width/4,s.height/2));
    sprite1->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );

    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(20,30));
    
    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-20,30));
    
    sprite4 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_04.png"));
    sprite4->setPosition(Vec2(0,0));
    sprite4->setScale( 0.5f );

    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = Sprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);
    
    // anchor (0.5, 0.5)
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_08.png"));
    sprite1->setPosition(Vec2(s.width/2,s.height/2));
    sprite1->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(20,30));

    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-20,30));

    sprite4 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_04.png"));
    sprite4->setPosition(Vec2(0,0));
    sprite4->setScale( 0.5f );        

    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = Sprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);        
    
    
    // anchor (1,1)
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_08.png"));
    sprite1->setPosition(Vec2(s.width/2+s.width/4,s.height/2));
    sprite1->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );

    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(20,30));
    
    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-20,30));
    
    sprite4 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_04.png"));
    sprite4->setPosition(Vec2(0,0));
    sprite4->setScale( 0.5f );        
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = Sprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);        
}

void SpriteChildrenAnchorPoint::onExit()
{
    SpriteTestDemo::onExit();
    _director->getSpriteFrameCache().removeUnusedSpriteFrames();
}

std::string SpriteChildrenAnchorPoint::title() const
{
    return "Testing Sprite";
}

std::string SpriteChildrenAnchorPoint::subtitle() const
{
    return "children + anchor point";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeChildrenAnchorPoint
//
//------------------------------------------------------------------
SpriteBatchNodeChildrenAnchorPoint::SpriteBatchNodeChildrenAnchorPoint()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();

    cache.addSpriteFramesWithFile("animations/grossini.plist");
    
    Node *aParent;
    Sprite* sprite1, *sprite2, *sprite3, *sprite4, *point;
    //
    // SpriteBatchNode
    //
    // parents
    
    aParent = SpriteBatchNode::create("animations/grossini.png", 50);
    addChild(aParent, 0);
    
    // anchor (0,0)
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_08.png"));
    sprite1->setPosition(Vec2(s.width/4,s.height/2));
    sprite1->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(20,30));
    
    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-20,30));
    
    sprite4 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_04.png"));
    sprite4->setPosition(Vec2(0,0));
    sprite4->setScale( 0.5f );
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = Sprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);
    
    
    // anchor (0.5, 0.5)
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_08.png"));
    sprite1->setPosition(Vec2(s.width/2,s.height/2));
    sprite1->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(20,30));
    
    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-20,30));
    
    sprite4 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_04.png"));
    sprite4->setPosition(Vec2(0,0));
    sprite4->setScale( 0.5f );        
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = Sprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);        
    
    
    // anchor (1,1)
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_08.png"));
    sprite1->setPosition(Vec2(s.width/2+s.width/4,s.height/2));
    sprite1->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_02.png"));
    sprite2->setPosition(Vec2(20,30));
    
    sprite3 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_03.png"));
    sprite3->setPosition(Vec2(-20,30));
    
    sprite4 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_04.png"));
    sprite4->setPosition(Vec2(0,0));
    sprite4->setScale( 0.5f );        
    
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2, -2);
    sprite1->addChild(sprite3, -2);
    sprite1->addChild(sprite4, 3);
    
    point = Sprite::create("Images/r1.png");
    point->setScale( 0.25f );
    point->setPosition( sprite1->getPosition() );
    addChild(point, 10);        
}

void SpriteBatchNodeChildrenAnchorPoint::onExit()
{
    SpriteTestDemo::onExit();
    _director->getSpriteFrameCache().removeUnusedSpriteFrames();
}

std::string SpriteBatchNodeChildrenAnchorPoint::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeChildrenAnchorPoint::subtitle() const
{
    return "children + anchor point";
}


//------------------------------------------------------------------
//
// SpriteBatchNodeChildrenScale
//
//------------------------------------------------------------------
SpriteBatchNodeChildrenScale::SpriteBatchNodeChildrenScale()
{
    auto s = Director::getInstance()->getWinSize();        
    
    auto & cache = _director->getSpriteFrameCache();

    cache.addSpriteFramesWithFile("animations/grossini_family.plist");

    Node *aParent;
    Sprite* sprite1, *sprite2;
    auto seq = std::make_unique<RepeatForever>(std::make_unique<RotateBy>(10, 360));
    
    //
    // Children + Scale using Sprite
    // Test 1
    //
    aParent = Node::create();
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister1.png"));
    sprite1->setPosition( Vec2( s.width/4, s.height/4) );
    sprite1->setScaleX( -0.5f );
    sprite1->setScaleY( 2.0f );
    sprite1->runAction( std::unique_ptr<RepeatForever>(seq->clone()) );
    
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister2.png"));
    sprite2->setPosition( Vec2( 50,0) );
    
    addChild(aParent);
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2);

    
    //
    // Children + Scale using SpriteBatchNode
    // Test 2
    //
    
    aParent = SpriteBatchNode::create("animations/grossini_family.png");
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister1.png"));
    sprite1->setPosition( Vec2( 3*s.width/4, s.height/4) );
    sprite1->setScaleX( -0.5f );
    sprite1->setScaleY( 2.0f );
    sprite1->runAction( std::unique_ptr<RepeatForever>(seq->clone()) );
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister2.png"));
    sprite2->setPosition( Vec2( 50,0) );
    
    addChild(aParent);
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2);

    
    //
    // Children + Scale using Sprite
    // Test 3
    //
    
    aParent = Node::create();
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister1.png"));
    sprite1->setPosition( Vec2( s.width/4, 2*s.height/3) );
    sprite1->setScaleX( 1.5f );
    sprite1->setScaleY( -0.5f );
    sprite1->runAction( std::unique_ptr<RepeatForever>(seq->clone()) );
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister2.png"));
    sprite2->setPosition( Vec2( 50,0) );
    
    addChild(aParent);
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2);
    
    //
    // Children + Scale using Sprite
    // Test 4
    //
    
    aParent = SpriteBatchNode::create("animations/grossini_family.png");
    sprite1 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister1.png"));
    sprite1->setPosition( Vec2( 3*s.width/4, 2*s.height/3) );
    sprite1->setScaleX( 1.5f );
    sprite1->setScaleY( -0.5f);
    sprite1->runAction( std::unique_ptr<RepeatForever>(seq->clone()) );
    
    sprite2 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister2.png"));
    sprite2->setPosition( Vec2( 50,0) );
    
    addChild(aParent);
    aParent->addChild(sprite1);
    sprite1->addChild(sprite2);
    
}

std::string SpriteBatchNodeChildrenScale::title() const
{
    return "Testing Sprite / SpriteBatchNode";
}

std::string SpriteBatchNodeChildrenScale::subtitle() const
{
    return "child + scale + rot";
}

//------------------------------------------------------------------
//
// SpriteChildrenChildren
//
//------------------------------------------------------------------
SpriteChildrenChildren::SpriteChildrenChildren()
{
    auto s = Director::getInstance()->getWinSize();        
    
    auto & cache = _director->getSpriteFrameCache();

    cache.addSpriteFramesWithFile("animations/ghosts.plist");
    
    Node *aParent;
    Sprite *l1, *l2a, *l2b, *l3a1, *l3a2, *l3b1, *l3b2;
    
    auto rot = std::make_unique<RotateBy>(10, 360);
    auto rot_back_fe = std::make_unique<RepeatForever>(std::unique_ptr<ActionInterval>(rot->reverse()));
    auto seq = std::make_unique<RepeatForever>(std::move(rot));
    
    //
    // SpriteBatchNode: 3 levels of children
    //
    
    aParent = Node::create();
    addChild(aParent);
    
    // parent
    l1 = Sprite::create(cache.getSpriteFrameByName("father.gif"));
    l1->setPosition( Vec2( s.width/2, s.height/2) );
    l1->runAction( std::unique_ptr<RepeatForever>(seq->clone()) );
    aParent->addChild(l1);
    auto l1Size = l1->getContentSize();
    
    // child left
    l2a = Sprite::create(cache.getSpriteFrameByName("sister1.gif"));
    l2a->setPosition( Vec2( -50 + l1Size.width/2, 0 + l1Size.height/2) );
    l2a->runAction( std::unique_ptr<RepeatForever>(rot_back_fe->clone()) );
    l1->addChild(l2a);
    auto l2aSize = l2a->getContentSize();        
    
    
    // child right
    l2b = Sprite::create(cache.getSpriteFrameByName("sister2.gif"));
    l2b->setPosition( Vec2( +50 + l1Size.width/2, 0 + l1Size.height/2) );
    l2b->runAction( std::unique_ptr<RepeatForever>(rot_back_fe->clone()) );
    l1->addChild(l2b);
    auto l2bSize = l2a->getContentSize();        
    
    
    // child left bottom
    l3a1 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3a1->setScale( 0.45f );
    l3a1->setPosition( Vec2(0+l2aSize.width/2,-100+l2aSize.height/2) );
    l2a->addChild(l3a1);
    
    // child left top
    l3a2 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3a2->setScale( 0.45f );
    l3a1->setPosition( Vec2(0+l2aSize.width/2,+100+l2aSize.height/2) );
    l2a->addChild(l3a2);
    
    // child right bottom
    l3b1 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3b1->setScale( 0.45f);
    l3b1->setFlippedY( true );
    l3b1->setPosition( Vec2(0+l2bSize.width/2,-100+l2bSize.height/2) );
    l2b->addChild(l3b1);
    
    // child right top
    l3b2 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3b2->setScale( 0.45f );
    l3b2->setFlippedY( true );
    l3b1->setPosition( Vec2(0+l2bSize.width/2,+100+l2bSize.height/2) );
    l2b->addChild(l3b2);
}

std::string SpriteChildrenChildren::title() const
{
    return "Testing Sprite";
}

std::string SpriteChildrenChildren::subtitle() const
{
    return "multiple levels of children";
}


//------------------------------------------------------------------
//
// SpriteBatchNodeChildrenChildren
//
//------------------------------------------------------------------

SpriteBatchNodeChildrenChildren::SpriteBatchNodeChildrenChildren()
{
    auto s = Director::getInstance()->getWinSize();        
    
    auto & cache = _director->getSpriteFrameCache();

    cache.addSpriteFramesWithFile("animations/ghosts.plist");
    
    SpriteBatchNode *aParent;
    Sprite *l1, *l2a, *l2b, *l3a1, *l3a2, *l3b1, *l3b2;
    
    auto rot = std::make_unique<RotateBy>(10, 360);
    auto rot_back_fe = std::make_unique<RepeatForever>(std::unique_ptr<ActionInterval>(rot->reverse()));
    auto seq = std::make_unique<RepeatForever>(std::move(rot));

    //
    // SpriteBatchNode: 3 levels of children
    //
    
    aParent = SpriteBatchNode::create("animations/ghosts.png");
    aParent->getTexture()->generateMipmap();
    addChild(aParent);
    
    // parent
    l1 = Sprite::create(cache.getSpriteFrameByName("father.gif"));
    l1->setPosition( Vec2( s.width/2, s.height/2) );
    l1->runAction( std::unique_ptr<RepeatForever>(seq->clone()) );
    aParent->addChild(l1);
    auto l1Size = l1->getContentSize();

    // child left
    l2a = Sprite::create(cache.getSpriteFrameByName("sister1.gif"));
    l2a->setPosition( Vec2( -50 + l1Size.width/2, 0 + l1Size.height/2) );
    l2a->runAction( std::unique_ptr<RepeatForever>(rot_back_fe->clone()) );
    l1->addChild(l2a);
    auto l2aSize = l2a->getContentSize();        


    // child right
    l2b = Sprite::create(cache.getSpriteFrameByName("sister2.gif"));
    l2b->setPosition( Vec2( +50 + l1Size.width/2, 0 + l1Size.height/2) );
    l2b->runAction( std::unique_ptr<RepeatForever>(rot_back_fe->clone()) );
    l1->addChild(l2b);
    auto l2bSize = l2a->getContentSize();        

    
    // child left bottom
    l3a1 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3a1->setScale( 0.45f );
    l3a1->setPosition( Vec2(0+l2aSize.width/2,-100+l2aSize.height/2) );
    l2a->addChild(l3a1);
    
    // child left top
    l3a2 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3a2->setScale( 0.45f );
    l3a1->setPosition( Vec2(0+l2aSize.width/2,+100+l2aSize.height/2) );
    l2a->addChild(l3a2);
    
    // child right bottom
    l3b1 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3b1->setScale( 0.45f );
    l3b1->setFlippedY( true );
    l3b1->setPosition( Vec2(0+l2bSize.width/2,-100+l2bSize.height/2) );
    l2b->addChild(l3b1);

    // child right top
    l3b2 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3b2->setScale( 0.45f );
    l3b2->setFlippedY( true );
    l3b1->setPosition( Vec2(0+l2bSize.width/2,+100+l2bSize.height/2) );
    l2b->addChild(l3b2);
    
}

std::string SpriteBatchNodeChildrenChildren::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeChildrenChildren::subtitle() const
{
    return "multiple levels of children";
}

//------------------------------------------------------------------
//
// SpriteBatchNodeSkewNegativeScaleChildren
//
//------------------------------------------------------------------

SpriteBatchNodeSkewNegativeScaleChildren::SpriteBatchNodeSkewNegativeScaleChildren()
{
    auto s = Director::getInstance()->getWinSize();

    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);

    for(int i=0;i<2;i++) {
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2( s.width/4*(i+1), s.height/2));

        // Skew
        auto skewX = std::make_unique<SkewBy>(2, 45, 0);
        auto skewX_back = std::unique_ptr<FiniteTimeAction>(skewX->reverse());
        auto skewY = std::make_unique<SkewBy>(2, 0, 45);
        auto skewY_back = std::unique_ptr<FiniteTimeAction>(skewY->reverse());

        if(i == 1)
        {
            sprite->setScale(-1.0f);
        }

        auto seq_skew = std::make_unique<Sequence>(
            std::move(skewX),
            std::move(skewX_back),
            std::move(skewY),
            std::move(skewY_back)
        );
        sprite->runAction( std::make_unique<RepeatForever>( std::move( seq_skew)));

        auto child1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        child1->setPosition(Vec2(sprite->getContentSize().width / 2.0f, sprite->getContentSize().height / 2.0f));

        child1->setScale(0.8f);

        sprite->addChild(child1);

        spritebatch->addChild(sprite, i);
    }
}

SpriteBatchNodeSkewNegativeScaleChildren::~SpriteBatchNodeSkewNegativeScaleChildren()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteBatchNodeSkewNegativeScaleChildren::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeSkewNegativeScaleChildren::subtitle() const
{
    return "skew + negative scale with children";
}

// SpriteSkewNegativeScaleChildren

SpriteSkewNegativeScaleChildren::SpriteSkewNegativeScaleChildren()
{
    auto s = Director::getInstance()->getWinSize();

    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    auto parent = Node::create();
    addChild(parent);

    for(int i=0;i<2;i++) {
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2( s.width/4*(i+1), s.height/2));

        // Skew
        auto skewX = std::make_unique<SkewBy>(2, 45, 0);
        auto skewX_back = std::unique_ptr<FiniteTimeAction>(skewX->reverse());
        auto skewY = std::make_unique<SkewBy>(2, 0, 45);
        auto skewY_back = std::unique_ptr<FiniteTimeAction>(skewY->reverse());

        if(i == 1)
        {
            sprite->setScale(-1.0f);
        }

        auto seq_skew = std::make_unique<Sequence>(
            std::move(skewX),
            std::move(skewX_back),
            std::move(skewY),
            std::move(skewY_back)
        );
        sprite->runAction( std::make_unique<RepeatForever>( std::move( seq_skew)));

        auto child1 = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        child1->setPosition(Vec2(sprite->getContentSize().width / 2.0f, sprite->getContentSize().height / 2.0f));

        sprite->addChild(child1);

        child1->setScale(0.8f);

        parent->addChild(sprite, i);
    }
}

SpriteSkewNegativeScaleChildren::~SpriteSkewNegativeScaleChildren()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteSkewNegativeScaleChildren::title() const
{
    return "Testing Sprite";
}

std::string SpriteSkewNegativeScaleChildren::subtitle() const
{
    return "skew + negative scale with children";
}

//------------------------------------------------------------------
//
// SpriteNilTexture
//
//------------------------------------------------------------------

SpriteNilTexture::SpriteNilTexture()
{
    auto s = Director::getInstance()->getWinSize();

    Sprite* sprite = nullptr;
    
    // TEST: If no texture is given, then Opacity + Color should work.

    sprite = Sprite::create();
    sprite->setTextureRect( Rect(0, 0, 300,300) );
    sprite->setColor(Color3B::RED);
    sprite->setOpacity(128);
    sprite->setPosition(Vec2(3*s.width/4, s.height/2));
    addChild(sprite, 100);

    sprite = Sprite::create();
    sprite->setTextureRect(Rect(0, 0, 300,300));
    sprite->setColor(Color3B::BLUE);
    sprite->setOpacity(128);
    sprite->setPosition(Vec2(1*s.width/4, s.height/2));
    addChild(sprite, 100);
}

std::string SpriteNilTexture::title() const
{
    return "Sprite without texture";
}

std::string SpriteNilTexture::subtitle() const
{
    return "opacity and color should work";
}

class MySprite1 : public Sprite
{
public:
    CREATE_FUNC(MySprite1);
    MySprite1() {}
    static MySprite1* createWithSpriteFrame(SpriteFrame *spriteFrameName)
    {
        auto sprite = MySprite1::create();
        sprite->setSpriteFrame(spriteFrameName);
        return sprite;
    }
};

class MySprite2 : public Sprite
{
public:
    CREATE_FUNC(MySprite2);
    MySprite2() {}
    static MySprite2* create(const std::string& name)
    {
        auto sprite = MySprite2::create();
        sprite ->setTexture(name);
        return sprite;
    }
};

//------------------------------------------------------------------
//
// SpriteSubclass
//
//------------------------------------------------------------------
SpriteSubclass::SpriteSubclass()
{
    auto s = Director::getInstance()->getWinSize();

    auto & cache = _director->getSpriteFrameCache();

    cache.addSpriteFramesWithFile("animations/ghosts.plist");

    auto aParent = SpriteBatchNode::create("animations/ghosts.png");

    // MySprite1
    MySprite1 *sprite = MySprite1::createWithSpriteFrame(cache.getSpriteFrameByName("father.gif"));
    sprite->setPosition(Vec2( s.width/4*1, s.height/2));
    aParent->addChild(sprite);
    addChild(aParent);

    // MySprite2
    MySprite2 *sprite2 = MySprite2::create("Images/grossini.png");
    addChild(sprite2);
    sprite2->setPosition(Vec2(s.width/4*3, s.height/2));
}

std::string SpriteSubclass::title() const
{
    return "Sprite subclass";
}

std::string SpriteSubclass::subtitle() const
{
    return "Testing initWithTexture:rect method";
}


class DoubleSprite : public Sprite
{
public:
    DoubleSprite() {_HD = false;}
    virtual bool initWithTexture(Texture2D* texture, const Rect& rect);
    // Called everytime the vertex needs to be updated.
    virtual void setContentSize(const Size& size);
    // rect used only for the vertex. Called everytime the vertex needs to be updated.
    virtual void setVertexRect(const Rect& rect);

    static DoubleSprite* create(const std::string& filename);
    bool _HD;
};

DoubleSprite* DoubleSprite::create(const std::string& filename)
{
    auto sprite = new (std::nothrow) DoubleSprite;
    sprite->initWithFile(filename);
    sprite->autorelease();
    return sprite;
}

bool DoubleSprite::initWithTexture(Texture2D* texture, const Rect& rect)
{
    if( Sprite::initWithTexture(texture, rect)) 
    {
        return true;
    }

    return false;
}

// Called everytime the vertex needs to be updated.
void DoubleSprite::setContentSize(const Size& size)
{
    auto s = size;
    // If Retina Display and Texture is in HD then scale the vertex rect
    if( CC_CONTENT_SCALE_FACTOR() == 2 && ! _HD ) {
        s.width *= 2;
        s.height *= 2;
    }

    Sprite::setContentSize(s);
}

// rect used only for the vertex. Called everytime the vertex needs to be updated.
void DoubleSprite::setVertexRect(const Rect& rect)
{
    auto tmpRect = rect;
    // If Retina Display and Texture is in HD then scale the vertex rect
    if( CC_CONTENT_SCALE_FACTOR() == 2 && ! _HD ) {
        tmpRect.size.width *= 2;
        tmpRect.size.height *= 2;
    }

    Sprite::setVertexRect(tmpRect);
}


// SpriteDoubleResolution

SpriteDoubleResolution::SpriteDoubleResolution()
{
    auto s = Director::getInstance()->getWinSize();

    //
    // LEFT: SD sprite
    //
    // there is no HD resolution file of grossini_dance_08.
    auto spriteSD = DoubleSprite::create("Images/grossini_dance_08.png");
    addChild(spriteSD);
    spriteSD->setPosition(Vec2(s.width/4*1,s.height/2));

    auto child1_left = DoubleSprite::create("Images/grossini_dance_08.png");
    spriteSD->addChild(child1_left);
    child1_left->setPosition(Vec2(-30,0));

    auto child1_right = Sprite::create("Images/grossini.png");
    spriteSD->addChild(child1_right);
    child1_left->setPosition(Vec2( spriteSD->getContentSize().height, 0));



    //
    // RIGHT: HD sprite
    //
    // there is an HD version of grossini.png
    auto spriteHD = Sprite::create("Images/grossini.png");
    addChild(spriteHD);
    spriteHD->setPosition(Vec2(s.width/4*3,s.height/2));

    auto child2_left = DoubleSprite::create("Images/grossini_dance_08.png");
    spriteHD->addChild(child2_left);
    child2_left->setPosition(Vec2(-30,0));

    auto child2_right = Sprite::create("Images/grossini.png");
    spriteHD->addChild(child2_right);
    child2_left->setPosition(Vec2( spriteHD->getContentSize().height, 0));



    // Actions
    auto scale = std::make_unique<ScaleBy>(2, 0.5);
    auto scale_back = std::unique_ptr<FiniteTimeAction>(scale->reverse());
    auto seq = std::make_unique<Sequence>( std::move(scale), std::move( scale_back) );

    auto seq_copy = std::unique_ptr<Action>(seq->clone());

    spriteSD->runAction( std::move(seq) );
    spriteHD->runAction( std::move(seq_copy) );

}

std::string SpriteDoubleResolution::title() const
{
    return "Sprite Double resolution";
}

std::string SpriteDoubleResolution::subtitle() const
{
    return "Retina Display. SD (left) should be equal to HD (right)";
}

//------------------------------------------------------------------
//
// AnimationCache
//
//------------------------------------------------------------------
AnimationCacheTest::AnimationCacheTest()
{
    auto & frameCache = _director->getSpriteFrameCache();
    frameCache.addSpriteFramesWithFile("animations/grossini.plist");
    frameCache.addSpriteFramesWithFile("animations/grossini_gray.plist");
    frameCache.addSpriteFramesWithFile("animations/grossini_blue.plist");

    //
    // create animation "dance"
    //
    std::vector<retaining_ptr<SpriteFrame>> animFrames;
    animFrames.reserve(15);
    char str[100] = {0};

    for(int i = 1; i < 15; i++)
    {
        sprintf(str, "grossini_dance_%02d.png",i);
        auto frame = frameCache.getSpriteFrameByName(str);
        animFrames.push_back(to_retaining_ptr(frame));
    }

    std::unique_ptr<Animation> animation(new Animation( std::move(animFrames), 0.2f));

    // Add an animation to the Cache
    AnimationCache::getInstance()->addAnimation( std::move(animation), "dance");

    //
    // create animation "dance gray"
    //
    animFrames.clear();

    for(int i = 1; i < 15; i++)
    {
        sprintf(str, "grossini_dance_gray_%02d.png",i);
        auto frame = frameCache.getSpriteFrameByName(str);
        animFrames.push_back(to_retaining_ptr(frame));
    }

    animation.reset(new Animation( std::move(animFrames), 0.2f));

    // Add an animation to the Cache
    AnimationCache::getInstance()->addAnimation( std::move(animation), "dance_gray");

    //
    // create animation "dance blue"
    //
    animFrames.clear();

    for(int i = 1; i < 4; i++)
    {
        sprintf(str, "grossini_blue_%02d.png",i);
        auto frame = frameCache.getSpriteFrameByName(str);
        animFrames.push_back(to_retaining_ptr(frame));
    }

    animation.reset(new Animation( std::move(animFrames), 0.2f));

    // Add an animation to the Cache
    AnimationCache::getInstance()->addAnimation( std::move(animation), "dance_blue");


    auto animCache = AnimationCache::getInstance();

    auto normal = animCache->cloneAnimation("dance");
    normal->setRestoreOriginalFrame(true);
    auto dance_grey = animCache->cloneAnimation("dance_gray");
    dance_grey->setRestoreOriginalFrame(true);
    auto dance_blue = animCache->cloneAnimation("dance_blue");
    dance_blue->setRestoreOriginalFrame(true);

    auto animN = std::make_unique<Animate>( std::move( normal));
    auto animG = std::make_unique<Animate>( std::move( dance_grey));
    auto animB = std::make_unique<Animate>( std::move( dance_blue));

    auto seq = std::make_unique<Sequence>(
        std::move(animN),
        std::move(animG),
        std::move(animB)
    );

    // create an sprite without texture
    auto grossini = Sprite::create();
    auto frame = frameCache.getSpriteFrameByName("grossini_dance_01.png");
    grossini->setSpriteFrame(frame);

    auto winSize = Director::getInstance()->getWinSize();
    grossini->setPosition(Vec2(winSize.width/2, winSize.height/2));
    addChild(grossini);

    // run the animation
    grossini->runAction(std::move(seq));
}

AnimationCacheTest::~AnimationCacheTest()
{
    auto & frameCache = _director->getSpriteFrameCache();
    frameCache.removeSpriteFramesFromFile("animations/grossini.plist");
    frameCache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
    frameCache.removeSpriteFramesFromFile("animations/grossini_blue.plist");
}

std::string AnimationCacheTest::title() const
{
    return "AnimationCache";
}

std::string AnimationCacheTest::subtitle() const
{
    return "Sprite should be animated";
}

// AnimationCacheFile


AnimationCacheFile::AnimationCacheFile()
{
    auto & frameCache = _director->getSpriteFrameCache();
    frameCache.addSpriteFramesWithFile("animations/grossini.plist");
    frameCache.addSpriteFramesWithFile("animations/grossini_gray.plist");
    frameCache.addSpriteFramesWithFile("animations/grossini_blue.plist");


    // Purge previously loaded animation
    AnimationCache::destroyInstance();

    auto animCache = AnimationCache::getInstance();

    // Add an animation to the Cache
    animCache->addAnimationsWithFile("animations/animations.plist");


    auto normal = animCache->extractAnimation("dance_1");
    normal->setRestoreOriginalFrame(true);
    auto dance_grey = animCache->extractAnimation("dance_2");
    dance_grey->setRestoreOriginalFrame(true);
    auto dance_blue = animCache->extractAnimation("dance_3");
    dance_blue->setRestoreOriginalFrame(true);

    auto animN = std::make_unique<Animate>( std::move( normal));
    auto animG = std::make_unique<Animate>( std::move( dance_grey));
    auto animB = std::make_unique<Animate>( std::move( dance_blue));

    auto seq = std::make_unique<Sequence>(
        std::move(animN),
        std::move(animG),
        std::move(animB)
    );

    // create an sprite without texture
    auto grossini = Sprite::create();

    auto frame = frameCache.getSpriteFrameByName("grossini_dance_01.png");
    grossini->setSpriteFrame(frame);

    auto winSize = Director::getInstance()->getWinSize();

    grossini->setPosition(Vec2(winSize.width/2, winSize.height/2));

    addChild(grossini);


    // run the animation
    grossini->runAction(std::move(seq));
}

AnimationCacheFile::~AnimationCacheFile()
{
    auto & frameCache = _director->getSpriteFrameCache();
    frameCache.removeSpriteFramesFromFile("animations/grossini.plist");
    frameCache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
    frameCache.removeSpriteFramesFromFile("animations/grossini_blue.plist");
}

std::string AnimationCacheFile::title() const
{
    return "AnimationCache - Load file";
}

std::string AnimationCacheFile::subtitle() const
{
    return "Sprite should be animated";
}

// SpriteBatchBug1217

SpriteBatchBug1217::SpriteBatchBug1217()
{
    auto bn = SpriteBatchNode::create("Images/grossini_dance_atlas.png", 15);

    auto s1 = Sprite::create(bn->getTexture(), Rect(0, 0, 57, 57));
    auto s2 = Sprite::create(bn->getTexture(), Rect(0, 0, 57, 57));
    auto s3 = Sprite::create(bn->getTexture(), Rect(0, 0, 57, 57));

    s1->setColor(Color3B(255, 0, 0));
    s2->setColor(Color3B(0, 255, 0));
    s3->setColor(Color3B(0, 0, 255));

    s1->setPosition(Vec2(20,200));
    s2->setPosition(Vec2(100,0));
    s3->setPosition(Vec2(100,0));

    bn->setPosition(Vec2(0,0));

    //!!!!!
    s1->addChild(s2);
    s2->addChild(s3);
    bn->addChild(s1);

    addChild(bn);
}

std::string SpriteBatchBug1217::title() const
{
    return "SpriteBatch - Bug 1217";
}

std::string SpriteBatchBug1217::subtitle() const
{
    return "Adding big family to spritebatch. You shall see 3 heads";
}

//
// SpriteOffsetAnchorSkew
// 
SpriteOffsetAnchorSkew::SpriteOffsetAnchorSkew()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    for (int i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2(s.width / 4 * (i + 1), s.height / 2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            break;
        case 1:
            sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            break;
        case 2:
            sprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            break;
        }

        point->setPosition(sprite->getPosition());

        run_grossini_dance_animation_forever(sprite);

        auto skewX = std::make_unique<SkewBy>(2, 45, 0);
        auto skewX_back = std::unique_ptr<FiniteTimeAction>(skewX->reverse());
        auto skewY = std::make_unique<SkewBy>(2, 0, 45);
        auto skewY_back = std::unique_ptr<FiniteTimeAction>(skewY->reverse());

        auto seq_skew = std::make_unique<Sequence>(
            std::move(skewX),
            std::move(skewX_back),
            std::move(skewY),
            std::move(skewY_back)
        );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_skew)));

        addChild(sprite, 0);
    }
}

SpriteOffsetAnchorSkew::~SpriteOffsetAnchorSkew()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteOffsetAnchorSkew::title() const
{
    return "Testing Sprite";
}

std::string SpriteOffsetAnchorSkew::subtitle() const
{
    return "offset + anchor + skew";
}

//
// SpriteBatchNodeOffsetAnchorSkew
// 
SpriteBatchNodeOffsetAnchorSkew::SpriteBatchNodeOffsetAnchorSkew()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);

    for (int i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2(s.width / 4 * (i + 1), s.height / 2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            break;
        case 1:
            sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            break;
        case 2:
            sprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT );
            break;
        }

        point->setPosition(sprite->getPosition());
        
        run_grossini_dance_animation_forever(sprite);

        auto skewX = std::make_unique<SkewBy>(2, 45, 0);
        auto skewX_back = std::unique_ptr<FiniteTimeAction>(skewX->reverse());
        auto skewY = std::make_unique<SkewBy>(2, 0, 45);
        auto skewY_back = std::unique_ptr<FiniteTimeAction>(skewY->reverse());

        auto seq_skew = std::make_unique<Sequence>(
            std::move(skewX),
            std::move(skewX_back),
            std::move(skewY),
            std::move(skewY_back)
        );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_skew)));

        spritebatch->addChild(sprite, i);
    }
}

SpriteBatchNodeOffsetAnchorSkew::~SpriteBatchNodeOffsetAnchorSkew()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteBatchNodeOffsetAnchorSkew::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeOffsetAnchorSkew::subtitle() const
{
    return "offset + anchor + skew";
}

//
// SpriteOffsetAnchorSkewScale
//
SpriteOffsetAnchorSkewScale::SpriteOffsetAnchorSkewScale()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    for (int i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2(s.width / 4 * (i + 1), s.height / 2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            break;
        case 1:
            sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            break;
        case 2:
            sprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT );
            break;
        }

        point->setPosition(sprite->getPosition());

        run_grossini_dance_animation_forever(sprite);

        // Skew
        auto skewX = std::make_unique<SkewBy>(2, 45, 0);
        auto skewX_back = std::unique_ptr<FiniteTimeAction>(skewX->reverse());
        auto skewY = std::make_unique<SkewBy>(2, 0, 45);
        auto skewY_back = std::unique_ptr<FiniteTimeAction>(skewY->reverse());

        auto seq_skew = std::make_unique<Sequence>(
            std::move(skewX),
            std::move(skewX_back),
            std::move(skewY),
            std::move(skewY_back)
        );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_skew)));

        // Scale
        auto scale = std::make_unique<ScaleBy>(2, 2);
        auto scale_back = std::unique_ptr<FiniteTimeAction>(scale->reverse());
        auto seq_scale = std::make_unique<Sequence>( std::move(scale), std::move( scale_back) );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_scale)));

        addChild(sprite, 0);
    }
}

SpriteOffsetAnchorSkewScale::~SpriteOffsetAnchorSkewScale()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteOffsetAnchorSkewScale::title() const
{
    return "Testing Sprite";
}

std::string SpriteOffsetAnchorSkewScale::subtitle() const
{
    return "anchor + skew + scale";
}

///

SpriteBatchNodeOffsetAnchorSkewScale::SpriteBatchNodeOffsetAnchorSkewScale()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);

    for (int i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2(s.width / 4 * (i + 1), s.height / 2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            break;
        case 1:
            sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            break;
        case 2:
            sprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT );
            break;
        }

        point->setPosition(sprite->getPosition());        

        run_grossini_dance_animation_forever(sprite);

        // skew
        auto skewX = std::make_unique<SkewBy>(2, 45, 0);
        auto skewX_back = std::unique_ptr<FiniteTimeAction>(skewX->reverse());
        auto skewY = std::make_unique<SkewBy>(2, 0, 45);
        auto skewY_back = std::unique_ptr<FiniteTimeAction>(skewY->reverse());

        auto seq_skew = std::make_unique<Sequence>(
            std::move(skewX),
            std::move(skewX_back),
            std::move(skewY),
            std::move(skewY_back)
        );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_skew)));

        // scale 
        auto scale = std::make_unique<ScaleBy>(2, 2);
        auto scale_back = std::unique_ptr<FiniteTimeAction>(scale->reverse());
        auto seq_scale = std::make_unique<Sequence>( std::move(scale), std::move( scale_back) );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_scale)));

        spritebatch->addChild(sprite, i);
    }
}

SpriteBatchNodeOffsetAnchorSkewScale::~SpriteBatchNodeOffsetAnchorSkewScale()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteBatchNodeOffsetAnchorSkewScale::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeOffsetAnchorSkewScale::subtitle() const
{
    return "anchor + skew + scale";
}


//
// SpriteOffsetAnchorFlip
// 
SpriteOffsetAnchorFlip::SpriteOffsetAnchorFlip()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");

    for (int i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2(s.width / 4 * (i + 1), s.height / 2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            break;
        case 1:
            sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            break;
        case 2:
            sprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT );
            break;
        }

        point->setPosition(sprite->getPosition());

        run_grossini_dance_animation_forever(sprite);

        auto flip = std::make_unique<FlipY>(true);
        auto flip_back = std::make_unique<FlipY>(false);
        auto delay = std::make_unique<DelayTime>(1);
        auto delay_clone = std::unique_ptr<FiniteTimeAction>(delay->clone());
        auto seq = std::make_unique<Sequence>(
            std::move(delay),
            std::move(flip),
            std::move(delay_clone),
            std::move(flip_back)
        );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq)));

        addChild(sprite, 0);
    }
}

SpriteOffsetAnchorFlip::~SpriteOffsetAnchorFlip()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteOffsetAnchorFlip::title() const
{
    return "Testing Sprite";
}

std::string SpriteOffsetAnchorFlip::subtitle() const
{
    return "issue #1078: offset + anchor + flip";
}

//
// SpriteBatchNodeOffsetAnchorFlip
// 

SpriteBatchNodeOffsetAnchorFlip::SpriteBatchNodeOffsetAnchorFlip()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);

    for (int i = 0; i < 3; i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2(s.width / 4 * (i + 1), s.height / 2));

        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 200);

        switch (i)
        {
        case 0:
            sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            break;
        case 1:
            sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            break;
        case 2:
            sprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT );
            break;
        }

        point->setPosition(sprite->getPosition());        

        run_grossini_dance_animation_forever(sprite);

        auto flip = std::make_unique<FlipY>(true);
        auto flip_back = std::make_unique<FlipY>(false);
        auto delay = std::make_unique<DelayTime>(1);
        auto delay_clone = std::unique_ptr<FiniteTimeAction>(delay->clone());
        auto seq = std::make_unique<Sequence>(
            std::move(delay),
            std::move(flip),
            std::move(delay_clone),
            std::move(flip_back)
        );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq)));

        spritebatch->addChild(sprite, i);
    }
}

SpriteBatchNodeOffsetAnchorFlip::~SpriteBatchNodeOffsetAnchorFlip()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteBatchNodeOffsetAnchorFlip::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeOffsetAnchorFlip::subtitle() const
{
    return "issue #1078: offset + anchor + flip";
}


/// NodeSort

NodeSort::NodeSort()
{
    _node = Node::create();
    addChild(_node, 0, 0);

    _sprite1 = Sprite::create("Images/piece.png", Rect(128, 0, 64, 64));
    _sprite1->setPosition(Vec2(100, 160));
    _node->addChild(_sprite1, -6, 1);

    _sprite2 = Sprite::create("Images/piece.png", Rect(128, 0, 64, 64));
    _sprite2->setPosition(Vec2(164, 160));
    _node->addChild(_sprite2, -6, 2);

    _sprite4 = Sprite::create("Images/piece.png", Rect(128, 0, 64, 64));
    _sprite4->setPosition(Vec2(292, 160));
    _node->addChild(_sprite4, -3, 4);

    _sprite3 = Sprite::create("Images/piece.png", Rect(128, 0, 64, 64));
    _sprite3->setPosition(Vec2(228, 160));
    _node->addChild(_sprite3, -4, 3);

    _sprite5 = Sprite::create("Images/piece.png", Rect(128, 0, 64, 64));
    _sprite5->setPosition(Vec2(356, 160));
    _node->addChild(_sprite5, -3, 5);

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &NodeSort::reorderSprite, 0).paused(isPaused())
    );
}

std::string NodeSort::title() const
{
    return "node sort same index";
}

std::string NodeSort::subtitle() const
{
    return "tag order in console should be 2,1,3,4,5";
}

void NodeSort::reorderSprite(float)
{
    Director::getInstance()->getScheduler().unscheduleTimedJob(this, 0);

    log("Before reorder--");
    
    auto& children = _node->getChildren();
    
    for(const auto &child : children) {
        log("tag %i z %i",(int)child->getTag(),(int)child->getLocalZOrder());
    }
    //z-4
    _node->reorderChild( _node->getChildren().at(0).get(), -6);

    _node->sortAllChildren();
    
    log("After reorder--");
    for(const auto &child : children) {
        log("tag %i z %i",(int)child->getTag(),(int)child->getLocalZOrder());
    }
}

/// SpriteBatchNodeReorderSameIndex
SpriteBatchNodeReorderSameIndex::SpriteBatchNodeReorderSameIndex()
{
    _batchNode = SpriteBatchNode::create("Images/piece.png", 15);
    addChild(_batchNode, 1, 0);

    _sprite1 = Sprite::create(_batchNode->getTexture(), Rect(128,0,64,64));
    _sprite1->setPosition(Vec2(100,160));
    _batchNode->addChild(_sprite1, 3, 1);

    _sprite2= Sprite::create(_batchNode->getTexture(), Rect(128,0,64,64));
    _sprite2->setPosition(Vec2(164,160));
    _batchNode->addChild(_sprite2, 4, 2);

    _sprite3 = Sprite::create(_batchNode->getTexture(), Rect(128,0,64,64));
    _sprite3->setPosition(Vec2(228,160));
    _batchNode->addChild(_sprite3, 4, 3);

    _sprite4 = Sprite::create(_batchNode->getTexture(), Rect(128,0,64,64));
    _sprite4->setPosition(Vec2(292,160));
    _batchNode->addChild(_sprite4, 5, 4);

    _sprite5 = Sprite::create(_batchNode->getTexture(), Rect(128,0,64,64));
    _sprite5->setPosition(Vec2(356,160));
    _batchNode->addChild(_sprite5, 6, 5);


    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteBatchNodeReorderSameIndex::reorderSprite, 0)
            .delay(2.0f)
            .repeat(0)
            .paused(isPaused())
    );
}

std::string SpriteBatchNodeReorderSameIndex::title() const
{
    return "SpriteBatchNodeReorder same index";
}

std::string SpriteBatchNodeReorderSameIndex::subtitle() const
{
    return "tag order in console should be 2,3,4,5,1";
}

void SpriteBatchNodeReorderSameIndex::reorderSprite(float)
{
    _batchNode->reorderChild(_sprite4, 4);
    _batchNode->reorderChild(_sprite5, 4);
    _batchNode->reorderChild(_sprite1, 4);

    _batchNode->sortAllChildren();

    for(const auto &sprite : _batchNode->getDescendants()) {
        log("tag %i", sprite->getTag() );
    }
}

/// SpriteBatchNodeReorderOneChild
SpriteBatchNodeReorderOneChild::SpriteBatchNodeReorderOneChild()
{
    auto s = Director::getInstance()->getWinSize();

    auto & cache = _director->getSpriteFrameCache();

    cache.addSpriteFramesWithFile("animations/ghosts.plist");

    SpriteBatchNode *aParent;
    Sprite *l1, *l2a, *l2b, *l3a1, *l3a2, *l3b1, *l3b2;

    //
    // SpriteBatchNode: 3 levels of children
    //

    aParent = SpriteBatchNode::create("animations/ghosts.png");

    _batchNode = aParent;
    //[[aParent texture] generateMipmap];
    addChild(aParent);

    // parent
    l1 = Sprite::create(cache.getSpriteFrameByName("father.gif"));
    l1->setPosition(Vec2( s.width/2, s.height/2));

    aParent->addChild(l1);
    auto l1Size = l1->getContentSize();

    // child left
    l2a = Sprite::create(cache.getSpriteFrameByName("sister1.gif"));
    l2a->setPosition(Vec2( -10 + l1Size.width/2, 0 + l1Size.height/2));

    l1->addChild(l2a, 1);
    auto l2aSize = l2a->getContentSize();


    // child right
    l2b = Sprite::create(cache.getSpriteFrameByName("sister2.gif"));
    l2b->setPosition(Vec2( +50 + l1Size.width/2, 0 + l1Size.height/2));

    l1->addChild(l2b, 2);
    auto l2bSize = l2a->getContentSize();


    // child left bottom
    l3a1 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3a1->setScale(0.45f);
    l3a1->setPosition(Vec2(0+l2aSize.width/2,-50+l2aSize.height/2));
    l2a->addChild(l3a1, 1);

    // child left top
    l3a2 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3a2->setScale(0.45f);
    l3a2->setPosition(Vec2(0+l2aSize.width/2,+50+l2aSize.height/2));
    l2a->addChild(l3a2, 2);

    _reorderSprite = l2a;

    // child right bottom
    l3b1 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3b1->setScale(0.45f);
    l3b1->setFlippedY(true);
    l3b1->setPosition(Vec2(0+l2bSize.width/2,-50+l2bSize.height/2));
    l2b->addChild(l3b1);

    // child right top
    l3b2 = Sprite::create(cache.getSpriteFrameByName("child1.gif"));
    l3b2->setScale(0.45f);
    l3b2->setFlippedY(true);
    l3b2->setPosition(Vec2(0+l2bSize.width/2,+50+l2bSize.height/2));
    l2b->addChild(l3b2);

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteBatchNodeReorderOneChild::reorderSprite, 0)
            .delay(2.0f)
            .repeat(0)
            .paused(isPaused())
    );
}

void SpriteBatchNodeReorderOneChild::reorderSprite(float)
{
    _reorderSprite->getParent()->reorderChild(_reorderSprite, -1);

    _batchNode->sortAllChildren();
    //CCSprite* child;
    //CCARRAY_FOREACH(batchNode.descendants,child) NSLog(@"tag %i",child.tag);
}

std::string SpriteBatchNodeReorderOneChild::title() const
{
    return "SpriteBatchNode reorder 1 child";
}

//
// SpriteOffsetAnchorRotationalSkew
//
SpriteOffsetAnchorRotationalSkew::SpriteOffsetAnchorRotationalSkew()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    for(int i=0;i<3;i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2(s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
                            
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);
        
        switch(i)
        {
            case 0:
                sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                break;
            case 1:
                sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                break;
            case 2:
                sprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                break;
        }
        
        point->setPosition(sprite->getPosition());
        
        run_grossini_dance_animation_forever(sprite);
        
        auto skewX = std::make_unique<RotateBy>(2, 45, 0);
        auto skewX_back = std::unique_ptr<FiniteTimeAction>(skewX->reverse());
        auto skewY = std::make_unique<RotateBy>(2, 0, 45);
        auto skewY_back = std::unique_ptr<FiniteTimeAction>(skewY->reverse());
        
        auto seq_skew = std::make_unique<Sequence>(
            std::move(skewX),
            std::move(skewX_back),
            std::move(skewY),
            std::move(skewY_back)
        );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_skew)));
        
        addChild(sprite, 0);
    }
}

SpriteOffsetAnchorRotationalSkew::~SpriteOffsetAnchorRotationalSkew()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteOffsetAnchorRotationalSkew::title() const
{
    return "Testing Sprite";
}

std::string SpriteOffsetAnchorRotationalSkew::subtitle() const
{
    return "offset + anchor + rotational skew";
}

//
// SpriteBatchNodeOffsetAnchorRotationalSkew
//
SpriteBatchNodeOffsetAnchorRotationalSkew::SpriteBatchNodeOffsetAnchorRotationalSkew()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);
    
    for(int i=0;i<3;i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2(s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);
        
        switch(i)
        {
            case 0:
                sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                break;
            case 1:
                sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                break;
            case 2:
                sprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                break;
        }
        
        point->setPosition(sprite->getPosition());
        
        run_grossini_dance_animation_forever(sprite);
        
        auto skewX = std::make_unique<RotateBy>(2, 45, 0);
        auto skewX_back = std::unique_ptr<FiniteTimeAction>(skewX->reverse());
        auto skewY = std::make_unique<RotateBy>(2, 0, 45);
        auto skewY_back = std::unique_ptr<FiniteTimeAction>(skewY->reverse());
        
        auto seq_skew = std::make_unique<Sequence>(
            std::move(skewX),
            std::move(skewX_back),
            std::move(skewY),
            std::move(skewY_back)
        );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_skew)));
        
        spritebatch->addChild(sprite, 0);
    }
}

SpriteBatchNodeOffsetAnchorRotationalSkew::~SpriteBatchNodeOffsetAnchorRotationalSkew()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteBatchNodeOffsetAnchorRotationalSkew::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeOffsetAnchorRotationalSkew::subtitle() const
{
    return "offset + anchor + rotational skew";
}

//
// SpriteOffsetAnchorRotationalSkewScale
//
SpriteOffsetAnchorRotationalSkewScale::SpriteOffsetAnchorRotationalSkewScale()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    for(int i=0;i<3;i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2(s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);
        
        switch(i)
        {
            case 0:
                sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                break;
            case 1:
                sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                break;
            case 2:
                sprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                break;
        }
        
        point->setPosition(sprite->getPosition());
        
        run_grossini_dance_animation_forever(sprite);
        
        // Skew
        auto skewX = std::make_unique<RotateBy>(2, 45, 0);
        auto skewX_back = std::unique_ptr<FiniteTimeAction>(skewX->reverse());
        auto skewY = std::make_unique<RotateBy>(2, 0, 45);
        auto skewY_back = std::unique_ptr<FiniteTimeAction>(skewY->reverse());
        
        auto seq_skew = std::make_unique<Sequence>(
            std::move(skewX),
            std::move(skewX_back),
            std::move(skewY),
            std::move(skewY_back)
        );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_skew)));
        
        // Scale
        auto scale = std::make_unique<ScaleBy>(2, 2);
        auto scale_back = std::unique_ptr<FiniteTimeAction>(scale->reverse());
        auto seq_scale = std::make_unique<Sequence>( std::move(scale), std::move( scale_back) );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_scale)));
        
        addChild(sprite, 0);
    }
}

SpriteOffsetAnchorRotationalSkewScale::~SpriteOffsetAnchorRotationalSkewScale()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteOffsetAnchorRotationalSkewScale::title() const
{
    return "Testing Sprite";
}

std::string SpriteOffsetAnchorRotationalSkewScale::subtitle() const
{
    return "anchor + rot skew + scale";
}

// SpriteBatchNodeOffsetAnchorRotationalSkewScale
SpriteBatchNodeOffsetAnchorRotationalSkewScale::SpriteBatchNodeOffsetAnchorRotationalSkewScale()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto spritebatch = SpriteBatchNode::create("animations/grossini.png");
    addChild(spritebatch);
    
    for(int i=0;i<3;i++)
    {
        //
        // Animation using Sprite batch
        //
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2(s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);
        
        switch(i)
        {
            case 0:
                sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                break;
            case 1:
                sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                break;
            case 2:
                sprite->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                break;
        }
        
        point->setPosition(sprite->getPosition());
        
        run_grossini_dance_animation_forever(sprite);
        
        // Skew
        auto skewX = std::make_unique<RotateBy>(2, 45, 0);
        auto skewX_back = std::unique_ptr<FiniteTimeAction>(skewX->reverse());
        auto skewY = std::make_unique<RotateBy>(2, 0, 45);
        auto skewY_back = std::unique_ptr<FiniteTimeAction>(skewY->reverse());
        
        auto seq_skew = std::make_unique<Sequence>(
            std::move(skewX),
            std::move(skewX_back),
            std::move(skewY),
            std::move(skewY_back)
        );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_skew)));
        
        // Scale
        auto scale = std::make_unique<ScaleBy>(2, 2);
        auto scale_back = std::unique_ptr<FiniteTimeAction>(scale->reverse());
        auto seq_scale = std::make_unique<Sequence>( std::move(scale), std::move( scale_back) );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_scale)));
        
        spritebatch->addChild(sprite, 0);
    }
}

SpriteBatchNodeOffsetAnchorRotationalSkewScale::~SpriteBatchNodeOffsetAnchorRotationalSkewScale()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteBatchNodeOffsetAnchorRotationalSkewScale::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeOffsetAnchorRotationalSkewScale::subtitle() const
{
    return "anchor + rot skew + scale";
}

//
// SpriteRotationalSkewNegativeScaleChildren
//
SpriteRotationalSkewNegativeScaleChildren::SpriteRotationalSkewNegativeScaleChildren()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto parent = Node::create();
    addChild(parent);
    
    for(int i=0;i<2;i++)
    {
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2(s.width/4*(i+1), s.height/2));
        
        auto point = Sprite::create("Images/r1.png");
        
        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);
        
        // Skew
        auto skewX = std::make_unique<RotateBy>(2, 45, 0);
        auto skewX_back = std::unique_ptr<FiniteTimeAction>(skewX->reverse());
        auto skewY = std::make_unique<RotateBy>(2, 0, 45);
        auto skewY_back = std::unique_ptr<FiniteTimeAction>(skewY->reverse());
        
        if (1 == 1)
        {
            sprite->setScale(-1.0f);
        }
        
        auto seq_skew = std::make_unique<Sequence>( std::move(skewX), std::move( skewX_back), std::move( skewY), std::move( skewY_back) );
        sprite->runAction(std::make_unique<RepeatForever>(std::move(seq_skew)));
        
        auto child1 = Sprite::create("Images/grossini_dance_01.png");
        child1->setPosition(Vec2(sprite->getContentSize().width/2.0f, sprite->getContentSize().height/2.0f));
        
        sprite->addChild(child1);
        
        child1->setScale(0.8f);
        
        parent->addChild(sprite, 0);
    }
}

SpriteRotationalSkewNegativeScaleChildren::~SpriteRotationalSkewNegativeScaleChildren()
{
    auto & cache = _director->getSpriteFrameCache();
    cache.removeSpriteFramesFromFile("animations/grossini.plist");
    cache.removeSpriteFramesFromFile("animations/grossini_gray.plist");
}

std::string SpriteRotationalSkewNegativeScaleChildren::title() const
{
    return "Testing Sprite";
}

std::string SpriteRotationalSkewNegativeScaleChildren::subtitle() const
{
    return "rot skew + negative scale with children";
}

//
// SpriteBatchNodeRotationalSkewNegativeScaleChildren
//
SpriteBatchNodeRotationalSkewNegativeScaleChildren::SpriteBatchNodeRotationalSkewNegativeScaleChildren()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto & cache = _director->getSpriteFrameCache();
    cache.addSpriteFramesWithFile("animations/grossini.plist");
    cache.addSpriteFramesWithFile("animations/grossini_gray.plist", "animations/grossini_gray.png");
    
    auto parent = SpriteBatchNode::create("animations/grossini.png");
    addChild(parent);
    
    for(int i=0;i<2;i++)
    {
        auto sprite = Sprite::create(cache.getSpriteFrameByName("grossini_dance_01.png"));
        sprite->setPosition(Vec2(s.width/4*(i+1), s.height/2));

        auto point = Sprite::create("Images/r1.png");

        point->setScale(0.25f);
        point->setPosition(sprite->getPosition());
        addChild(point, 1);

        // Skew
        auto skewX = std::make_unique<RotateBy>(2, 45, 0);
        auto skewX_back = std::unique_ptr<FiniteTimeAction>(skewX->reverse());
        auto skewY = std::make_unique<RotateBy>(2, 0, 45);
        auto skewY_back = std::unique_ptr<FiniteTimeAction>(skewY->reverse());

        if (1 == 1)
        {
            sprite->setScale(-1.0f);
        }

        auto seq_skew = std::make_unique<Sequence>( std::move(skewX), std::move( skewX_back), std::move( skewY), std::move( skewY_back) );
        sprite->runAction( std::make_unique<RepeatForever>( std::move( seq_skew)));

        auto child1 = Sprite::create("Images/grossini_dance_01.png");
        child1->setPosition(Vec2(sprite->getContentSize().width/2.0f, sprite->getContentSize().height/2.0f));

        sprite->addChild(child1);

        child1->setScale(0.8f);

        parent->addChild(sprite, 0);
    }
}

SpriteBatchNodeRotationalSkewNegativeScaleChildren::~SpriteBatchNodeRotationalSkewNegativeScaleChildren()
{
    
}

std::string SpriteBatchNodeRotationalSkewNegativeScaleChildren::title() const
{
    return "Testing SpriteBatchNode";
}

std::string SpriteBatchNodeRotationalSkewNegativeScaleChildren::subtitle() const
{
    return "rot skew + negative scale with children";
}


//------------------------------------------------------------------
//
// SpriteCullTest1
//
//------------------------------------------------------------------

SpriteCullTest1::SpriteCullTest1()
{
    Size s = Director::getInstance()->getWinSize();
    auto grossini = Sprite::create("Images/grossini.png");

    grossini->setPosition(s.width/2, s.height/2);

    auto right = std::make_unique<MoveBy>(3, Vec2(s.width*2,0));
    auto back1 = std::unique_ptr<FiniteTimeAction>(right->reverse());
    auto left = std::make_unique<MoveBy>(3, Vec2(-s.width*2,0));
    auto back2 = std::unique_ptr<FiniteTimeAction>(left->reverse());
    auto up = std::make_unique<MoveBy>(3, Vec2(0,s.height*2));
    auto back3 = std::unique_ptr<FiniteTimeAction>(up->reverse());
    auto down = std::make_unique<MoveBy>(3, Vec2(0,-s.height*2));
    auto back4 = std::unique_ptr<FiniteTimeAction>(down->reverse());

    auto seq = std::make_unique<Sequence>(
        std::move(right),
        std::move(back1),
        std::move(left),
        std::move(back2),
        std::move(up),
        std::move(back3),
        std::move(down),
        std::move(back4)
    );
    grossini->runAction(std::move(seq));
    this->addChild(grossini);
}


std::string SpriteCullTest1::title() const
{
    return "Testing Culling: No Scale, No Rotation";
}

std::string SpriteCullTest1::subtitle() const
{
    return "Look at the GL calls";
}


//------------------------------------------------------------------
//
// SpriteCullTest2
//
//------------------------------------------------------------------

SpriteCullTest2::SpriteCullTest2()
{
    Size s = Director::getInstance()->getWinSize();
    auto grossini = Sprite::create("Images/grossini.png");

    grossini->setPosition(s.width/2, s.height/2);

    auto right = std::make_unique<MoveBy>(3, Vec2(s.width*2,0));
    auto back1 = std::unique_ptr<FiniteTimeAction>(right->reverse());
    auto left = std::make_unique<MoveBy>(3, Vec2(-s.width*2,0));
    auto back2 = std::unique_ptr<FiniteTimeAction>(left->reverse());
    auto up = std::make_unique<MoveBy>(3, Vec2(0,s.height*2));
    auto back3 = std::unique_ptr<FiniteTimeAction>(up->reverse());
    auto down = std::make_unique<MoveBy>(3, Vec2(0,-s.height*2));
    auto back4 = std::unique_ptr<FiniteTimeAction>(down->reverse());

    grossini->setScale(0.1f);
    
    auto seq = std::make_unique<Sequence>(
        std::move(right),
        std::move(back1),
        std::move(left),
        std::move(back2),
        std::move(up),
        std::move(back3),
        std::move(down),
        std::move(back4)
    );
    grossini->runAction(std::move(seq));
    this->addChild(grossini);
}


std::string SpriteCullTest2::title() const
{
    return "Testing Culling: Scale, No Rotation";
}

std::string SpriteCullTest2::subtitle() const
{
    return "Look at the GL calls";
}

//------------------------------------------------------------------
//
// Sprite 3D rotation test
//
//------------------------------------------------------------------
Sprite3DRotationTest::Sprite3DRotationTest()
{
    Size s = Director::getInstance()->getWinSize();
    
    //Create reference sprite that's rotating based on there anchor point
    auto s1 = Sprite::create("Images/grossini.png");
    s1->setPosition(s.width/4, s.height/4 * 3);
    s1->setAnchorPoint(Vec2(0, 0));
    s1->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(6, 360)));
    addChild(s1);
    
    auto s2 = Sprite::create("Images/grossini.png");
    s2->setPosition(s.width/4 * 3, s.height/4 * 3);
    s2->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(6, 360)));
    addChild(s2);
    
    sprite1 = Sprite::create("Images/grossini.png");
    sprite1->setPosition(s.width/4, s.height/4);
    sprite1->setAnchorPoint(Vec2(0,0));
    
    addChild(sprite1);
    
    sprite2 = Sprite::create("Images/grossini.png");
    sprite2->setPosition(s.width/4 * 3, s.height/4);
    
    addChild(sprite2);

    auto f = [&](float) {
        rotation.y += 1;
        sprite1->setRotation3D(rotation);
        sprite2->setRotation3D(rotation);
    };
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, f, 0).paused(isPaused())
    );
}

//------------------------------------------------------------------
//
// Sprite::getSpriteFrame() test
//
//------------------------------------------------------------------
SpriteGetSpriteFrameTest::SpriteGetSpriteFrameTest()
{
    Size s = Director::getInstance()->getVisibleSize();

    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini_family.plist");
    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini.plist");

    auto & cache = _director->getSpriteFrameCache();

    //Create reference sprite that's rotating based on there anchor point
    auto s1 = Sprite::create(cache.getSpriteFrameByName("grossini.png"));
    addChild(s1);
    s1->getTexture()->setAliasTexParameters();
    s1->setPosition(s.width/2-s.width/3, s.height/2);
    s1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s1->setContentSize(s1->getContentSize()*2);
    s1->setSpriteFrame(s1->getSpriteFrame());

    //Create reference sprite that's rotating based on there anchor point
    auto s2 = Sprite::create(cache.getSpriteFrameByName("grossini.png"));
    addChild(s2);
    s2->setPosition(s.width*2/4, s.height/2);
    s2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    //Create reference sprite that's rotating based on there anchor point
    auto s3 = Sprite::create(cache.getSpriteFrameByName("grossini.png"));
    addChild(s3);
    s3->setTextureRect(CC_RECT_PIXELS_TO_POINTS(Rect(128,0,64,128)));
    s3->setPosition(s.width/2+s.width/3, s.height/2);
    s3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s3->setSpriteFrame(s3->getSpriteFrame());
}

//------------------------------------------------------------------
//
// Slice9 Test #1
//
//------------------------------------------------------------------
SpriteSlice9Test1::SpriteSlice9Test1()
{
    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini_family.plist");
    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini.plist");

    Size s = Director::getInstance()->getWinSize();

    for (int i=2; i>0; --i)
    {
        auto s1 = Sprite::create("Images/grossinis_sister1.png");
        addChild(s1);
        s1->setPosition(s.width*1/4, s.height*i/3);
        s1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        s1->setContentSize(Size(s1->getContentSize().width, 200));
        s1->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(5, 360)));

        //Create reference sprite that's rotating based on there anchor point
        auto s2 = Sprite::create("Images/grossinis_sister1.png");
        addChild(s2);
        s2->setPosition(s.width*2/4, s.height*i/3);
        s2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        s2->setContentSize(Size(s2->getContentSize().width, 200));
        s2->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(5, 360)));

        //Create reference sprite that's rotating based on there anchor point
        auto s3 = Sprite::create("Images/grossinis_sister1.png");
        addChild(s3);
        s3->setPosition(s.width*3/4, s.height*i/3);
        s3->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        s3->setContentSize(Size(s3->getContentSize().width, 200));
        s3->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(5, 360)));

        if (i==2) {
            s3->setCenterRectNormalized(Rect(0.4, 0.4, 0.2, 0.2));
            s2->setCenterRectNormalized(Rect(0.4, 0.4, 0.2, 0.2));
            s1->setCenterRectNormalized(Rect(0.4, 0.4, 0.2, 0.2));
        }

        // "anchor points"
        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition( s1->getPosition() );
        addChild(point, 10);

        auto point2 = Sprite::create("Images/r1.png");
        point2->setScale(0.25f);
        point2->setPosition(s2->getPosition());
        addChild(point2, 10);

        auto point3 = Sprite::create("Images/r1.png");
        point3->setScale(0.25f);
        point3->setPosition(s3->getPosition());
        addChild(point3, 10);
    }
}

//------------------------------------------------------------------
//
// Slice9 Test #2
//
//------------------------------------------------------------------
SpriteSlice9Test2::SpriteSlice9Test2()
{
    auto & cache = _director->getSpriteFrameCache();

    cache.addSpriteFramesWithFile("animations/grossini_family.plist");
    cache.addSpriteFramesWithFile("animations/grossini.plist");

    Size s = Director::getInstance()->getWinSize();

    for (int i=2; i>0; i--)
    {
        //Create reference sprite that's rotating based on there anchor point
        auto s1 = Sprite::create(cache.getSpriteFrameByName("grossini.png"));
        addChild(s1);
        s1->setPosition(s.width*1/4, s.height*i/3);
        s1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        s1->setContentSize(Size(80, s1->getContentSize().height));
        s1->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(5, 360)));

        //Create reference sprite that's rotating based on there anchor point
        auto s2 = Sprite::create(cache.getSpriteFrameByName("grossini.png"));
        addChild(s2);
        s2->setPosition(s.width*2/4, s.height*i/3);
        s2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        s2->setContentSize(Size(80, s2->getContentSize().height));
        s2->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(5, 360)));

        //Create reference sprite that's rotating based on there anchor point
        auto s3 = Sprite::create(cache.getSpriteFrameByName("grossini.png"));
        addChild(s3);
        s3->setPosition(s.width*3/4, s.height*i/3);
        s3->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        s3->setContentSize(Size(80, s3->getContentSize().height));
        s3->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(5, 360)));

        if (i==2) {
            s3->setCenterRectNormalized(Rect(0.4, 0.4, 0.2, 0.2));
            s2->setCenterRectNormalized(Rect(0.4, 0.4, 0.2, 0.2));
            s1->setCenterRectNormalized(Rect(0.4, 0.4, 0.2, 0.2));
        }

        // "anchor points"
        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition( s1->getPosition() );
        addChild(point, 10);

        auto point2 = Sprite::create("Images/r1.png");
        point2->setScale(0.25f);
        point2->setPosition(s2->getPosition());
        addChild(point2, 10);

        auto point3 = Sprite::create("Images/r1.png");
        point3->setScale(0.25f);
        point3->setPosition(s3->getPosition());
        addChild(point3, 10);
    }
}

//------------------------------------------------------------------
//
// Slice9 Test #3
//
//------------------------------------------------------------------
SpriteSlice9Test3::SpriteSlice9Test3()
{
    Size s = Director::getInstance()->getWinSize();


    for (int i=2; i>0; --i)
    {
        //Create reference sprite that's rotating based on there anchor point
        auto s1 = Sprite::create("ccb/scale-9-demo.png");
        addChild(s1);
        s1->setPosition(s.width*1/4, s.height*i/3);
        s1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        s1->setContentSize(s1->getContentSize());
        s1->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(5, 360)));

        //Create reference sprite that's rotating based on there anchor point
        auto s2 = Sprite::create("ccb/scale-9-demo.png");
        addChild(s2);
        s2->setPosition(s.width*2/4, s.height*i/3);
        s2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        s2->setContentSize(s2->getContentSize() * 2);
        s2->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(5, 360)));

        //Create reference sprite that's rotating based on there anchor point
        auto s3 = Sprite::create("ccb/scale-9-demo.png");
        addChild(s3);
        s3->setPosition(s.width*3/4, s.height*i/3);
        s3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        s3->setContentSize(s3->getContentSize() * 3);
        s3->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(5, 360)));

        // enable slice 9, only in the first row
        if (i==2) {
            s1->setCenterRectNormalized(Rect(0.4, 0.4, 0.2, 0.2));
            s2->setCenterRectNormalized(Rect(0.4, 0.4, 0.2, 0.2));
            s3->setCenterRectNormalized(Rect(0.4, 0.4, 0.2, 0.2));
        }


        // "anchor points"
        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.25f);
        point->setPosition( s1->getPosition() );
        addChild(point, 10);

        auto point2 = Sprite::create("Images/r1.png");
        point2->setScale(0.25f);
        point2->setPosition(s2->getPosition());
        addChild(point2, 10);

        auto point3 = Sprite::create("Images/r1.png");
        point3->setScale(0.25f);
        point3->setPosition(s3->getPosition());
        addChild(point3, 10);
    }
}

//------------------------------------------------------------------
//
// Slice9 Test #4
//
//------------------------------------------------------------------
SpriteSlice9Test4::SpriteSlice9Test4()
{
    Size s = Director::getInstance()->getWinSize();


    for (int i=2; i>0; --i)
    {
        //Create reference sprite that's rotating based on there anchor point
        auto s1 = Sprite::create("ccs-res/cocosui/button.png");
        addChild(s1);
        s1->setPosition(s.width*1/4, s.height*i/3);
        s1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        s1->setContentSize(s1->getContentSize() * 2);
        s1->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(5, 360)));

        //Create reference sprite that's rotating based on there anchor point
        auto s2 = Sprite::create("ccs-res/cocosui/button.png");
        addChild(s2);
        s2->setPosition(s.width*2/4, s.height*i/3);
        s2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        s2->setContentSize(s2->getContentSize() * 3);
        s2->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(5, 360)));

        //Create reference sprite that's rotating based on there anchor point
        auto s3 = Sprite::create("ccs-res/cocosui/button.png");
        addChild(s3);
        s3->setPosition(s.width*3/4, s.height*i/3);
        s3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        s3->setContentSize(s3->getContentSize() * 4);
        s3->runAction(std::make_unique<RepeatForever>(std::make_unique<RotateBy>(5, 360)));

        // enable slice 9, only in the first row
        if (i==2) {
            s1->setCenterRect(CC_RECT_PIXELS_TO_POINTS(Rect(6, 14, 2, 4)));
            s2->setCenterRect(CC_RECT_PIXELS_TO_POINTS(Rect(6, 14, 2, 4)));
            s3->setCenterRect(CC_RECT_PIXELS_TO_POINTS(Rect(6, 14, 2, 4)));
        }

        // "anchor points"
        auto point = Sprite::create("Images/r1.png");
        point->setScale(0.1f);
        point->setPosition( s1->getPosition() );
        addChild(point, 10);

        auto point2 = Sprite::create("Images/r1.png");
        point2->setScale(0.1f);
        point2->setPosition(s2->getPosition());
        addChild(point2, 10);

        auto point3 = Sprite::create("Images/r1.png");
        point3->setScale(0.1f);
        point3->setPosition(s3->getPosition());
        addChild(point3, 10);
    }
}

//------------------------------------------------------------------
//
// Slice9 Test #5
//
//------------------------------------------------------------------
SpriteSlice9Test5::SpriteSlice9Test5()
{
    Size s = Director::getInstance()->getVisibleSize();

    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini_family.plist");
    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini.plist");


    //Create reference sprite that's rotating based on there anchor point
    auto s1 = Sprite::create("Images/grossinis_heads.png");
    addChild(s1);
    s1->getTexture()->setAliasTexParameters();
    s1->setTextureRect(CC_RECT_PIXELS_TO_POINTS(Rect(0,0,64,128)));
    s1->setPosition(s.width/2-s.width/3, s.height/2);
    s1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s1->setContentSize(Size(s.width/3, s.height));
    s1->setCenterRectNormalized(Rect(0,0,1,1));
    _sprites[0] = s1;

    //Create reference sprite that's rotating based on there anchor point
    auto s2 = Sprite::create("Images/grossinis_heads.png");
    addChild(s2);
    s2->setTextureRect(CC_RECT_PIXELS_TO_POINTS(Rect(64,0,64,128)));
    s2->setPosition(s.width*2/4, s.height/2);
    s2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s2->setContentSize(Size(s.width/3, s.height));
    s2->setCenterRectNormalized(Rect(0,0,1,1));
    _sprites[1] = s2;

    //Create reference sprite that's rotating based on there anchor point
    auto s3 = Sprite::create("Images/grossinis_heads.png");
    addChild(s3);
    s3->setTextureRect(CC_RECT_PIXELS_TO_POINTS(Rect(128,0,64,128)));
    s3->setPosition(s.width/2+s.width/3, s.height/2);
    s3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s3->setContentSize(Size(s.width/3, s.height));
    s3->setCenterRectNormalized(Rect(0,0,1,1));
    _sprites[2] = s3;

    Director::getInstance()->getScheduler().schedule( UpdateJob(this, 0).paused(isPaused()) );

    _elapsed = 0;
}

void SpriteSlice9Test5::update(float dt)
{
    _elapsed += dt;

    float angle = _elapsed;

    // cap the value between 0 and 0.8
    float x = ((cos(angle) + sin(angle*3)) + 2) / 5.0f;
    float y1 = (sin(angle) + 1) / 2.5;
    float y2 = (sin(angle+M_PI_2) + 1) / 2.5;
    float y = y1;
    for (int i=0; i<3; ++i) {
        if (i==1) {
            x = 0.8 - x;
            y = y2;
        } else if (i==2) {
            y = 0.8 - y;
        }

        Rect rect(x,y,0.2, 0.2);
        _sprites[i]->setCenterRectNormalized(rect);
    }
}

//------------------------------------------------------------------
//
// Slice9 Test #6
//
//------------------------------------------------------------------
SpriteSlice9Test6::SpriteSlice9Test6()
{
    Size s = Director::getInstance()->getVisibleSize();

    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini_family.plist");
    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini.plist");


    //Create reference sprite that's rotating based on there anchor point
    auto s1 = Sprite::create("Images/grossinis_heads.png");
    addChild(s1);
    s1->getTexture()->setAliasTexParameters();
    s1->setTextureRect(CC_RECT_PIXELS_TO_POINTS(Rect(0,0,64,128)));
    s1->setPosition(s.width/2-s.width/3, s.height/2);
    s1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s1->setContentSize(Size(s.width/3, s.height));
    s1->setCenterRectNormalized(Rect(0,0,1,1));
    _sprites[0] = s1;

    //Create reference sprite that's rotating based on there anchor point
    auto s2 = Sprite::create("Images/grossinis_heads.png");
    addChild(s2);
    s2->setTextureRect(CC_RECT_PIXELS_TO_POINTS(Rect(64,0,64,128)));
    s2->setPosition(s.width*2/4, s.height/2);
    s2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s2->setContentSize(Size(s.width/3, s.height));
    s2->setCenterRectNormalized(Rect(0,0,1,1));
    _sprites[1] = s2;

    //Create reference sprite that's rotating based on there anchor point
    auto s3 = Sprite::create("Images/grossinis_heads.png");
    addChild(s3);
    s3->setTextureRect(CC_RECT_PIXELS_TO_POINTS(Rect(128,0,64,128)));
    s3->setPosition(s.width/2+s.width/3, s.height/2);
    s3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s3->setContentSize(Size(s.width/3, s.height));
    s3->setCenterRectNormalized(Rect(0,0,1,1));
    _sprites[2] = s3;

    Director::getInstance()->getScheduler().schedule( UpdateJob(this, 0).paused(isPaused()) );

    _elapsed = 0;
}

void SpriteSlice9Test6::update(float dt)
{
    _elapsed += dt;

    float angle = _elapsed;

    // cap the value between 0 and 1
    float x = ((cos(angle*2) - sin(angle/2)) + 2) / 4;
    float y1 = (sin(angle) + 1) / 2;
    float y2 = (sin(angle+M_PI_2) + 1) / 2;
    float y = y1;
    for (int i=0; i<3; ++i) {
        if (i==1) {
            x = 1 - x;
            y = y2;
        } else if (i==2) {
            y = 1 - y;
        }

        Rect rect((1-x)/2, (1-y)/2, x, y);
        _sprites[i]->setCenterRectNormalized(rect);
    }
}

//------------------------------------------------------------------
//
// Slice9 Test #7
//
//------------------------------------------------------------------
SpriteSlice9Test7::SpriteSlice9Test7()
{
    Size s = Director::getInstance()->getVisibleSize();

    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini_family.plist");
    _director->getSpriteFrameCache().addSpriteFramesWithFile("animations/grossini.plist");


    //Create reference sprite that's rotating based on there anchor point
    auto s1 = Sprite::create("Images/grossinis_heads.png");
    addChild(s1);
    s1->getTexture()->setAliasTexParameters();
    s1->setTextureRect(CC_RECT_PIXELS_TO_POINTS(Rect(0,0,64,128)));
    s1->setPosition(s.width/2-s.width/3, s.height/2);
    s1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s1->setContentSize(Size(s.width/3, s.height));
    s1->setCenterRectNormalized(Rect(0,0,0.5,0.5));

    //Create reference sprite that's rotating based on there anchor point
    auto s2 = Sprite::create("Images/grossinis_heads.png");
    addChild(s2);
    s2->setTextureRect(CC_RECT_PIXELS_TO_POINTS(Rect(64,0,64,128)));
    s2->setPosition(s.width*2/4, s.height/2);
    s2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s2->setContentSize(Size(s.width/3, s.height));
    s2->setCenterRectNormalized(Rect(0.25,0.25,0.5,0.5));

    //Create reference sprite that's rotating based on there anchor point
    auto s3 = Sprite::create("Images/grossinis_heads.png");
    addChild(s3);
    s3->setTextureRect(CC_RECT_PIXELS_TO_POINTS(Rect(128,0,64,128)));
    s3->setPosition(s.width/2+s.width/3, s.height/2);
    s3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s3->setContentSize(Size(s.width/3, s.height));
    s3->setCenterRectNormalized(Rect(0.5,0.5,0.5,0.5));
}

//------------------------------------------------------------------
//
// Slice9 Test #8
//
//------------------------------------------------------------------
SpriteSlice9Test8::SpriteSlice9Test8()
{
    Size s = Director::getInstance()->getVisibleSize();

    auto & cache = _director->getSpriteFrameCache();

    cache.addSpriteFramesWithFile("animations/grossini_family.plist");
    cache.addSpriteFramesWithFile("animations/grossini.plist");


    //
    // flip BEFORE contentSize
    //
    auto s1 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister1.png"));
    addChild(s1);
    s1->setPosition(s.width/2-s.width/3, s.height*2/3);
    s1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s1->setCenterRectNormalized(Rect(1/3.f, 1/3.f, 1/3.f, 1/3.f));
    s1->setFlippedX(true);
    s1->setContentSize(s1->getContentSize()*2);

    auto s2 = Sprite::create(cache.getSpriteFrameByName("grossini.png"));
    addChild(s2);
    s2->setPosition(s.width*2/4, s.height*2/3);
    s2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s2->setCenterRectNormalized(Rect(1/3.f, 1/3.f, 1/3.f, 1/3.f));
    s2->setFlippedX(true);
    s2->setFlippedY(true);
    s2->setContentSize(s2->getContentSize()*2);

    //Create reference sprite that's rotating based on there anchor point
    auto s3 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister2.png"));
    addChild(s3);
    s3->setPosition(s.width/2+s.width/3, s.height*2/3);
    s3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s3->setCenterRectNormalized(Rect(1/3.f, 1/3.f, 1/3.f, 1/3.f));
    s3->setFlippedY(true);
    s3->setContentSize(s3->getContentSize()*2);

    //
    // flip AFTER contentSize
    //
    auto s4 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister1.png"));
    addChild(s4);
    s4->setPosition(s.width/2-s.width/3, s.height*1/3);
    s4->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s4->setCenterRectNormalized(Rect(1/3.f, 1/3.f, 1/3.f, 1/3.f));
    s4->setFlippedX(true);
    s4->setContentSize(s4->getContentSize()*2);

    auto s5 = Sprite::create(cache.getSpriteFrameByName("grossini.png"));
    addChild(s5);
    s5->setPosition(s.width*2/4, s.height*1/3);
    s5->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s5->setCenterRectNormalized(Rect(1/3.f, 1/3.f, 1/3.f, 1/3.f));
    s5->setFlippedX(true);
    s5->setFlippedY(true);
    s5->setContentSize(s5->getContentSize()*2);

    //Create reference sprite that's rotating based on there anchor point
    auto s6 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister2.png"));
    addChild(s6);
    s6->setPosition(s.width/2+s.width/3, s.height*1/3);
    s6->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s6->setCenterRectNormalized(Rect(1/3.f, 1/3.f, 1/3.f, 1/3.f));
    s6->setFlippedY(true);
    s6->setContentSize(s6->getContentSize()*2);
}

//------------------------------------------------------------------
//
// Slice9 Test #9
//
//------------------------------------------------------------------
SpriteSlice9Test9::SpriteSlice9Test9()
{
    Size s = Director::getInstance()->getVisibleSize();

    auto & cache = _director->getSpriteFrameCache();

    cache.addSpriteFramesWithFile("animations/grossini_family.plist");
    cache.addSpriteFramesWithFile("animations/grossini.plist");


    // flipped BEFORE content size
    auto s1 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister1.png"));
    addChild(s1);
    s1->setPosition(s.width/2-s.width/3, s.height*2/3);
    s1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s1->setCenterRectNormalized(Rect(2/3.f, 2/3.f, 1/3.f, 1/3.f));
    s1->setFlippedX(true);
    s1->setContentSize(s1->getContentSize()*2);

    auto s2 = Sprite::create(cache.getSpriteFrameByName("grossini.png"));
    addChild(s2);
    s2->setPosition(s.width*2/4, s.height*2/3);
    s2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s2->setCenterRectNormalized(Rect(1/3.f, 1/3.f, 2/3.f, 2/3.f));
    s2->setFlippedX(true);
    s2->setFlippedY(true);
    s2->setContentSize(s2->getContentSize()*2);

    auto s3 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister2.png"));
    addChild(s3);
    s3->setPosition(s.width/2+s.width/3, s.height*2/3);
    s3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s3->setCenterRectNormalized(Rect(0.1f, 0.1f, 0.8f, 0.8f));
    s3->setFlippedY(true);
    s3->setContentSize(s3->getContentSize()*2);


    // flipped AFTER content size
    auto s4 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister1.png"));
    addChild(s4);
    s4->setPosition(s.width/2-s.width/3, s.height*1/3);
    s4->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s4->setCenterRectNormalized(Rect(2/3.f, 2/3.f, 1/3.f, 1/3.f));
    s4->setContentSize(s4->getContentSize()*2);
    s4->setFlippedX(true);

    auto s5 = Sprite::create(cache.getSpriteFrameByName("grossini.png"));
    addChild(s5);
    s5->setPosition(s.width*2/4, s.height*1/3);
    s5->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s5->setCenterRectNormalized(Rect(1/3.f, 1/3.f, 2/3.f, 2/3.f));
    s5->setContentSize(s5->getContentSize()*2);
    s5->setFlippedX(true);
    s5->setFlippedY(true);

    auto s6 = Sprite::create(cache.getSpriteFrameByName("grossinis_sister2.png"));
    addChild(s6);
    s6->setPosition(s.width/2+s.width/3, s.height*1/3);
    s6->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s6->setCenterRectNormalized(Rect(0.1f, 0.1f, 0.8f, 0.8f));
    s6->setContentSize(s6->getContentSize()*2);
    s6->setFlippedY(true);
}

//------------------------------------------------------------------
//
// Slice9 Test #10
//
//------------------------------------------------------------------
SpriteSlice9Test10::SpriteSlice9Test10()
{
    Size s = Director::getInstance()->getVisibleSize();

    auto & cache = _director->getSpriteFrameCache();

    cache.addSpriteFramesWithFile("Images/blocks9ss.plist");

    auto s1 = Sprite::create(cache.getSpriteFrameByName("blocks9r.png"));
    addChild(s1);
    s1->setPosition(s.width/2-s.width/3, s.height/2);
    s1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s1->setCenterRectNormalized(Rect(1/3.f, 1/3.f, 1/3.f, 1/3.f));
    s1->setContentSize(s1->getContentSize()*1.5);
    s1->setFlippedX(true);

    auto s2 = Sprite::create(cache.getSpriteFrameByName("blocks9r.png"));
    addChild(s2);
    s2->setPosition(s.width*2/4, s.height/2);
    s2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s2->setCenterRectNormalized(Rect(1/3.f, 1/3.f, 1/3.f, 1/3.f));
    s2->setContentSize(s2->getContentSize()*1.5);

    //Create reference sprite that's rotating based on there anchor point
    auto s3 = Sprite::create(cache.getSpriteFrameByName("blocks9r.png"));
    addChild(s3);
    s3->setPosition(s.width/2+s.width/3, s.height/2);
    s3->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    s3->setCenterRectNormalized(Rect(1/3.f, 1/3.f, 1/3.f, 1/3.f));
    s3->setContentSize(s3->getContentSize()*1.5);
    s3->setFlippedY(true);
}

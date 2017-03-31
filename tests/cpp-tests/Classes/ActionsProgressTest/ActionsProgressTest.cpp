/****************************************************************************
 Copyright (c) 2012 cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.

 http://www.cocos2d-x.org

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

#include "ActionsProgressTest.h"
#include "../testResource.h"

#include "2d/CCActionProgressTimer.h"
#include "2d/CCLabel.h"
#include "2d/CCProgressTimer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "base/CCDirector.h"

using namespace cocos2d;

ActionsProgressTests::ActionsProgressTests()
{
    ADD_TEST_CASE(SpriteProgressToRadial);
    ADD_TEST_CASE(SpriteProgressToHorizontal);
    ADD_TEST_CASE(SpriteProgressToVertical);
    ADD_TEST_CASE(SpriteProgressToRadialMidpointChanged);
    ADD_TEST_CASE(SpriteProgressBarVarious);
    ADD_TEST_CASE(SpriteProgressBarTintAndFade);
    ADD_TEST_CASE(SpriteProgressWithSpriteFrame);
}

//------------------------------------------------------------------
//
// SpriteDemo
//
//------------------------------------------------------------------
SpriteDemo::SpriteDemo()
{
}

SpriteDemo::~SpriteDemo()
{
}

std::string SpriteDemo::title() const
{
    return "ActionsProgressTest";
}

void SpriteDemo::onEnter()
{
    TestCase::onEnter();

    auto background = LayerColor::create(Color4B(255,0,0,255));
    addChild(background, -10);
}

//------------------------------------------------------------------
//
// SpriteProgressToRadial
//
//------------------------------------------------------------------
void SpriteProgressToRadial::onEnter()
{
    SpriteDemo::onEnter();
    
    auto s = Director::getInstance()->getWinSize();

    auto to1 = std::make_unique<Sequence>(std::make_unique<ProgressTo>(2, 100), std::make_unique<ProgressTo>(0, 0));
    auto to2 = std::make_unique<Sequence>(std::make_unique<ProgressTo>(2, 100), std::make_unique<ProgressTo>(0, 0));

    auto left = ProgressTimer::create(Sprite::create(s_pathSister1));
    left->setType( ProgressTimer::Type::RADIAL );
    addChild(left);
    left->setPosition(100, s.height/2);
    left->runAction( std::make_unique<RepeatForever>( std::move(to1)));
    
    auto right = ProgressTimer::create(Sprite::create(s_pathBlock));
    right->setType(ProgressTimer::Type::RADIAL);
    // Makes the ridial CCW
    right->setReverseDirection(true);
    addChild(right);
    right->setPosition(s.width-100, s.height/2);
    right->runAction( std::make_unique<RepeatForever>( std::move(to2)));
}

std::string SpriteProgressToRadial::subtitle() const
{
    return "ProgressTo Radial";
}

//------------------------------------------------------------------
//
// SpriteProgressToHorizontal
//
//------------------------------------------------------------------

void SpriteProgressToHorizontal::onEnter()
{
    SpriteDemo::onEnter();
    
    auto s = Director::getInstance()->getWinSize();
    
    auto to1 = std::make_unique<Sequence>(std::make_unique<ProgressTo>(2, 100), std::make_unique<ProgressTo>(0, 0));
    auto to2 = std::make_unique<Sequence>(std::make_unique<ProgressTo>(2, 100), std::make_unique<ProgressTo>(0, 0));

    auto left = ProgressTimer::create(Sprite::create(s_pathSister1));
    left->setType(ProgressTimer::Type::BAR);
    //    Setup for a bar starting from the left since the midpoint is 0 for the x
    left->setMidpoint(Vec2(0,0));
    //    Setup for a horizontal bar since the bar change rate is 0 for y meaning no vertical change
    left->setBarChangeRate(Vec2(1, 0));
    addChild(left);
    left->setPosition(100, s.height/2);
    left->runAction( std::make_unique<RepeatForever>( std::move(to1)));
    
    auto right = ProgressTimer::create(Sprite::create(s_pathSister2));
    right->setType(ProgressTimer::Type::BAR);
    //    Setup for a bar starting from the left since the midpoint is 1 for the x
    right->setMidpoint(Vec2(1, 0));
    //    Setup for a horizontal bar since the bar change rate is 0 for y meaning no vertical change
    right->setBarChangeRate(Vec2(1, 0));
    addChild(right);
    right->setPosition(s.width-100, s.height/2);
    right->runAction( std::make_unique<RepeatForever>( std::move(to2)));
}

std::string SpriteProgressToHorizontal::subtitle() const
{
    return "ProgressTo Horizontal";
}

//------------------------------------------------------------------
//
// SpriteProgressToVertical
//
//------------------------------------------------------------------
void SpriteProgressToVertical::onEnter()
{
    SpriteDemo::onEnter();
    
    auto s = Director::getInstance()->getWinSize();
    
    auto to1 = std::make_unique<Sequence>(std::make_unique<ProgressTo>(2, 100), std::make_unique<ProgressTo>(0, 0));
    auto to2 = std::make_unique<Sequence>(std::make_unique<ProgressTo>(2, 100), std::make_unique<ProgressTo>(0, 0));

    auto left = ProgressTimer::create(Sprite::create(s_pathSister1));
    left->setType(ProgressTimer::Type::BAR);

    //    Setup for a bar starting from the bottom since the midpoint is 0 for the y
    left->setMidpoint(Vec2(0,0));
    //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
    left->setBarChangeRate(Vec2(0, 1));
    addChild(left);
    left->setPosition(100, s.height/2);
    left->runAction( std::make_unique<RepeatForever>( std::move(to1)));
    
    auto right = ProgressTimer::create(Sprite::create(s_pathSister2));
    right->setType(ProgressTimer::Type::BAR);
    //    Setup for a bar starting from the bottom since the midpoint is 0 for the y
    right->setMidpoint(Vec2(0, 1));
    //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
    right->setBarChangeRate(Vec2(0, 1));
    addChild(right);
    right->setPosition(s.width-100, s.height/2);
    right->runAction( std::make_unique<RepeatForever>( std::move(to2)));
}

std::string SpriteProgressToVertical::subtitle() const
{
    return "ProgressTo Vertical";
}

//------------------------------------------------------------------
//
// SpriteProgressToRadialMidpointChanged
//
//------------------------------------------------------------------
void SpriteProgressToRadialMidpointChanged::onEnter()
{
    SpriteDemo::onEnter();

    auto s = Director::getInstance()->getWinSize();

    auto action = std::make_unique<Sequence>(std::make_unique<ProgressTo>(2, 100), std::make_unique<ProgressTo>(0, 0));

    /**
   *  Our image on the left should be a radial progress indicator, clockwise
   */
    auto left = ProgressTimer::create(Sprite::create(s_pathBlock));
    left->setType(ProgressTimer::Type::RADIAL);
    addChild(left);
    left->setMidpoint(Vec2(0.25f, 0.75f));
    left->setPosition(100, s.height/2);
    left->runAction( std::make_unique<RepeatForever>( std::unique_ptr<Sequence>(action->clone())));

    /**
   *  Our image on the left should be a radial progress indicator, counter clockwise
   */
    auto right = ProgressTimer::create(Sprite::create(s_pathBlock));
    right->setType(ProgressTimer::Type::RADIAL);
    right->setMidpoint(Vec2(0.75f, 0.25f));

    /**
   *  Note the reverse property (default=NO) is only added to the right image. That's how
   *  we get a counter clockwise progress.
   */
    addChild(right);
    right->setPosition(s.width-100, s.height/2);
    right->runAction( std::make_unique<RepeatForever>( std::move(action)));
}

std::string SpriteProgressToRadialMidpointChanged::subtitle() const
{
    return "Radial w/ Different Midpoints";
}

//------------------------------------------------------------------
//
// SpriteProgressBarVarious
//
//------------------------------------------------------------------
void SpriteProgressBarVarious::onEnter()
{
    SpriteDemo::onEnter();

    auto s = Director::getInstance()->getWinSize();

    auto to = std::make_unique<Sequence>(std::make_unique<ProgressTo>(2, 100), std::make_unique<ProgressTo>(0, 0));

    auto left = ProgressTimer::create(Sprite::create(s_pathSister1));
    left->setType(ProgressTimer::Type::BAR);

    //    Setup for a bar starting from the bottom since the midpoint is 0 for the y
    left->setMidpoint(Vec2(0.5f, 0.5f));
    //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
    left->setBarChangeRate(Vec2(1, 0));
    addChild(left);
    left->setPosition(100, s.height/2);
    left->runAction( std::make_unique<RepeatForever>( std::unique_ptr<Sequence>(to->clone())));

    auto middle = ProgressTimer::create(Sprite::create(s_pathSister2));
    middle->setType(ProgressTimer::Type::BAR);
    //    Setup for a bar starting from the bottom since the midpoint is 0 for the y
    middle->setMidpoint(Vec2(0.5f, 0.5f));
    //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
    middle->setBarChangeRate(Vec2(1,1));
    addChild(middle);
    middle->setPosition(s.width/2, s.height/2);
    middle->runAction( std::make_unique<RepeatForever>( std::unique_ptr<Sequence>(to->clone())));

    auto right = ProgressTimer::create(Sprite::create(s_pathSister2));
    right->setType(ProgressTimer::Type::BAR);
    //    Setup for a bar starting from the bottom since the midpoint is 0 for the y
    right->setMidpoint(Vec2(0.5f, 0.5f));
    //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
    right->setBarChangeRate(Vec2(0, 1));
    addChild(right);
    right->setPosition(s.width-100, s.height/2);
    right->runAction( std::make_unique<RepeatForever>( std::unique_ptr<Sequence>(to->clone())));
}

std::string SpriteProgressBarVarious::subtitle() const
{
    return "ProgressTo Bar Mid";
}

//------------------------------------------------------------------
//
// SpriteProgressBarTintAndFade
//
//------------------------------------------------------------------
void SpriteProgressBarTintAndFade::onEnter()
{
    SpriteDemo::onEnter();

    auto s = Director::getInstance()->getWinSize();

    auto to = std::make_unique<Sequence>(std::make_unique<ProgressTo>(6, 100), std::make_unique<ProgressTo>(0, 0));

	auto tint = std::make_unique<Sequence>(
        std::make_unique<TintTo>(1, 255, 0, 0),
        std::make_unique<TintTo>(1, 0, 255, 0),
        std::make_unique<TintTo>(1, 0, 0, 255)
    );
	auto fade = std::make_unique<Sequence>(
        std::make_unique<FadeTo>(1.0f, 0),
        std::make_unique<FadeTo>(1.0f, 255)
    );

    auto left = ProgressTimer::create(Sprite::create(s_pathSister1));
    left->setType(ProgressTimer::Type::BAR);

    //    Setup for a bar starting from the bottom since the midpoint is 0 for the y
    left->setMidpoint(Vec2(0.5f, 0.5f));
    //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
    left->setBarChangeRate(Vec2(1, 0));
    addChild(left);
    left->setPosition(100, s.height/2);
    left->runAction(std::make_unique<RepeatForever>( std::unique_ptr<Sequence>( to->clone())));
    left->runAction(std::make_unique<RepeatForever>( std::unique_ptr<Sequence>( tint->clone())));

    left->addChild(Label::createWithTTF("Tint", "fonts/Marker Felt.ttf", 20.0f));

    auto middle = ProgressTimer::create(Sprite::create(s_pathSister2));
    middle->setType(ProgressTimer::Type::BAR);
    //    Setup for a bar starting from the bottom since the midpoint is 0 for the y
    middle->setMidpoint(Vec2(0.5f, 0.5f));
    //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
    middle->setBarChangeRate(Vec2(1, 1));
    addChild(middle);
    middle->setPosition(s.width/2, s.height/2);
    middle->runAction(std::make_unique<RepeatForever>( std::unique_ptr<Sequence>( to->clone())));
    middle->runAction(std::make_unique<RepeatForever>( std::unique_ptr<Sequence>( fade->clone())));

    middle->addChild(Label::createWithTTF("Fade", "fonts/Marker Felt.ttf", 20.0f));

    auto right = ProgressTimer::create(Sprite::create(s_pathSister2));
    right->setType(ProgressTimer::Type::BAR);
    //    Setup for a bar starting from the bottom since the midpoint is 0 for the y
    right->setMidpoint(Vec2(0.5f, 0.5f));
    //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
    right->setBarChangeRate(Vec2(0, 1));
    addChild(right);
    right->setPosition(s.width-100, s.height/2);
    right->runAction(std::make_unique<RepeatForever>( std::unique_ptr<Sequence>( to->clone())));
	right->runAction(std::make_unique<RepeatForever>( std::unique_ptr<Sequence>( tint->clone())));
    right->runAction(std::make_unique<RepeatForever>( std::unique_ptr<Sequence>( fade->clone())));

    right->addChild(Label::createWithTTF("Tint and Fade", "fonts/Marker Felt.ttf", 20.0f));
}

std::string SpriteProgressBarTintAndFade::subtitle() const
{
    return "ProgressTo Bar Mid";
}

//------------------------------------------------------------------
//
// SpriteProgressWithSpriteFrame
//
//------------------------------------------------------------------
void SpriteProgressWithSpriteFrame::onEnter()
{
    SpriteDemo::onEnter();

    auto s = Director::getInstance()->getWinSize();

    auto to = std::make_unique<Sequence>(std::make_unique<ProgressTo>(6, 100), std::make_unique<ProgressTo>(0, 0));

    auto cache = SpriteFrameCache::getInstance();

    cache->addSpriteFramesWithFile("zwoptex/grossini.plist");

    auto left = ProgressTimer::create(Sprite::create(cache->getSpriteFrameByName("grossini_dance_01.png")));
    left->setType(ProgressTimer::Type::BAR);
    //    Setup for a bar starting from the bottom since the midpoint is 0 for the y
    left->setMidpoint(Vec2(0.5f, 0.5f));
    //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
    left->setBarChangeRate(Vec2(1, 0));
    addChild(left);
    left->setPosition(100, s.height/2);
    left->runAction(std::make_unique<RepeatForever>( std::unique_ptr<Sequence>( to->clone())));

    auto middle = ProgressTimer::create(Sprite::create(cache->getSpriteFrameByName("grossini_dance_02.png")));
    middle->setType(ProgressTimer::Type::BAR);
    //    Setup for a bar starting from the bottom since the midpoint is 0 for the y
    middle->setMidpoint(Vec2(0.5f, 0.5f));
    //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
    middle->setBarChangeRate(Vec2(1, 1));
    addChild(middle);
    middle->setPosition(s.width/2, s.height/2);
    middle->runAction(std::make_unique<RepeatForever>( std::unique_ptr<Sequence>( to->clone())));

    auto right = ProgressTimer::create(Sprite::create(cache->getSpriteFrameByName("grossini_dance_03.png")));
    right->setType(ProgressTimer::Type::RADIAL);
    //    Setup for a bar starting from the bottom since the midpoint is 0 for the y
    right->setMidpoint(Vec2(0.5f, 0.5f));
    //    Setup for a vertical bar since the bar change rate is 0 for x meaning no horizontal change
    right->setBarChangeRate(Vec2(0, 1));
    addChild(right);
    right->setPosition(s.width-100, s.height/2);
    right->runAction(std::make_unique<RepeatForever>( std::unique_ptr<Sequence>( to->clone())));
}

std::string SpriteProgressWithSpriteFrame::subtitle() const
{
    return "Progress With Sprite Frame";
}

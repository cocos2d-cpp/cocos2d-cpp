/****************************************************************************
 Copyright (c) 2017 Iakov Sergeev <yahont@github>

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

#include "ActionManagerTest.h"
#include "../testResource.h"

#include "2d/CCActionInstant.h"
#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "renderer/CCTextureAtlas.h"

using namespace cocos2d;

enum 
{
    kTagNode,
    kTagGrossini,
    kTagSequence,
}; 

ActionManagerTests::ActionManagerTests()
{
    ADD_TEST_CASE(CrashTest);
    ADD_TEST_CASE(LogicTest);
    ADD_TEST_CASE(PauseTest);
    ADD_TEST_CASE(StopActionTest);
    ADD_TEST_CASE(StopAllActionsTest);
    ADD_TEST_CASE(StopActionsByFlagsTest);
    ADD_TEST_CASE(ResumeTest);
    ADD_TEST_CASE(Issue14050Test);
}

//------------------------------------------------------------------
//
// ActionManagerTest
//
//------------------------------------------------------------------

ActionManagerTest::ActionManagerTest(void)
{
}

ActionManagerTest::~ActionManagerTest(void)
{
}

std::string ActionManagerTest::title() const
{
    return "ActionManager Test";
}
std::string ActionManagerTest::subtitle() const
{
    return "No title";
}

//------------------------------------------------------------------
//
// Test1
//
//------------------------------------------------------------------

void CrashTest::onEnter()
{
    ActionManagerTest::onEnter();

    auto child = Sprite::create(s_pathGrossini);
    child->setPosition( VisibleRect::center() );
    addChild(child, 1, kTagGrossini);

    //Sum of all action's duration is 1.5 second.
    child->runAction( std::make_unique<RotateBy>(1.5f, 90));
    child->runAction
    (
        std::make_unique<Sequence>
        (
            std::make_unique<DelayTime>(1.4f),
            std::make_unique<FadeOut>(1.1f)
        )
    );

    //After 1.5 second, self will be removed.
    child->runAction
    (
        std::make_unique<Sequence>
        (
            std::make_unique<DelayTime>(1.4f),
            std::make_unique<CallFunc>( CC_CALLBACK_0(CrashTest::removeThis, this))
        )
    );
}

void CrashTest::removeThis()
{
    auto child = getChildByTag(kTagGrossini);
    child->removeChild(child, true);
    
    getTestSuite()->enterNextTest();
}

std::string CrashTest::subtitle() const
{
    return "Test 1. Should not crash";
}

//------------------------------------------------------------------
//
// Test2
//
//------------------------------------------------------------------
void LogicTest::onEnter()
{
    ActionManagerTest::onEnter();

    auto grossini = Sprite::create(s_pathGrossini);
    addChild(grossini, 0, 2);
    grossini->setPosition(VisibleRect::center());

    grossini->runAction
    (
        std::make_unique<Sequence>
        ( 
            std::make_unique<MoveBy>(1, Vec2(150,0)),
            std::make_unique<CallFuncN>( CC_CALLBACK_1(LogicTest::bugMe, this))
        ) 
    );
}

void LogicTest::bugMe(Node* node)
{
    node->stopAllActions(); //After this stop next action not working, if remove this stop everything is working
    node->runAction( std::make_unique<ScaleTo>(2, 2));
}

std::string LogicTest::subtitle() const
{
    return "Logic test"; 
}

//------------------------------------------------------------------
//
// PauseTest
//
//------------------------------------------------------------------

void PauseTest::onEnter()
{
    //
    // This test MUST be done in 'onEnter' and not on 'init'
    // otherwise the paused action will be resumed at 'onEnter' time
    //
    ActionManagerTest::onEnter();
    

    auto l = Label::createWithTTF("After 3 seconds grossini should move", "fonts/Thonburi.ttf", 16.0f);
    addChild(l);
    l->setPosition(VisibleRect::center().x, VisibleRect::top().y-75);
    
    
    //
    // Also, this test MUST be done, after [super onEnter]
    //
    auto grossini = Sprite::create(s_pathGrossini);
    addChild(grossini, 0, kTagGrossini);
    grossini->setPosition(VisibleRect::center() );
    
    grossini->runAction( std::make_unique<MoveBy>(1, Vec2(150,0)));

    grossini->pause();

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &PauseTest::unpause)
            .repeat(0)
            .delay(3.0f)
            .paused(isPaused())
    );
}

void PauseTest::unpause(float /*dt*/)
{
    auto node = getChildByTag( kTagGrossini );
    node->resume();
}

std::string PauseTest::subtitle() const
{
    return "Pause Test";
}

//------------------------------------------------------------------
//
// RemoveTest
//
//------------------------------------------------------------------
void StopActionTest::onEnter()
{
    ActionManagerTest::onEnter();

    auto l = Label::createWithTTF("Should not crash", "fonts/Thonburi.ttf", 16.0f);
    addChild(l);
    l->setPosition(VisibleRect::center().x, VisibleRect::top().y - 75);

    auto pSequence = std::make_unique<Sequence>
        (
            std::make_unique<MoveBy>(2, Vec2(200, 0)),
            std::make_unique<CallFunc>(CC_CALLBACK_0(StopActionTest::stopAction,this))
        );
    pSequence->setTag(kTagSequence);

    auto pChild = Sprite::create(s_pathGrossini);
    pChild->setPosition( VisibleRect::center() );

    addChild(pChild, 1, kTagGrossini);
    pChild->runAction( std::move( pSequence));
}

void StopActionTest::stopAction()
{
    auto sprite = getChildByTag(kTagGrossini);
    sprite->stopActionByTag(kTagSequence);
}

std::string StopActionTest::subtitle() const
{
    return "Stop Action Test";
}

//------------------------------------------------------------------
//
// RemoveTest
//
//------------------------------------------------------------------
void StopAllActionsTest::onEnter()
{
    ActionManagerTest::onEnter();
    
    auto l = Label::createWithTTF("Should stop scale & move after 4 seconds but keep rotate", "fonts/Thonburi.ttf", 16.0f);
    addChild(l);
    l->setPosition( Vec2(VisibleRect::center().x, VisibleRect::top().y - 75) );
    
    auto pRepeatMove = std::make_unique<RepeatForever>
        (
            std::make_unique<Sequence>
            (
                std::make_unique<MoveBy>(2, Vec2(200, 0)),
                std::make_unique<MoveBy>(2, Vec2(-200, 0))
            )
        );

    pRepeatMove->setTag(kTagSequence);
    
    auto pRepeatScale = std::make_unique<RepeatForever>
        (
            std::make_unique<Sequence>
            (
                std::make_unique<ScaleBy>(2, 1.5f),
                std::make_unique<ScaleBy>(2, 1.0f / 1.5f)
            )
        );

    pRepeatScale->setTag(kTagSequence);
    
    auto pRepeatRotate = std::make_unique<RepeatForever>( std::make_unique<RotateBy>(2, 360));
    
    auto pChild = Sprite::create(s_pathGrossini);
    pChild->setPosition( VisibleRect::center() );
    
    addChild(pChild, 1, kTagGrossini);

    pChild->runAction( std::move( pRepeatMove));
    pChild->runAction( std::move( pRepeatScale));
    pChild->runAction( std::move( pRepeatRotate));
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &StopAllActionsTest::stopAction)
            .repeat(0)
            .delay(4)
            .paused(isPaused())
    );
}

void StopAllActionsTest::stopAction(float /*time*/)
{
    auto sprite = getChildByTag(kTagGrossini);
    sprite->stopAllActionsByTag(kTagSequence);
}

std::string StopAllActionsTest::subtitle() const
{
    return "Stop All Action Test";
}


//------------------------------------------------------------------
//
// ResumeTest
//
//------------------------------------------------------------------
std::string ResumeTest::subtitle() const
{
    return "Resume Test";
}

void ResumeTest::onEnter()
{
    ActionManagerTest::onEnter();

    auto l = Label::createWithTTF("Grossini only rotate/scale in 3 seconds", "fonts/Thonburi.ttf", 16.0f);
    addChild(l);
    l->setPosition(VisibleRect::center().x, VisibleRect::top().y - 75);

    auto pGrossini = Sprite::create(s_pathGrossini);
    addChild(pGrossini, 0, kTagGrossini);
    pGrossini->setPosition(VisibleRect::center());

    pGrossini->runAction( std::make_unique<ScaleBy>(2, 2));

    pGrossini->pause();
    pGrossini->runAction( std::make_unique<RotateBy>(2, 360));

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &ResumeTest::resumeGrossini)
            .delay(3.0f)
            .repeat(0)
            .paused(isPaused())
    );
}

void ResumeTest::resumeGrossini(float /*time*/)
{
    auto pGrossini = getChildByTag(kTagGrossini);
    pGrossini->resume();
}

//------------------------------------------------------------------
//
// StopActionsByFlagsTest
//
//------------------------------------------------------------------
void StopActionsByFlagsTest::onEnter()
{
    ActionManagerTest::onEnter();

    auto l = Label::createWithTTF("Should stop scale & move after 4 seconds but keep rotate", "fonts/Thonburi.ttf", 16.0f);
    addChild(l);
    l->setPosition( Vec2(VisibleRect::center().x, VisibleRect::top().y - 75) );

    auto pRepeatMove = std::make_unique<RepeatForever>
        (
            std::make_unique<Sequence>
            (
                std::make_unique<MoveBy>(2, Vec2(200, 0)),
                std::make_unique<MoveBy>(2, Vec2(-200, 0))
            )
        );

    pRepeatMove->setFlags(kMoveFlag | kRepeatForeverFlag);

    auto pRepeatScale = std::make_unique<RepeatForever>
        (
            std::make_unique<Sequence>
            (
                std::make_unique<ScaleBy>(2, 1.5f),
                std::make_unique<ScaleBy>(2, 1.0f / 1.5f)
            )
        );

    pRepeatScale->setFlags(kScaleFlag | kRepeatForeverFlag);

    auto pRepeatRotate = std::make_unique<RepeatForever>( std::make_unique<RotateBy>(2, 360));
    pRepeatRotate->setFlags(kRotateFlag | kRepeatForeverFlag);

    auto pChild = Sprite::create(s_pathGrossini);
    pChild->setPosition( VisibleRect::center() );

    addChild(pChild, 1, kTagGrossini);

    pChild->runAction( std::move( pRepeatMove));
    pChild->runAction( std::move( pRepeatScale));
    pChild->runAction( std::move( pRepeatRotate));

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &StopActionsByFlagsTest::stopAction)
            .delay(4)
            .repeat(0)
            .paused(isPaused())
    );
}

void StopActionsByFlagsTest::stopAction(float /*time*/)
{
    auto sprite = getChildByTag(kTagGrossini);
    sprite->stopActionsByFlags(kMoveFlag | kScaleFlag);
}

std::string StopActionsByFlagsTest::subtitle() const
{
    return "Stop All Actions By Flags Test";
}

//------------------------------------------------------------------
//
// Issue14050Test
//
//------------------------------------------------------------------
class SpriteIssue14050: public Sprite
{
private:
    SpriteIssue14050()
    {
        log("SpriteIssue14050::constructor");
    }
    virtual ~SpriteIssue14050()
    {
        log("SpriteIssue14050::destructor");
    }
public:
    static SpriteIssue14050* create(const std::string & s)
    {
        auto sprite = new (std::nothrow) SpriteIssue14050;
        sprite->initWithFile(s);
        sprite->autorelease();
        return sprite;
    }
};

void Issue14050Test::onEnter()
{
    ActionManagerTest::onEnter();
    
    auto sprite = SpriteIssue14050::create("Images/grossini.png");

    sprite->runAction( std::make_unique<MoveBy>(2, Vec2(100, 100)));
}

std::string Issue14050Test::subtitle() const
{
    return "Issue14050. Sprite should not leak.";
}

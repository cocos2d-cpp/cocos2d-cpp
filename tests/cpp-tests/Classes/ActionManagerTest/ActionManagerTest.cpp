#include "ActionManagerTest.h"
#include "../testResource.h"

#include "2d/CCActionInstant.h"
#include "2d/CCActionManager.h"
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
    child->runAction(RotateBy::create(1.5f, 90));
    child->runAction(
        Sequence::create(
            to_action_ptr(DelayTime::create(1.4f)),
            to_action_ptr(FadeOut::create(1.1f))
        )
    );

    //After 1.5 second, self will be removed.
    child->runAction(
        Sequence::create(
            to_action_ptr(DelayTime::create(1.4f)),
            to_action_ptr(CallFunc::create( CC_CALLBACK_0(CrashTest::removeThis,this)))
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

    grossini->runAction(
        Sequence::create( 
            to_action_ptr(MoveBy::create(1, Vec2(150,0))),
            to_action_ptr(CallFuncN::create(CC_CALLBACK_1(LogicTest::bugMe,this)))
        ) 
    );
}

void LogicTest::bugMe(Node* node)
{
    node->stopAllActions(); //After this stop next action not working, if remove this stop everything is working
    node->runAction(ScaleTo::create(2, 2));
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
    

    auto l = Label::createWithTTF("After 5 seconds grossini should move", "fonts/Thonburi.ttf", 16.0f);
    addChild(l);
    l->setPosition(VisibleRect::center().x, VisibleRect::top().y-75);
    
    
    //
    // Also, this test MUST be done, after [super onEnter]
    //
    auto grossini = Sprite::create(s_pathGrossini);
    addChild(grossini, 0, kTagGrossini);
    grossini->setPosition(VisibleRect::center() );
    
    auto action = MoveBy::create(1, Vec2(150,0));

    auto director = Director::getInstance();
    director->getActionManager().addAction(action, grossini, true);

    Director::getInstance()->getScheduler().schedule(CC_SCHEDULE_SELECTOR(PauseTest::unpause), this, 3, CC_REPEAT_FOREVER, 0.0f, isPaused());
}

void PauseTest::unpause(float /*dt*/)
{
    Director::getInstance()->getScheduler().unschedule( CC_SCHEDULE_SELECTOR(PauseTest::unpause), this );
    auto node = getChildByTag( kTagGrossini );
    auto director = Director::getInstance();
    director->getActionManager().resumeTarget(node);
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

    auto pMove = MoveBy::create(2, Vec2(200, 0));
    auto pCallback = CallFunc::create(CC_CALLBACK_0(StopActionTest::stopAction,this));
    auto pSequence = Sequence::create(to_action_ptr(pMove), to_action_ptr(pCallback));
    pSequence->setTag(kTagSequence);

    auto pChild = Sprite::create(s_pathGrossini);
    pChild->setPosition( VisibleRect::center() );

    addChild(pChild, 1, kTagGrossini);
    pChild->runAction(pSequence);
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
    
    auto pMove1 = MoveBy::create(2, Vec2(200, 0));
    auto pMove2 = MoveBy::create(2, Vec2(-200, 0));
    auto pSequenceMove = Sequence::createWithTwoActions(pMove1, pMove2);
    auto pRepeatMove = RepeatForever::create(pSequenceMove);
    pRepeatMove->setTag(kTagSequence);
    
    auto pScale1 = ScaleBy::create(2, 1.5f);
    auto pScale2 = ScaleBy::create(2, 1.0f/1.5f);
    auto pSequenceScale = Sequence::createWithTwoActions(pScale1, pScale2);
    auto pRepeatScale = RepeatForever::create(pSequenceScale);
    pRepeatScale->setTag(kTagSequence);
    
    auto pRotate = RotateBy::create(2, 360);
    auto pRepeatRotate = RepeatForever::create(pRotate);
    
    auto pChild = Sprite::create(s_pathGrossini);
    pChild->setPosition( VisibleRect::center() );
    
    addChild(pChild, 1, kTagGrossini);
    pChild->runAction(pRepeatMove);
    pChild->runAction(pRepeatScale);
    pChild->runAction(pRepeatRotate);
    Director::getInstance()->getScheduler().schedule((SEL_SCHEDULE)&StopAllActionsTest::stopAction, this, 0.0f, 0, 4, !_running);
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

    pGrossini->runAction(ScaleBy::create(2, 2));

    auto director = Director::getInstance();
    director->getActionManager().pauseTarget(pGrossini);
    pGrossini->runAction(RotateBy::create(2, 360));

    Director::getInstance()->getScheduler().schedule(CC_SCHEDULE_SELECTOR(ResumeTest::resumeGrossini), this, 3.0f, CC_REPEAT_FOREVER, 0.0f, !_running);
}

void ResumeTest::resumeGrossini(float /*time*/)
{
    Director::getInstance()->getScheduler().unschedule( CC_SCHEDULE_SELECTOR(ResumeTest::resumeGrossini), this );

    auto pGrossini = getChildByTag(kTagGrossini);
    auto director = Director::getInstance();
    director->getActionManager().resumeTarget(pGrossini);
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

    auto pMove1 = MoveBy::create(2, Vec2(200, 0));
    auto pMove2 = MoveBy::create(2, Vec2(-200, 0));
    auto pSequenceMove = Sequence::createWithTwoActions(pMove1, pMove2);
    auto pRepeatMove = RepeatForever::create(pSequenceMove);
    pRepeatMove->setFlags(kMoveFlag | kRepeatForeverFlag);

    auto pScale1 = ScaleBy::create(2, 1.5f);
    auto pScale2 = ScaleBy::create(2, 1.0f/1.5f);
    auto pSequenceScale = Sequence::createWithTwoActions(pScale1, pScale2);
    auto pRepeatScale = RepeatForever::create(pSequenceScale);
    pRepeatScale->setFlags(kScaleFlag | kRepeatForeverFlag);

    auto pRotate = RotateBy::create(2, 360);
    auto pRepeatRotate = RepeatForever::create(pRotate);
    pRepeatRotate->setFlags(kRotateFlag | kRepeatForeverFlag);

    auto pChild = Sprite::create(s_pathGrossini);
    pChild->setPosition( VisibleRect::center() );

    addChild(pChild, 1, kTagGrossini);
    pChild->runAction(pRepeatMove);
    pChild->runAction(pRepeatScale);
    pChild->runAction(pRepeatRotate);
    Director::getInstance()->getScheduler().schedule((SEL_SCHEDULE)&StopActionsByFlagsTest::stopAction, this, 0.0f, 0, 4, !_running);
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

    auto move = MoveBy::create(2, Vec2(100, 100));
    sprite->runAction(move);
}

std::string Issue14050Test::subtitle() const
{
    return "Issue14050. Sprite should not leak.";
}

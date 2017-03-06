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

#include "ActionsTest.h"
#include "../testResource.h"

#include "2d/CCActionCamera.h"
#include "2d/CCActionCatmullRom.h"
#include "2d/CCActionInstant.h"
#include "2d/CCAnimationCache.h"
#include "2d/CCLabel.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerCustom.h"
#include "base/CCEventCustom.h"
#include "base/CCEventDispatcher.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "renderer/CCGroupCommand.h"
#include "ui/UIImageView.h"
#include "ui/UIText.h"

using namespace cocos2d;
using namespace cocos2d::ui;

ActionsTests::ActionsTests()
{
    ADD_TEST_CASE(ActionMove);
    ADD_TEST_CASE(ActionMove3D);
    ADD_TEST_CASE(ActionRotate);
    ADD_TEST_CASE(ActionRotateBy3D);
    ADD_TEST_CASE(ActionScale);
    ADD_TEST_CASE(ActionSkew);
    ADD_TEST_CASE(ActionRotationalSkew);
    ADD_TEST_CASE(ActionRotationalSkewVSStandardSkew);
    ADD_TEST_CASE(ActionSkewRotateScale);
    ADD_TEST_CASE(ActionJump);
    ADD_TEST_CASE(ActionCardinalSpline);
    ADD_TEST_CASE(ActionCatmullRom);
    ADD_TEST_CASE(ActionBezier);
    ADD_TEST_CASE(ActionBlink);
    ADD_TEST_CASE(ActionFade);
    ADD_TEST_CASE(ActionTint);
    ADD_TEST_CASE(ActionAnimate);
    ADD_TEST_CASE(ActionSequence);
    ADD_TEST_CASE(ActionSequence2);
    ADD_TEST_CASE(ActionSequence3);
    ADD_TEST_CASE(ActionRemoveSelf);
    ADD_TEST_CASE(ActionSpawn);
    ADD_TEST_CASE(ActionSpawn2);
    ADD_TEST_CASE(ActionReverse);
    ADD_TEST_CASE(ActionDelayTime);
    ADD_TEST_CASE(ActionRepeat);
    ADD_TEST_CASE(ActionRepeatForever);
    ADD_TEST_CASE(ActionRotateToRepeat);
    ADD_TEST_CASE(ActionCallFunction);
    ADD_TEST_CASE(ActionCallFuncN);
    ADD_TEST_CASE(ActionCallFuncND);
    ADD_TEST_CASE(ActionReverseSequence);
    ADD_TEST_CASE(ActionReverseSequence2);
    ADD_TEST_CASE(ActionOrbit);
    ADD_TEST_CASE(ActionFollow);
    ADD_TEST_CASE(ActionFollowWithOffset);
    ADD_TEST_CASE(ActionTargeted);
    ADD_TEST_CASE(ActionTargetedReverse);
    ADD_TEST_CASE(ActionMoveStacked);
    ADD_TEST_CASE(ActionMoveJumpStacked);
    ADD_TEST_CASE(ActionMoveBezierStacked);
    ADD_TEST_CASE(ActionCardinalSplineStacked);
    ADD_TEST_CASE(ActionCatmullRomStacked);
    ADD_TEST_CASE(PauseResumeActions);
    ADD_TEST_CASE(ActionResize);
    ADD_TEST_CASE(Issue1305);
    ADD_TEST_CASE(Issue1305_2);
    ADD_TEST_CASE(Issue1288);
    ADD_TEST_CASE(Issue1288_2);
    ADD_TEST_CASE(Issue1327);
    ADD_TEST_CASE(Issue1398);
    ADD_TEST_CASE(Issue2599)
    ADD_TEST_CASE(ActionFloatTest);
}

std::string ActionsDemo::title() const
{
    return "ActionsTest";
}

void ActionsDemo::onEnter()
{
    TestCase::onEnter();

    // Or you can create an sprite using a filename. only PNG is supported now. Probably TIFF too
    _grossini = Sprite::create(s_pathGrossini);
    _grossini->retain();

    _tamara = Sprite::create(s_pathSister1); 
    _tamara->retain();

    _kathia = Sprite::create(s_pathSister2);
    _kathia->retain();

    addChild(_grossini, 1);
    addChild(_tamara, 2);
    addChild(_kathia, 3);

    _grossini->setPosition(VisibleRect::center().x, VisibleRect::bottom().y+VisibleRect::getVisibleRect().size.height/3);
    _tamara->setPosition(VisibleRect::center().x, VisibleRect::bottom().y+VisibleRect::getVisibleRect().size.height*2/3);
    _kathia->setPosition(VisibleRect::center().x, VisibleRect::bottom().y+VisibleRect::getVisibleRect().size.height/2);
}

void ActionsDemo::onExit()
{
    _grossini->release();
    _tamara->release();
    _kathia->release();

    TestCase::onExit();
}

void ActionsDemo::centerSprites(unsigned int numberOfSprites)
{
    auto s = Director::getInstance()->getWinSize();

    if( numberOfSprites == 0 )
    {
        _tamara->setVisible(false);
        _kathia->setVisible(false);
        _grossini->setVisible(false);
    } 
    else if ( numberOfSprites == 1 ) 
    {
        _tamara->setVisible(false);
        _kathia->setVisible(false);
        _grossini->setPosition(s.width/2, s.height/2);
    }
    else if( numberOfSprites == 2 ) 
    {        
        _kathia->setPosition(s.width/3, s.height/2);
        _tamara->setPosition(2*s.width/3, s.height/2);
        _grossini->setVisible(false);
    } 
    else if( numberOfSprites == 3 ) 
    {
        _grossini->setPosition(s.width/2, s.height/2);
        _tamara->setPosition(s.width/4, s.height/2);
        _kathia->setPosition(3 * s.width/4, s.height/2);
    }
}

void ActionsDemo::alignSpritesLeft(unsigned int numberOfSprites)
{
    auto s = Director::getInstance()->getWinSize();

    if( numberOfSprites == 1 ) 
    {
        _tamara->setVisible(false);
        _kathia->setVisible(false);
        _grossini->setPosition(60, s.height/2);
    } 
    else if( numberOfSprites == 2 ) 
    {        
        _kathia->setPosition(60, s.height/3);
        _tamara->setPosition(60, 2*s.height/3);
        _grossini->setVisible( false );
    } 
    else if( numberOfSprites == 3 ) 
    {
        _grossini->setPosition(60, s.height/2);
        _tamara->setPosition(60, 2*s.height/3);
        _kathia->setPosition(60, s.height/3);
    }
}

//------------------------------------------------------------------
//
//    ActionMove
//
//------------------------------------------------------------------
void ActionMove::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);

    auto s = Director::getInstance()->getWinSize();

    auto actionTo = MoveTo::create(2, Vec2(s.width-40, s.height-40));
    auto actionBy = MoveBy::create(2, Vec2(80,80));
    auto actionByBack = actionBy->reverse();

    _tamara->runAction( actionTo);
    _grossini->runAction(
        Sequence::create(
            to_action_ptr(actionBy),
            to_action_ptr(actionByBack)
        )
    );
    _kathia->runAction(MoveTo::create(1, Vec2(40,40)));
}

std::string ActionMove::subtitle() const
{
    return "MoveTo / MoveBy";
}

//------------------------------------------------------------------
//
//    ActionMove3D
//
//------------------------------------------------------------------
void ActionMove3D::onEnter()
{
    ActionsDemo::onEnter();
    
    centerSprites(3);
    
    auto s = Director::getInstance()->getWinSize();
    
    _tamara->setPosition3D(Vec3(s.width-40, s.height-40, 0));
    _kathia->setPosition3D(Vec3(40, 40, 0));
    
    auto actionTo = MoveTo::create(2, Vec3(s.width-40, s.height-40, -100));
    auto actionBy = MoveBy::create(2, Vec3(80, 80, -100));
    auto actionByBack = actionBy->reverse();
    
    _tamara->runAction(actionTo);
    _grossini->runAction(
        Sequence::create(
            to_action_ptr(actionBy),
            to_action_ptr(actionByBack)
        )
    );
    _kathia->runAction(MoveTo::create(1, Vec3(40, 40, -100)));
}

std::string ActionMove3D::subtitle() const
{
    return "MoveTo / MoveBy 3D";
}

//------------------------------------------------------------------
//
// ActionScale
//
//------------------------------------------------------------------
void ActionScale::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);

    auto actionTo = ScaleTo::create(2.0f, 0.5f);
    auto actionBy = ScaleBy::create(2.0f, 1.0f, 10.0f);
    auto actionBy_reverse = actionBy->reverse();
    auto actionBy2 = ScaleBy::create(2.0f, 5.0f, 1.0f);
    auto actionBy2_reverse = actionBy2->reverse();

    _grossini->runAction( actionTo);
    _tamara->runAction(
        Sequence::create(
            to_action_ptr(actionBy),
            to_action_ptr(actionBy_reverse)
        )
    );
    _kathia->runAction(
        Sequence::create(
            to_action_ptr(actionBy2),
            to_action_ptr(actionBy2_reverse)
        )
    );
}

std::string ActionScale::subtitle() const
{
    return "ScaleTo / ScaleBy";
}

//------------------------------------------------------------------
//
//    ActionSkew
//
//------------------------------------------------------------------
void ActionSkew::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);

    auto actionTo = SkewTo::create(2, 37.2f, -37.2f);
    auto actionToBack = SkewTo::create(2, 0, 0);
    auto actionBy = SkewBy::create(2, 0.0f, -90.0f);
    auto actionBy2 = SkewBy::create(2, 45.0f, 45.0f);
    auto actionBy2_reverse = actionBy2->reverse();
    auto actionByBack = actionBy->reverse();

    _tamara->runAction(
        Sequence::create(
            to_action_ptr(actionTo),
            to_action_ptr(actionToBack)
        )
    );
    _grossini->runAction(
        Sequence::create(
            to_action_ptr(actionBy),
            to_action_ptr(actionByBack)
        )
    );

    _kathia->runAction(
        Sequence::create(
            to_action_ptr(actionBy2),
            to_action_ptr(actionBy2_reverse)
        )
    );
}

std::string ActionSkew::subtitle() const
{
    return "SkewTo / SkewBy";
}

// ActionRotationalSkew
void ActionRotationalSkew::onEnter()
{
    ActionsDemo::onEnter();

    this->centerSprites(3);

    auto actionTo = RotateTo::create(2, 180, 180);
    auto actionToBack = RotateTo::create(2, 0, 0);
    auto actionBy = RotateBy::create(2, 0.0f, 360);
    auto actionByBack = actionBy->reverse();

    auto actionBy2 = RotateBy::create(2, 360, 0);
    auto actionBy2Back = actionBy2->reverse();

    _tamara->runAction(
        Sequence::create(
            to_action_ptr(actionBy),
            to_action_ptr(actionByBack)
        )
    );
    _grossini->runAction(
        Sequence::create(
            to_action_ptr(actionTo),
            to_action_ptr(actionToBack)
        )
    );
    _kathia->runAction(
        Sequence::create(
            to_action_ptr(actionBy2),
            to_action_ptr(actionBy2Back)
        )
    );
}

std::string ActionRotationalSkew::subtitle() const
{
    return "RotationalSkewTo / RotationalSkewBy";
}



//ActionRotationalSkewVSStandardSkew
void ActionRotationalSkewVSStandardSkew::onEnter()
{
    ActionsDemo::onEnter();

    _tamara->removeFromParentAndCleanup(true);
    _grossini->removeFromParentAndCleanup(true);
    _kathia->removeFromParentAndCleanup(true);

    auto s = Director::getInstance()->getWinSize();

    Size boxSize(100.0f, 100.0f);

    auto box = LayerColor::create(Color4B(255,255,0,255));
    box->setAnchorPoint(Vec2(0.5,0.5));
    box->setContentSize( boxSize );
    box->setIgnoreAnchorPointForPosition(false);
    box->setPosition(s.width/2, s.height - 100 - box->getContentSize().height/2);
    this->addChild(box);

    auto label = Label::createWithTTF("Standard cocos2d Skew", "fonts/Marker Felt.ttf", 16.0f);
    label->setPosition(s.width/2, s.height - 100 + label->getContentSize().height);
    this->addChild(label);

    auto actionTo = SkewBy::create(2, 360, 0);
    auto actionToBack = SkewBy::create(2, -360, 0);

    box->runAction(
        Sequence::create(
            to_action_ptr(actionTo),
            to_action_ptr(actionToBack)
        )
    );

    box = LayerColor::create(Color4B(255,255,0,255));
    box->setAnchorPoint(Vec2(0.5,0.5));
    box->setContentSize(boxSize);
    box->setIgnoreAnchorPointForPosition(false);
    box->setPosition(s.width/2, s.height - 250 - box->getContentSize().height/2);
    this->addChild(box);

    label = Label::createWithTTF("Rotational Skew", "fonts/Marker Felt.ttf", 16.0f);
    label->setPosition(s.width/2, s.height - 250 + label->getContentSize().height/2);
    this->addChild(label);
    auto actionTo2 = RotateBy::create(2, 360, 0);
    auto actionToBack2 = RotateBy::create(2, -360, 0);
    box->runAction(
        Sequence::create(
            to_action_ptr(actionTo2),
            to_action_ptr(actionToBack2)
        )
    );
}
std::string ActionRotationalSkewVSStandardSkew::subtitle() const
{
    return "Skew Comparison";
}

// ActionSkewRotateScale
void ActionSkewRotateScale::onEnter()
{
    ActionsDemo::onEnter();

    _tamara->removeFromParentAndCleanup(true);
    _grossini->removeFromParentAndCleanup(true);
    _kathia->removeFromParentAndCleanup(true);

    Size boxSize(100.0f, 100.0f);

    auto box = LayerColor::create(Color4B(255, 255, 0, 255));
    box->setAnchorPoint(Vec2(0, 0));
    box->setPosition(190, 110);
    box->setContentSize(boxSize);

    static float markrside = 10.0f;
    auto uL = LayerColor::create(Color4B(255, 0, 0, 255));
    box->addChild(uL);
    uL->setContentSize(Size(markrside, markrside));
    uL->setPosition(0.f, boxSize.height - markrside);
    uL->setAnchorPoint(Vec2(0, 0));

    auto uR = LayerColor::create(Color4B(0, 0, 255, 255));
    box->addChild(uR);
    uR->setContentSize(Size(markrside, markrside));
    uR->setPosition(boxSize.width - markrside, boxSize.height - markrside);
    uR->setAnchorPoint(Vec2(0, 0));
    addChild(box);

    auto actionTo = SkewTo::create(2, 0.f, 2.f);
    auto rotateTo = RotateTo::create(2, 61.0f);
    auto actionScaleTo = ScaleTo::create(2, -0.44f, 0.47f);

    auto actionScaleToBack = ScaleTo::create(2, 1.0f, 1.0f);
    auto rotateToBack = RotateTo::create(2, 0);
    auto actionToBack = SkewTo::create(2, 0, 0);

    box->runAction(
        Sequence::create(
            to_action_ptr(actionTo),
            to_action_ptr(actionToBack)
        )
    );
    box->runAction(
        Sequence::create(
            to_action_ptr(rotateTo),
            to_action_ptr(rotateToBack)
        )
    );
    box->runAction(
        Sequence::create(
            to_action_ptr(actionScaleTo),
            to_action_ptr(actionScaleToBack)
        )
    );
}

std::string ActionSkewRotateScale::subtitle() const
{
    return "Skew + Rotate + Scale";
}

//------------------------------------------------------------------
//
//    ActionRotate
//
//------------------------------------------------------------------
void ActionRotate::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);

    auto actionTo = RotateTo::create( 2, 45);
    auto actionTo2 = RotateTo::create( 2, -45);
    auto actionTo0 = RotateTo::create(2 , 0);
    _tamara->runAction(
        Sequence::create(
            to_action_ptr(actionTo),
            to_action_ptr(actionTo0)
        )
    );

    auto actionBy = RotateBy::create(2 ,  360);
    auto actionByBack = actionBy->reverse();
    _grossini->runAction(
        Sequence::create(
            to_action_ptr(actionBy),
            to_action_ptr(actionByBack)
        )
    );

    _kathia->runAction(
        Sequence::create(
            to_action_ptr(actionTo2),
            to_action_ptr(actionTo0->clone())
        )
    );
}

std::string ActionRotate::subtitle() const
{
    return "RotateTo / RotateBy";
}

//------------------------------------------------------------------
//
//    ActionRotateBy3D
//
//------------------------------------------------------------------
void ActionRotateBy3D::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);

    auto actionBy1 = RotateBy::create(4, Vec3(360, 0, 0));
    auto actionBy1_reverse = actionBy1->reverse();
    auto actionBy2 = RotateBy::create(4, Vec3(0, 360, 0));
    auto actionBy2_reverse = actionBy2->reverse();
    auto actionBy3 = RotateBy::create(4 ,Vec3(0, 0, 360));
    auto actionBy3_reverse = actionBy3->reverse();

    _tamara->runAction(
        Sequence::create(
            to_action_ptr(actionBy1),
            to_action_ptr(actionBy1_reverse)
        )
    );
    _grossini->runAction(
        Sequence::create(
            to_action_ptr(actionBy2),
            to_action_ptr(actionBy2_reverse)
        )
    );
    _kathia->runAction(
        Sequence::create(
            to_action_ptr(actionBy3),
            to_action_ptr(actionBy3_reverse)
        )
    );
}

std::string ActionRotateBy3D::subtitle() const
{
    return "RotateBy in 3D";
}

//------------------------------------------------------------------
//
// ActionJump
//
//------------------------------------------------------------------
void ActionJump::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);

    auto actionTo = JumpTo::create(2, Vec2(300,300), 50, 4);
    auto actionBy = JumpBy::create(2, Vec2(300,0), 50, 4);
    auto actionUp = JumpBy::create(2, Vec2(0,0), 80, 4);
    auto actionByBack = actionBy->reverse();

    _tamara->runAction( actionTo);
    _grossini->runAction(
        Sequence::create(
            to_action_ptr(actionBy),
            to_action_ptr(actionByBack)
        )
    );
    _kathia->runAction( RepeatForever::create(actionUp));
}
std::string ActionJump::subtitle() const
{
    return "JumpTo / JumpBy";
}

//------------------------------------------------------------------
//
// ActionBezier
//
//------------------------------------------------------------------
void ActionBezier::onEnter()
{
    ActionsDemo::onEnter();

    auto s = Director::getInstance()->getWinSize();

    //
    // startPosition can be any coordinate, but since the movement
    // is relative to the Bezier curve, make it (0,0)
    //

    centerSprites(3);

    // sprite 1
    BezierConfig bezier;
    bezier.controlPoint_1 = Vec2(0, s.height/2);
    bezier.controlPoint_2 = Vec2(300, -s.height/2);
    bezier.endPosition = Vec2(300,100);

    auto bezierForward = BezierBy::create(3, bezier);
    auto bezierBack = bezierForward->reverse();
    auto rep = RepeatForever::create(
        Sequence::create(
            to_action_ptr(bezierForward),
            to_action_ptr(bezierBack)
        )
    );


    // sprite 2
    _tamara->setPosition(80,160);
	BezierConfig bezier2;
    bezier2.controlPoint_1 = Vec2(100, s.height/2);
    bezier2.controlPoint_2 = Vec2(200, -s.height/2);
    bezier2.endPosition = Vec2(240,160);

    auto bezierTo1 = BezierTo::create(2, bezier2);    

    // sprite 3
    _kathia->setPosition(400,160);
    auto bezierTo2 = BezierTo::create(2, bezier2);

    _grossini->runAction( rep);
    _tamara->runAction(bezierTo1);
    _kathia->runAction(bezierTo2);

}

std::string ActionBezier::subtitle() const
{
    return "BezierBy / BezierTo";
}

//------------------------------------------------------------------
//
// ActionBlink
//
//------------------------------------------------------------------
void ActionBlink::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(2);

    auto action1 = Blink::create(2, 10);
    auto action2 = Blink::create(2, 5);

    _tamara->runAction( action1);
    _kathia->runAction(action2);
}

std::string  ActionBlink::subtitle() const
{
    return "Blink";
}

//------------------------------------------------------------------
//
// ActionFade
//
//------------------------------------------------------------------
void ActionFade::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(2);

    _tamara->setOpacity( 0 );
    auto action1 = FadeIn::create(1.0f);
    auto action1Back = action1->reverse();

    auto action2 = FadeOut::create(1.0f);
    auto action2Back = action2->reverse();
    auto action2BackReverse = action2Back->reverse();
    auto action2BackReverseReverse = action2BackReverse->reverse();

    _tamara->setOpacity(122);
    _tamara->runAction(
        Sequence::create(
            to_action_ptr(action1),
            to_action_ptr(action1Back)
        )
    );
    _kathia->setOpacity(122);
    _kathia->runAction(
        Sequence::create(
            to_action_ptr(action2),
            to_action_ptr(action2Back),
            to_action_ptr(action2BackReverse),
            to_action_ptr(action2BackReverseReverse)
        )
    );
}

std::string  ActionFade::subtitle() const
{
    return "FadeIn / FadeOut";
}

//------------------------------------------------------------------
//
// ActionTint
//
//------------------------------------------------------------------

void ActionTint::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(2);

    auto action1 = TintTo::create(2, 255, 0, 255);
    auto action2 = TintBy::create(2, -127, -255, -127);
    auto action2Back = action2->reverse();

    _tamara->runAction( action1);
    _kathia->runAction(
        Sequence::create(
            to_action_ptr(action2),
            to_action_ptr(action2Back)
        )
    );
}

std::string  ActionTint::subtitle() const
{
    return "TintTo / TintBy";
}

//------------------------------------------------------------------
//
// ActionAnimate
//
//------------------------------------------------------------------
void ActionAnimate::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);

    //
    // Manual animation
    //
    std::unique_ptr<Animation> animation(new Animation);
    
    for( int i=1;i<15;i++)
    {
        char szName[100] = {0};
        sprintf(szName, "Images/grossini_dance_%02d.png", i);
        animation->addSpriteFrameWithFile(szName);
    }
    // should last 2.8 seconds. And there are 14 frames.
    animation->setDelayPerUnit(2.8f / 14.0f);
    animation->setRestoreOriginalFrame(true);

    auto action = Animate::create( std::move( animation));
    auto action_reverse = action->reverse();

    _grossini->runAction(
        Sequence::create(
            to_action_ptr(action),
            to_action_ptr(action_reverse)
        )
    );
    
    //
    // File animation
    //
    // With 2 loops and reverse
    auto cache = AnimationCache::getInstance();
    cache->addAnimationsWithFile("animations/animations-2.plist");
    auto animation2 = cache->extractAnimation("dance_1");

    auto action2 = Animate::create(animation2->clone());
    auto action2_reverse = action2->reverse();

    _tamara->runAction(
        Sequence::create(
            to_action_ptr(action2),
            to_action_ptr(action2_reverse)
        )
    );

    _frameDisplayedListener = EventListenerCustom::create(AnimationFrameDisplayedNotification, [](EventCustom * event){
        auto userData = static_cast<AnimationFrame::DisplayedEventInfo*>(event->getUserData());
        
         log("target %p with data %s", userData->target, Value(userData->userInfo).getDescription().c_str());
    });

    _eventDispatcher->addEventListenerWithFixedPriority(_frameDisplayedListener, -1);

    //
    // File animation
    //
    // with 4 loops
    auto animation3 = std::move(animation2);
    animation3->setLoops(4);


    auto action3 = Animate::create( std::move( animation3));
    _kathia->runAction(action3);
}

void ActionAnimate::onExit()
{
    ActionsDemo::onExit();
    _eventDispatcher->removeEventListener(_frameDisplayedListener);
}

std::string ActionAnimate::title() const
{
    return "Animation";
}

std::string ActionAnimate::subtitle() const
{
    return "Center: Manual animation. Border: using file format animation";
}

//------------------------------------------------------------------
//
//    ActionSequence
//
//------------------------------------------------------------------
void ActionSequence::onEnter()
{
    ActionsDemo::onEnter();

    alignSpritesLeft(1);

    auto action = Sequence::create(
        to_action_ptr(MoveBy::create( 2, Vec2(240,0))),
        to_action_ptr(RotateBy::create( 2,  540))
    );

    _grossini->runAction(action);
}

std::string ActionSequence::subtitle() const
{
    return "Sequence: Move + Rotate";
}

//------------------------------------------------------------------
//
//    ActionSequence2
//
//------------------------------------------------------------------
void ActionSequence2::onEnter()
{
    ActionsDemo::onEnter();

    alignSpritesLeft(1);

    _grossini->setVisible(false);

    auto action = Sequence::create(
		to_action_ptr(Place::create(Vec2(200,200))),
		to_action_ptr(Show::create()),
		to_action_ptr(MoveBy::create(1, Vec2(100,0))),
		to_action_ptr(CallFunc::create( CC_CALLBACK_0(ActionSequence2::callback1,this))),
		to_action_ptr(CallFunc::create( CC_CALLBACK_0(ActionSequence2::callback2,this,_grossini))),
		to_action_ptr(CallFunc::create( CC_CALLBACK_0(ActionSequence2::callback3,this,_grossini,0xbebabeba)))
    );

    _grossini->runAction(action);
}

void ActionSequence2::callback1()
{
    auto s = Director::getInstance()->getWinSize();
    auto label = Label::createWithTTF("callback 1 called", "fonts/Marker Felt.ttf", 16.0f);
    label->setPosition(s.width/4*1,s.height/2);

    addChild(label);
}

void ActionSequence2::callback2(Node* /*sender*/)
{
    auto s = Director::getInstance()->getWinSize();
    auto label = Label::createWithTTF("callback 2 called", "fonts/Marker Felt.ttf", 16.0f);
    label->setPosition(s.width/4*2,s.height/2);

    addChild(label);
}

void ActionSequence2::callback3(Node* /*sender*/, long /*data*/)
{
    auto s = Director::getInstance()->getWinSize();
    auto label = Label::createWithTTF("callback 3 called", "fonts/Marker Felt.ttf", 16.0f);
    label->setPosition(s.width/4*3,s.height/2);

    addChild(label);
}

std::string ActionSequence2::subtitle() const
{
    return "Sequence of InstantActions";
}

//------------------------------------------------------------------
//
//    ActionSequence3
//
//------------------------------------------------------------------
void ActionSequence3::onEnter()
{
    ActionsDemo::onEnter();

    alignSpritesLeft(1);

    // Uses Array API
    auto action1 = MoveBy::create(2, Vec2(240,0));
    auto action2 = RotateBy::create(2, 540);
    auto action3 = action1->reverse();
    auto action4 = action2->reverse();

    Sequence::actions_container array;

    array.reserve(4);

    array.push_back(to_action_ptr<FiniteTimeAction>(action1));
    array.push_back(to_action_ptr<FiniteTimeAction>(action2));
    array.push_back(to_action_ptr<FiniteTimeAction>(action3));
    array.push_back(to_action_ptr<FiniteTimeAction>(action4));

    auto action = Sequence::create( std::move(array) );
    _grossini->runAction(action);
}

std::string ActionSequence3::subtitle() const
{
    return "Sequence: Using Array API";
}

//------------------------------------------------------------------
//
// ActionCallFuncN
//
//------------------------------------------------------------------
void ActionCallFuncN::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(1);

    auto action = Sequence::create(
        to_action_ptr(MoveBy::create(2.0f, Vec2(150,0))),
        to_action_ptr(CallFuncN::create( CC_CALLBACK_1(ActionCallFuncN::callback, this)))
    );

    _grossini->runAction(action);
}

std::string ActionCallFuncN::title() const
{
    return "CallFuncN";
}

std::string ActionCallFuncN::subtitle() const
{
    return "Grossini should jump after moving";
}

void ActionCallFuncN::callback(Node* sender )
{
    auto a = JumpBy::create(5, Vec2(0,0), 100, 5);
    sender->runAction(a);
}
//------------------------------------------------------------------
//
// ActionCallFuncND
// CallFuncND is no longer needed. It can simulated with std::bind()
//
//------------------------------------------------------------------
void ActionCallFuncND::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(1);

    auto action = Sequence::create(
        to_action_ptr(MoveBy::create(2.0f, Vec2(200,0))),
        to_action_ptr(CallFuncN::create( CC_CALLBACK_1(ActionCallFuncND::doRemoveFromParentAndCleanup, this, true)))
    );

    _grossini->runAction(action);
}

std::string ActionCallFuncND::title() const
{
    return "CallFuncND + auto remove";
}

std::string ActionCallFuncND::subtitle() const
{
    return "simulates CallFuncND with std::bind()";
}

void ActionCallFuncND::doRemoveFromParentAndCleanup(Node* /*sender*/, bool cleanup)
{
    _grossini->removeFromParentAndCleanup(cleanup);
}

//------------------------------------------------------------------
//
//    ActionCallFunction
//
//------------------------------------------------------------------
void ActionCallFunction::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);


	auto action1 = Sequence::create(
                        to_action_ptr(MoveBy::create(2, Vec2(200,0))),
                        to_action_ptr(CallFunc::create( std::bind(&ActionCallFunction::callback1, this) )),
                        to_action_ptr(CallFunc::create(
                             // lambda
                             [&](){
                                 auto s = Director::getInstance()->getWinSize();
                                 auto label = Label::createWithTTF("called:lambda callback", "fonts/Marker Felt.ttf", 16.0f);
                                 label->setPosition(s.width/4*1,s.height/2-40);
                                 this->addChild(label);
                             }  ))
                        );

    auto action2 = Sequence::create(
                        to_action_ptr(ScaleBy::create(2 ,  2)),
                        to_action_ptr(FadeOut::create(2)),
                        to_action_ptr(CallFunc::create( std::bind(&ActionCallFunction::callback2, this, _tamara) ))
                   );

    auto action3 = Sequence::create(
                        to_action_ptr(RotateBy::create(3 , 360)),
                        to_action_ptr(FadeOut::create(2)),
                        to_action_ptr(CallFunc::create( std::bind(&ActionCallFunction::callback3, this, _kathia, 42) ))
                   );

    _grossini->runAction(action1);
    _tamara->runAction(action2);
    _kathia->runAction(action3);
}


void ActionCallFunction::callback1()
{
    auto s = Director::getInstance()->getWinSize();
    auto label = Label::createWithTTF("callback 1 called", "fonts/Marker Felt.ttf", 16.0f);
    label->setPosition(s.width/4*1,s.height/2);

    addChild(label);
}

void ActionCallFunction::callback2(Node* sender)
{
    auto s = Director::getInstance()->getWinSize();
    auto label = Label::createWithTTF("callback 2 called", "fonts/Marker Felt.ttf", 16.0f);
    label->setPosition(s.width/4*2,s.height/2);

    addChild(label);

	CCLOG("sender is: %p", sender);
}

void ActionCallFunction::callback3(Node* sender, long data)
{
    auto s = Director::getInstance()->getWinSize();
    auto label = Label::createWithTTF("callback 3 called", "fonts/Marker Felt.ttf", 16.0f);
    label->setPosition(s.width/4*3,s.height/2);
    addChild(label);

	CCLOG("target is: %p, data is: %ld", sender, data);
}

std::string ActionCallFunction::subtitle() const
{
    return "Callbacks: CallFunc with std::function()";
}
//------------------------------------------------------------------
//
// ActionSpawn
//
//------------------------------------------------------------------

void ActionSpawn::onEnter()
{
    ActionsDemo::onEnter();

    alignSpritesLeft(1);

    auto action = Spawn::create(
        JumpBy::create(2, Vec2(300,0), 50, 4),
        RotateBy::create( 2,  720),
        nullptr);

    _grossini->runAction(action);
}

std::string ActionSpawn::subtitle() const
{
    return "Spawn: Jump + Rotate";
}

//------------------------------------------------------------------
//
// ActionSpawn2
//
//------------------------------------------------------------------

void ActionSpawn2::onEnter()
{
    ActionsDemo::onEnter();

    alignSpritesLeft(1);

    auto action1 = to_action_ptr<FiniteTimeAction>( JumpBy::create(2, Vec2(300,0), 50, 4) );
    auto action2 = to_action_ptr<FiniteTimeAction>( RotateBy::create( 2,  720) );

    std::vector<action_ptr<FiniteTimeAction>> array;

    array.push_back( std::move(action1) );
    array.push_back( std::move(action2) );

    auto action = Spawn::create( std::move(array) );
    _grossini->runAction(action);
}

std::string ActionSpawn2::subtitle() const
{
    return "Spawn: using the Array API";
}

//------------------------------------------------------------------
//
// ActionRepeatForever
//
//------------------------------------------------------------------
void ActionRepeatForever::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(1);

    auto action = Sequence::create(
        to_action_ptr(DelayTime::create(1)),
        to_action_ptr(CallFunc::create( std::bind( &ActionRepeatForever::repeatForever, this, _grossini) ))
    );

    _grossini->runAction(action);
}

void ActionRepeatForever::repeatForever(Node* sender)
{
    auto repeat = RepeatForever::create( RotateBy::create(1.0f, 360) );

    sender->runAction(repeat);
}

std::string ActionRepeatForever::subtitle() const
{
    return "CallFuncN + RepeatForever";
}


//------------------------------------------------------------------
//
// ActionRotateToRepeat
//
//------------------------------------------------------------------
void ActionRotateToRepeat::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(2);

	auto act1 = RotateTo::create(1, 90);
	auto act2 = RotateTo::create(1, 0);
	auto seq = Sequence::create(to_action_ptr(act1), to_action_ptr(act2));
	auto rep1 = RepeatForever::create(seq);
	auto rep2 = Repeat::create( seq->clone(), 10);

    _tamara->runAction(rep1);
    _kathia->runAction(rep2);
}

std::string ActionRotateToRepeat ::subtitle() const
{
    return "Repeat/RepeatForever + RotateTo";
}

//------------------------------------------------------------------
//
// ActionReverse
//
//------------------------------------------------------------------
void ActionReverse::onEnter()
{
    ActionsDemo::onEnter();

    alignSpritesLeft(1);

    auto jump = JumpBy::create(2, Vec2(300,0), 50, 4);
    auto jump_reverse = jump->reverse();

    auto action = Sequence::create( to_action_ptr(jump), to_action_ptr(jump_reverse));

    _grossini->runAction(action);
}

std::string ActionReverse::subtitle() const
{
    return "Reverse an action";
}


//------------------------------------------------------------------
//
// ActionDelayTime
//
//------------------------------------------------------------------
void ActionDelayTime::onEnter()
{
    ActionsDemo::onEnter();

    alignSpritesLeft(1);

    auto move = MoveBy::create(1, Vec2(150,0));
    auto move_clone = move->clone();

    auto action = Sequence::create(
        to_action_ptr(move),
        to_action_ptr(DelayTime::create(2)),
        to_action_ptr(move_clone)
    );

    _grossini->runAction(action);
}

std::string ActionDelayTime::subtitle() const
{
    return "DelayTime: m + delay + m";
}


//------------------------------------------------------------------
//
// ActionReverseSequence
//
//------------------------------------------------------------------
void ActionReverseSequence::onEnter()
{
    ActionsDemo::onEnter();

    alignSpritesLeft(1);

    auto move1 = MoveBy::create(1, Vec2(250,0));
    auto move1_reverse = move1->reverse();
    auto move2 = MoveBy::create(1, Vec2(0,50));

    auto seq = Sequence::create(
        to_action_ptr(move1),
        to_action_ptr(move2),
        to_action_ptr(move1_reverse)
    );
    auto seq_reverse = seq->reverse();

    auto action = Sequence::create(
        to_action_ptr(seq),
        to_action_ptr(seq_reverse)
    );

    _grossini->runAction(action);
}

std::string ActionReverseSequence::subtitle() const
{
    return "Reverse a sequence";
}


//------------------------------------------------------------------
//
// ActionReverseSequence2
//
//------------------------------------------------------------------
void ActionReverseSequence2::onEnter()
{
    ActionsDemo::onEnter();

    alignSpritesLeft(2);


    // Test:
    //   Sequence should work both with IntervalAction and InstantActions
	auto move1 = MoveBy::create(1, Vec2(250,0));
    auto move1_reverse = move1->reverse();
	auto move2 = MoveBy::create(1, Vec2(0,50));
	auto tog1 = ToggleVisibility::create();
	auto tog2 = ToggleVisibility::create();
    auto seq = Sequence::create(
        to_action_ptr(move1),
        to_action_ptr(tog1),
        to_action_ptr(move2),
        to_action_ptr(tog2),
        to_action_ptr(move1_reverse)
    );
    auto seq_reverse = seq->reverse();
	auto action = Repeat::create(
        Sequence::create(
            to_action_ptr(seq),
            to_action_ptr(seq_reverse)
        ),
        3
    );


    // Test:
    //   Also test that the reverse of Hide is Show, and vice-versa
    _kathia->runAction(action);

	auto move_tamara = MoveBy::create(1, Vec2(100,0));
	auto move_tamara2 = MoveBy::create(1, Vec2(50,0));
	auto hide = Hide::create();
	auto seq_tamara = Sequence::create(
        to_action_ptr(move_tamara),
        to_action_ptr(hide),
        to_action_ptr(move_tamara2)
    );
	auto seq_back = seq_tamara->reverse();
    _tamara->runAction(
        Sequence::create(
            to_action_ptr(seq_tamara),
            to_action_ptr(seq_back)
        )
    );
}
std::string ActionReverseSequence2::subtitle() const
{
    return "Reverse sequence 2";
}

//------------------------------------------------------------------
//
// ActionRepeat
//
//------------------------------------------------------------------
void ActionRepeat::onEnter()
{
    ActionsDemo::onEnter();

    alignSpritesLeft(2);


    auto a1 = MoveBy::create(1, Vec2(150,0));
    auto a1_clone = a1->clone();
    auto a1_reverse = a1->reverse();

    auto action1 = Repeat::create(
        Sequence::create( to_action_ptr(Place::create(Vec2(60,60))), to_action_ptr(a1)) , 
        3); 
    auto  action2 = RepeatForever::create(
        Sequence::create(to_action_ptr(a1_clone), to_action_ptr(a1_reverse))
    );

    _kathia->runAction(action1);
    _tamara->runAction(action2);
}

std::string ActionRepeat::subtitle() const
{
    return "Repeat / RepeatForever actions";
}

//------------------------------------------------------------------
//
// ActionOrbit
//
//------------------------------------------------------------------
void ActionOrbit::onEnter()
{
    ActionsDemo::onEnter();

    Director::getInstance()->setProjection(Director::Projection::_2D);
    centerSprites(3);

    auto orbit1 = OrbitCamera::create(2,1, 0, 0, 180, 0, 0);
    auto orbit1_reverse = orbit1->reverse();
    auto action1 = Sequence::create(
        to_action_ptr(orbit1),
        to_action_ptr(orbit1_reverse)
    );

    auto orbit2 = OrbitCamera::create(2,1, 0, 0, 180, -45, 0);
    auto orbit2_reverse = orbit2->reverse();
    auto action2 = Sequence::create(
        to_action_ptr(orbit2),
        to_action_ptr(orbit2_reverse)
    );

    auto orbit3 = OrbitCamera::create(2,1, 0, 0, 180, 90, 0);
    auto orbit3_reverse = orbit3->reverse();
    auto action3 = Sequence::create(
        to_action_ptr(orbit3),
        to_action_ptr(orbit3_reverse)
    );

    _kathia->runAction(RepeatForever::create(action1));
    _tamara->runAction(RepeatForever::create(action2));
    _grossini->runAction(RepeatForever::create(action3));

    auto move = MoveBy::create(3, Vec2(100,-100));
    auto move_back = move->reverse();
    auto seq = Sequence::create(to_action_ptr(move), to_action_ptr(move_back));
    auto rfe = RepeatForever::create(seq);
    _kathia->runAction(rfe);
    _tamara->runAction(rfe->clone() );
    _grossini->runAction( rfe->clone() );
}

void ActionOrbit::onExit()
{
    ActionsDemo::onExit();
    
    Director::getInstance()->setProjection(Director::Projection::DEFAULT);
}

std::string ActionOrbit::subtitle() const
{
    return "OrbitCamera action";
}

//------------------------------------------------------------------
//
// ActionFollow
//
//------------------------------------------------------------------
void ActionFollow::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(1);
    auto s = Director::getInstance()->getWinSize();

    DrawNode* drawNode = DrawNode::create();
    float x = s.width*2 - 100;
    float y = s.height;
    
    Vec2 vertices[] = { Vec2(5,5), Vec2(x-5,5), Vec2(x-5,y-5), Vec2(5,y-5) };
    drawNode->drawPoly(vertices, 4, true,  Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
    
    this->addChild(drawNode);

    _grossini->setPosition(-200, s.height / 2);
    auto move = MoveBy::create(2, Vec2(s.width * 3, 0));
    auto move_back = move->reverse();
    auto seq = Sequence::create(to_action_ptr(move), to_action_ptr(move_back));
    auto rep = RepeatForever::create(seq);

    _grossini->runAction(rep);

    this->runAction(Follow::create(_grossini, Rect(0, 0, s.width * 2 - 100, s.height)));
}

std::string ActionFollow::subtitle() const
{
    return "Follow action";
}

//------------------------------------------------------------------
//
// ActionFollowWithOffset
//
//------------------------------------------------------------------
void ActionFollowWithOffset::onEnter()
{
    ActionsDemo::onEnter();
    
    centerSprites(1);
    auto s = Director::getInstance()->getWinSize();
    
    DrawNode* drawNode = DrawNode::create();
    float x = s.width*2 - 100;
    float y = s.height;
    
    Vec2 vertices[] = { Vec2(5,5), Vec2(x-5,5), Vec2(x-5,y-5), Vec2(5,y-5) };
    drawNode->drawPoly(vertices, 4, true,  Color4F(CCRANDOM_0_1(), CCRANDOM_0_1(), CCRANDOM_0_1(), 1));
    
    this->addChild(drawNode);
    
    _grossini->setPosition(-200, s.height / 2);
    auto move = MoveBy::create(2, Vec2(s.width * 3, 1));
    auto move_back = move->reverse();
    auto seq = Sequence::create(to_action_ptr(move), to_action_ptr(move_back));
    auto rep = RepeatForever::create(seq);
    
    _grossini->runAction(rep);
    
    //sample offset values set
    float verticalOffset = -900;
    float horizontalOffset = 200;
    this->runAction(Follow::createWithOffset(_grossini, horizontalOffset,verticalOffset,Rect(0, 0, s.width * 2 - 100, s.height)));
}

std::string ActionFollowWithOffset::subtitle() const
{
    return "Follow action with horizontal and vertical offset";
}


void ActionTargeted::onEnter()
{
    ActionsDemo::onEnter();
    centerSprites(2);


    auto jump1 = JumpBy::create(2,Vec2::ZERO,100,3);
    auto jump2 = jump1->clone();
    auto rot1 = RotateBy::create(1, 360);
    auto rot2 = rot1->clone();

    auto t1 = TargetedAction::create(_kathia, jump2);
    auto t2 = TargetedAction::create(_kathia, rot2);

    auto seq = Sequence::create(
        to_action_ptr(jump1),
        to_action_ptr(t1),
        to_action_ptr(rot1),
        to_action_ptr(t2)
    );
    auto always = RepeatForever::create(seq);

    _tamara->runAction(always);
}

std::string ActionTargeted::title() const
{
    return "ActionTargeted";
}

std::string ActionTargeted::subtitle() const
{
    return "Action that runs on another target. Useful for sequences";
}


void ActionTargetedReverse::onEnter()
{
    ActionsDemo::onEnter();
    centerSprites(2);
    
    
    auto jump1 = JumpBy::create(2,Vec2::ZERO,100,3);
    auto jump2 = jump1->clone();
    auto rot1 = RotateBy::create(1, 360);
    auto rot2 = rot1->clone();
    
    auto t1 = TargetedAction::create(_kathia, jump2);
    auto t2 = TargetedAction::create(_kathia, rot2);
    
    auto seq = Sequence::create(
        to_action_ptr(jump1),
        to_action_ptr(t1->reverse()),
        to_action_ptr(rot1),
        to_action_ptr(t2->reverse())
    );
    auto always = RepeatForever::create(seq);
    
    _tamara->runAction(always);
}

std::string ActionTargetedReverse::title() const
{
    return "ActionTargetedReverse";
}

std::string ActionTargetedReverse::subtitle() const
{
    return "Action that runs reversely on another target. Useful for sequences";
}

// ActionStacked

void ActionStacked::onEnter()
{
    ActionsDemo::onEnter();
        
    this->centerSprites(0);
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(ActionStacked::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto s = Director::getInstance()->getWinSize();
    this->addNewSpriteWithCoords(Vec2(s.width/2, s.height/2));
}

void ActionStacked::addNewSpriteWithCoords(Vec2 p)
{
    int idx = CCRANDOM_0_1() * 1400 / 100;
    int x = (idx%5) * 85;
    int y = (idx/5) * 121;
    
    
    auto sprite = Sprite::create("Images/grossini_dance_atlas.png", Rect(x,y,85,121));
    
    sprite->setPosition(p);
    this->addChild(sprite);
    
    this->runActionsInSprite(sprite);
}

void ActionStacked::runActionsInSprite(Sprite *)
{
    // override me
}

void ActionStacked::onTouchesEnded(const std::vector<Touch*>& touches, Event*)
{
    for ( auto &touch: touches ) {
        auto location = touch->getLocation();
        addNewSpriteWithCoords( location );
    }
}

std::string ActionStacked::title() const
{
    return "Override me";
}

std::string ActionStacked::subtitle() const
{
    return "Tap screen";
}

// ActionMoveStacked


void ActionMoveStacked::runActionsInSprite(Sprite *sprite)
{
    sprite->runAction(
        RepeatForever::create(
            Sequence::create(
                to_action_ptr(MoveBy::create(0.05f, Vec2(10,10))),
                to_action_ptr(MoveBy::create(0.05f, Vec2(-10,-10)))
            )
        )
    );
    
    auto action = MoveBy::create(2.0f, Vec2(400,0));
    auto action_back = action->reverse();
    
    sprite->runAction(
      RepeatForever::create(
        Sequence::create(to_action_ptr(action), to_action_ptr(action_back))
      ));
}


std::string ActionMoveStacked::title() const
{
    return "Stacked MoveBy/To actions";
}

// ActionMoveJumpStacked

void ActionMoveJumpStacked::runActionsInSprite(Sprite *sprite)
{
    sprite->runAction(
        RepeatForever::create(
            Sequence::create(
                to_action_ptr(MoveBy::create(0.05f, Vec2(10,2))),
                to_action_ptr(MoveBy::create(0.05f, Vec2(-10,-2)))
            )));
    
    auto jump = JumpBy::create(2.0f, Vec2(400,0), 100, 5);
    auto jump_back = jump->reverse();
    
    sprite->runAction(
      RepeatForever::create(
          Sequence::create(to_action_ptr(jump), to_action_ptr(jump_back))
          ));
}

std::string ActionMoveJumpStacked::title() const
{
    return "tacked Move + Jump actions";
}

// ActionMoveBezierStacked

void ActionMoveBezierStacked::runActionsInSprite(Sprite *sprite)
{
    auto s = Director::getInstance()->getWinSize();
    
    // sprite 1
    BezierConfig bezier;
    bezier.controlPoint_1 = Vec2(0, s.height/2);
    bezier.controlPoint_2 = Vec2(300, -s.height/2);
    bezier.endPosition = Vec2(300,100);
    
    auto bezierForward = BezierBy::create(3, bezier);
    auto bezierBack = bezierForward->reverse();
    auto seq = Sequence::create(to_action_ptr(bezierForward), to_action_ptr(bezierBack));
    auto rep = RepeatForever::create(seq);
    sprite->runAction(rep);
    
    sprite->runAction(
     RepeatForever::create(
      Sequence::create(
       to_action_ptr(MoveBy::create(0.05f, Vec2(10,0))),
       to_action_ptr(MoveBy::create(0.05f, Vec2(-10,0)))
       )));
}

std::string ActionMoveBezierStacked::title() const
{
    return "Stacked Move + Bezier actions";
}


// ActionCatmullRomStacked

void ActionCatmullRomStacked::onEnter()
{
    ActionsDemo::onEnter();
    
    this->centerSprites(2);
    
    auto s = Director::getInstance()->getWinSize();
    
    //
    // sprite 1 (By)
    //
    // startPosition can be any coordinate, but since the movement
    // is relative to the Catmull Rom curve, it is better to start with (0,0).
    //
    
    _tamara->setPosition(50,50);
    
    auto array = PointArray::create(20);
    
    array->addControlPoint(Vec2(0,0));
    array->addControlPoint(Vec2(80,80));
    array->addControlPoint(Vec2(s.width-80,80));
    array->addControlPoint(Vec2(s.width-80,s.height-80));
    array->addControlPoint(Vec2(80,s.height-80));
    array->addControlPoint(Vec2(80,80));
    array->addControlPoint(Vec2(s.width/2, s.height/2));
    
    auto action = CatmullRomBy::create(3, array);
    auto reverse = action->reverse();
    
    auto seq = Sequence::create(to_action_ptr(action), to_action_ptr(reverse));
    
    _tamara->runAction(seq);
    
    _tamara->runAction(
        RepeatForever::create(
            Sequence::create(
                to_action_ptr(MoveBy::create(0.05f, Vec2(10,0))),
                to_action_ptr(MoveBy::create(0.05f, Vec2(-10,0)))
                )));
    
    auto drawNode1 = DrawNode::create();
    drawNode1->setPosition(Vec2(50,50));
    drawNode1->drawCatmullRom(array, 50, Color4F(1.0, 1.0, 0.0, 0.5));
    this->addChild(drawNode1);
    
    //
    // sprite 2 (To)
    //
    // The startPosition is not important here, because it uses a "To" action.
    // The initial position will be the 1st point of the Catmull Rom path
    //
    
    auto array2 = PointArray::create(20);
    
    array2->addControlPoint(Vec2(s.width/2, 30));
    array2->addControlPoint(Vec2(s.width-80,30));
    array2->addControlPoint(Vec2(s.width-80,s.height-80));
    array2->addControlPoint(Vec2(s.width/2,s.height-80));
    array2->addControlPoint(Vec2(s.width/2, 30));
    
    auto action2 = CatmullRomTo::create(3, array2);
    auto reverse2 = action2->reverse();
    
    auto seq2 = Sequence::create(to_action_ptr(action2), to_action_ptr(reverse2));
    
    _kathia->runAction(seq2);
    
    _kathia->runAction(
        RepeatForever::create(
            Sequence::create(
                to_action_ptr(MoveBy::create(0.05f, Vec2(10,0))),
                to_action_ptr(MoveBy::create(0.05f, Vec2(-10,0)))
            )));
    
    auto drawNode2 = DrawNode::create();
    drawNode2->drawCatmullRom(array2, 50, Color4F(1.0, 0.0, 0.0, 0.5));
    this->addChild(drawNode2);
}

ActionCatmullRomStacked::~ActionCatmullRomStacked()
{
}

std::string ActionCatmullRomStacked::title() const
{
    return "Stacked MoveBy + CatmullRom actions";
}

std::string ActionCatmullRomStacked::subtitle() const
{
    return "MoveBy + CatmullRom at the same time in the same sprite";
}


// ActionCardinalSplineStacked

void ActionCardinalSplineStacked::onEnter()
{
    ActionsDemo::onEnter();
    
    this->centerSprites(2);
    
    auto s = Director::getInstance()->getWinSize();
    
    auto array = PointArray::create(20);
    
    array->addControlPoint(Vec2(0, 0));
    array->addControlPoint(Vec2(s.width/2-30,0));
    array->addControlPoint(Vec2(s.width/2-30,s.height-80));
    array->addControlPoint(Vec2(0, s.height-80));
    array->addControlPoint(Vec2(0, 0));
    
    
    //
    // sprite 1 (By)
    //
    // Spline with no tension (tension==0)
    //

    auto action = CardinalSplineBy::create(3, array, 0);
    auto reverse = action->reverse();
    
    auto seq = Sequence::create(to_action_ptr(action), to_action_ptr(reverse));
    
    _tamara->setPosition(50,50);
    _tamara->runAction(seq);
    
    _tamara->runAction(
        RepeatForever::create(
            Sequence::create(
                to_action_ptr(MoveBy::create(0.05f, Vec2(10,0))),
                to_action_ptr(MoveBy::create(0.05f, Vec2(-10,0)))
                )));
    
    auto drawNode1 = DrawNode::create();
    drawNode1->setPosition(Vec2(50,50));
    drawNode1->drawCardinalSpline(array, 0, 100, Color4F(1.0, 0.0, 1.0, 1.0));
    this->addChild(drawNode1);
    
    //
    // sprite 2 (By)
    //
    // Spline with high tension (tension==1)
    //
    
    auto *action2 = CardinalSplineBy::create(3, array, 1);
    auto reverse2 = action2->reverse();
    
    auto seq2 = Sequence::create(to_action_ptr(action2), to_action_ptr(reverse2));
    
    _kathia->setPosition(s.width/2,50);
    
    _kathia->runAction(seq2);
    
    _kathia->runAction(
        RepeatForever::create(
            Sequence::create(
                to_action_ptr(MoveBy::create(0.05f, Vec2(10,0))),
                to_action_ptr(MoveBy::create(0.05f, Vec2(-10,0)))
                )));
    
    auto drawNode2 = DrawNode::create();
    drawNode2->setPosition(Vec2(s.width/2,50));
    drawNode2->drawCardinalSpline(array, 1, 100, Color4F(0.0, 0.0, 1.0, 1.0));
    this->addChild(drawNode2);
}

ActionCardinalSplineStacked::~ActionCardinalSplineStacked()
{
}

std::string ActionCardinalSplineStacked::title() const
{
    return "Stacked MoveBy + CardinalSpline actions";
}

std::string ActionCardinalSplineStacked::subtitle() const
{
    return "CCMoveBy + CardinalSplineBy/To at the same time";
}

// Issue1305
void Issue1305::onEnter()
{
    ActionsDemo::onEnter();
    centerSprites(0);

    _spriteTmp = to_node_ptr<Sprite>(Sprite::create("Images/grossini.png"));
    _spriteTmp->runAction(CallFunc::create(std::bind(&Issue1305::log, this, _spriteTmp.get())));

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this,
                 [=](float /*dt*/) {
                     _spriteTmp->setPosition(250,250);
                     addChild(_spriteTmp.get());
                 })
            .repeat(0)
            .delay(2.0f)
            .paused(isPaused())
    );
}

void Issue1305::log(Node* /*sender*/)
{
    cocos2d::log("This message SHALL ONLY appear when the sprite is added to the scene, NOT BEFORE");
}

void Issue1305::onExit()
{
    _spriteTmp->stopAllActions();
    _spriteTmp.reset();
    ActionsDemo::onExit();
}

std::string Issue1305::title() const
{
    return "Issue 1305";
}

std::string Issue1305::subtitle() const
{
    return "In two seconds you should see a message on the console. NOT BEFORE.";
}

void Issue1305_2::onEnter()
{
    ActionsDemo::onEnter();
    centerSprites(0);

    auto spr = Sprite::create("Images/grossini.png");
    spr->setPosition(200,200);
    addChild(spr);

    auto act1 = MoveBy::create(2 ,Vec2(0, 100));
    auto act2 = CallFunc::create( std::bind( &Issue1305_2::printLog1, this));
    auto act3 = MoveBy::create(2, Vec2(0, -100));
    auto act4 = CallFunc::create( std::bind( &Issue1305_2::printLog2, this));
    auto act5 = MoveBy::create(2, Vec2(100, -100));
    auto act6 = CallFunc::create( std::bind( &Issue1305_2::printLog3, this));
    auto act7 = MoveBy::create(2, Vec2(-100, 0));
    auto act8 = CallFunc::create( std::bind( &Issue1305_2::printLog4, this));

    auto actF = Sequence::create(
        to_action_ptr(act1),
        to_action_ptr(act2),
        to_action_ptr(act3),
        to_action_ptr(act4),
        to_action_ptr(act5),
        to_action_ptr(act6),
        to_action_ptr(act7),
        to_action_ptr(act8)
    );

    spr->runAction(actF);
}

void Issue1305_2::printLog1()
{
    log("1st block");
}

void Issue1305_2::printLog2()
{
    log("2nd block");
}

void Issue1305_2::printLog3()
{
    log("3rd block");
}

void Issue1305_2::printLog4()
{
    log("4th block");
}

std::string Issue1305_2::title() const
{
    return "Issue 1305 #2";
}

std::string Issue1305_2::subtitle() const
{
    return "See console. You should only see one message for each block";
}

void Issue1288::onEnter()
{
    ActionsDemo::onEnter();
    centerSprites(0);

    auto spr = Sprite::create("Images/grossini.png");
    spr->setPosition(100, 100);
    addChild(spr);

    auto act1 = MoveBy::create(0.5, Vec2(100, 0));
    auto act2 = act1->reverse();
    auto act3 = Sequence::create(to_action_ptr(act1), to_action_ptr(act2));
    auto act4 = Repeat::create(act3, 2);

    spr->runAction(act4);
}

std::string Issue1288::title() const
{
    return "Issue 1288";
}

std::string Issue1288::subtitle() const
{
    return "Sprite should end at the position where it started.";
}

void Issue1288_2::onEnter()
{
    ActionsDemo::onEnter();
    centerSprites(0);

    auto spr = Sprite::create("Images/grossini.png");
    spr->setPosition(100, 100);
    addChild(spr);

    auto act1 = MoveBy::create(0.5, Vec2(100, 0));
    spr->runAction(Repeat::create(act1, 1));
}

std::string Issue1288_2::title() const
{
    return "Issue 1288 #2";
}

std::string Issue1288_2::subtitle() const
{
    return "Sprite should move 100 pixels, and stay there";
}


void Issue1327::onEnter()
{
    ActionsDemo::onEnter();
    centerSprites(0);

    auto spr = Sprite::create("Images/grossini.png");
    spr->setPosition(100, 100);
    addChild(spr);

    auto act1 = CallFunc::create( std::bind(&Issue1327::logSprRotation, this, spr));
    auto act2 = RotateBy::create(0.25, 45);
    auto act3 = CallFunc::create( std::bind(&Issue1327::logSprRotation, this, spr));
    auto act4 = RotateBy::create(0.25, 45);
    auto act5 = CallFunc::create( std::bind(&Issue1327::logSprRotation, this, spr));
    auto act6 = RotateBy::create(0.25, 45);
    auto act7 = CallFunc::create( std::bind(&Issue1327::logSprRotation, this, spr));
    auto act8 = RotateBy::create(0.25, 45);
    auto act9 = CallFunc::create( std::bind(&Issue1327::logSprRotation, this, spr));

    auto actF = Sequence::create(
        to_action_ptr(act1),
        to_action_ptr(act2),
        to_action_ptr(act3),
        to_action_ptr(act4),
        to_action_ptr(act5),
        to_action_ptr(act6),
        to_action_ptr(act7),
        to_action_ptr(act8),
        to_action_ptr(act9)
    );
    spr->runAction(actF);
}

std::string Issue1327::title() const
{
    return "Issue 1327";
}

std::string Issue1327::subtitle() const
{
    return "See console: You should see: 0, 45, 90, 135, 180";
}

void Issue1327::logSprRotation(Sprite* sender)
{
    log("%f", sender->getRotation());
}

//Issue1398
void Issue1398::incrementInteger()
{
    _testInteger++;
    log("incremented to %d", _testInteger);
}

void Issue1398::onEnter()
{
    ActionsDemo::onEnter();
    this->centerSprites(0);

    _testInteger = 0;
    log("testInt = %d", _testInteger);

    this->runAction(
        Sequence::create(
			to_action_ptr(CallFunc::create( std::bind(&Issue1398::incrementIntegerCallback, this, (void*)"1"))),
			to_action_ptr(CallFunc::create( std::bind(&Issue1398::incrementIntegerCallback, this, (void*)"2"))),
			to_action_ptr(CallFunc::create( std::bind(&Issue1398::incrementIntegerCallback, this, (void*)"3"))),
			to_action_ptr(CallFunc::create( std::bind(&Issue1398::incrementIntegerCallback, this, (void*)"4"))),
			to_action_ptr(CallFunc::create( std::bind(&Issue1398::incrementIntegerCallback, this, (void*)"5"))),
			to_action_ptr(CallFunc::create( std::bind(&Issue1398::incrementIntegerCallback, this, (void*)"6"))),
			to_action_ptr(CallFunc::create( std::bind(&Issue1398::incrementIntegerCallback, this, (void*)"7"))),
			to_action_ptr(CallFunc::create( std::bind(&Issue1398::incrementIntegerCallback, this, (void*)"8")))
        )
    );
}

void Issue1398::incrementIntegerCallback(void* data)
{
    this->incrementInteger();
    log("%s", (char*)data);
}

std::string Issue1398::subtitle() const
{
    return "See console: You should see an 8";
}

std::string Issue1398::title() const
{
    return "Issue 1398";
}

void Issue2599::onEnter()
{
    ActionsDemo::onEnter();
    this->centerSprites(0);
    
    _count = 0;
    log("before: count = %d", _count);
    
    log("start count up 50 times using Repeat action");
    auto delay = 1.0f / 50;
    auto repeatAction = Repeat::create(
        Sequence::createWithTwoActions(
            CallFunc::create([&](){ this->_count++; }),
            DelayTime::create(delay)),
        50);
    this->runAction(
        Sequence::createWithTwoActions(
            repeatAction,
            CallFunc::create([&]() { log("after: count = %d", this->_count); })
        ));
}

std::string Issue2599::subtitle() const
{
    return "See console: You should see '50'";
}

std::string Issue2599::title() const
{
    return "Issue 2599";
}

/** ActionCatmullRom
 */
void ActionCatmullRom::onEnter()
{
    ActionsDemo::onEnter();
    
    this->centerSprites(2);
    
    auto s = Director::getInstance()->getWinSize();
    
    //
    // sprite 1 (By)
    //
    // startPosition can be any coordinate, but since the movement
    // is relative to the Catmull Rom curve, it is better to start with (0,0).
    //
    
    _tamara->setPosition(50, 50);
    
    auto array = PointArray::create(20);
    
    array->addControlPoint(Vec2(0, 0));
    array->addControlPoint(Vec2(80, 80));
    array->addControlPoint(Vec2(s.width - 80, 80));
    array->addControlPoint(Vec2(s.width - 80, s.height - 80));
    array->addControlPoint(Vec2(80, s.height - 80));
    array->addControlPoint(Vec2(80, 80));
    array->addControlPoint(Vec2(s.width / 2, s.height / 2));
    
    auto action = CatmullRomBy::create(3, array);
    auto reverse = action->reverse();
    
    auto seq = Sequence::create(to_action_ptr(action), to_action_ptr(reverse));
    
    _tamara->runAction(seq);
    
    auto drawNode1 = DrawNode::create();
    drawNode1->setPosition(Vec2(50,50));
    drawNode1->drawCatmullRom(array, 50, Color4F(1.0, 0.0, 1.0, 1.0));
    this->addChild(drawNode1);
    
    //
    // sprite 2 (To)
    //
    // The startPosition is not important here, because it uses a "To" action.
    // The initial position will be the 1st point of the Catmull Rom path
    //    
    
    auto array2 = PointArray::create(20);
    
    array2->addControlPoint(Vec2(s.width / 2, 30));
    array2->addControlPoint(Vec2(s.width  -80, 30));
    array2->addControlPoint(Vec2(s.width - 80, s.height - 80));
    array2->addControlPoint(Vec2(s.width / 2, s.height - 80));
    array2->addControlPoint(Vec2(s.width / 2, 30));
    
    auto action2 = CatmullRomTo::create(3, array2);
    auto reverse2 = action2->reverse();
    
    auto seq2 = Sequence::create(to_action_ptr(action2), to_action_ptr(reverse2));
    
    _kathia->runAction(seq2);
    
    auto drawNode2 = DrawNode::create();
    drawNode2->drawCatmullRom(array2, 50, Color4F(0.0, 1.0, 1.0, 1.0));
    this->addChild(drawNode2);
}

ActionCatmullRom::~ActionCatmullRom()
{
}

std::string ActionCatmullRom::title() const
{
    return "CatmullRomBy / CatmullRomTo";
}

std::string ActionCatmullRom::subtitle() const
{
    return "Catmull Rom spline paths. Testing reverse too";
}

/** ActionCardinalSpline
 */
void ActionCardinalSpline::onEnter()
{
    ActionsDemo::onEnter();
    
    this->centerSprites(2);
    
    auto s = Director::getInstance()->getWinSize();
    
    auto array = PointArray::create(20);
    
    array->addControlPoint(Vec2(0, 0));
    array->addControlPoint(Vec2(s.width/2-30, 0));
    array->addControlPoint(Vec2(s.width/2-30, s.height-80));
    array->addControlPoint(Vec2(0, s.height-80));
    array->addControlPoint(Vec2(0, 0));
    
    //
    // sprite 1 (By)
    //
    // Spline with no tension (tension==0)
    //
    
    auto action = CardinalSplineBy::create(3, array, 0);
    auto reverse = action->reverse();
    
    auto seq = Sequence::create(to_action_ptr(action), to_action_ptr(reverse));
    
    _tamara->setPosition(50, 50);
    _tamara->runAction(seq);
    
    auto drawNode1 = DrawNode::create();
    drawNode1->setPosition(Vec2(50,50));
    drawNode1->drawCardinalSpline(array, 0, 100, Color4F(1.0, 0.0, 1.0, 1.0));
    this->addChild(drawNode1);
    
    //
    // sprite 2 (By)
    //
    // Spline with high tension (tension==1)
    //
    
    auto action2 = CardinalSplineBy::create(3, array, 1);
    auto reverse2 = action2->reverse();
    
    auto seq2 = Sequence::create(to_action_ptr(action2), to_action_ptr(reverse2));
    
    _kathia->setPosition(s.width/2, 50);
    _kathia->runAction(seq2);
    
    auto drawNode2 = DrawNode::create();
    drawNode2->setPosition(Vec2(s.width/2, 50));
    drawNode2->drawCardinalSpline(array, 1, 100, Color4F(1.0, 0.0, 1.0, 1.0));
    this->addChild(drawNode2);
}

ActionCardinalSpline::~ActionCardinalSpline()
{
}

std::string ActionCardinalSpline::title() const
{
    return "CardinalSplineBy / CardinalSplineTo";
}

std::string ActionCardinalSpline::subtitle() const
{
    return "Cardinal Spline paths. Testing different tensions for one array";
}

/** PauseResumeActions
 */

PauseResumeActions::PauseResumeActions()
{

}

PauseResumeActions::~PauseResumeActions()
{

}

void PauseResumeActions::onEnter()
{
    ActionsDemo::onEnter();
    
    this->centerSprites(3);
    
    _tamara->runAction(RepeatForever::create(RotateBy::create(3, 360)));
    _grossini->runAction(RepeatForever::create(RotateBy::create(3, -360)));
    _kathia->runAction(RepeatForever::create(RotateBy::create(3, 360)));
    
    auto pause = [=](float){
        log("Pausing");
        _tamara->pause();
        _grossini->pause();
        _kathia->pause();
    };
    auto resume = [=](float){
        log("Resuming");
        _tamara->resume();
        _grossini->resume();
        _kathia->resume();
    };

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, pause)
            .delay(3)
            .repeat(0)
            .paused(isPaused())
    );
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, resume)
            .delay(5)
            .repeat(0)
            .paused(isPaused())
    );
}

std::string PauseResumeActions::title() const
{
    return "PauseResumeActions";
}

std::string PauseResumeActions::subtitle() const
{
    return "All actions pause at 3s and resume at 5s";
}

//------------------------------------------------------------------
//
//    ActionResize
//    Works on all nodes where setContentSize is effective. 
//    But it's mostly useful for nodes where 9-slice is enabled
//
//------------------------------------------------------------------
void ActionResize::onEnter() 
{
    ActionsDemo::onEnter();

    _grossini->setVisible(false);
    _tamara->setVisible(false);
    _kathia->setVisible(false);

    Size widgetSize = getContentSize();

    Text* alert = Text::create("ImageView Content ResizeTo ResizeBy action. \nTop: ResizeTo/ResizeBy on a 9-slice ImageView  \nBottom: ScaleTo/ScaleBy on a 9-slice ImageView (for comparison)", "fonts/Marker Felt.ttf", 14);
    alert->setColor(Color3B(159, 168, 176));
    alert->setPosition(Vec2(widgetSize.width / 2.0f,
                            widgetSize.height / 2.0f - alert->getContentSize().height * 1.125f));

    addChild(alert);

    // Create the imageview
    Vec2 offset(0.0f, 50.0f);
    ImageView* imageViewResize = ImageView::create("cocosui/buttonHighlighted.png");
    imageViewResize->setScale9Enabled(true);
    imageViewResize->setContentSize(Size(50, 40));
    imageViewResize->setPosition(Vec2((widgetSize.width / 2.0f) + offset.x,
                                (widgetSize.height / 2.0f) + offset.y));

    auto resizeDown = cocos2d::ResizeTo::create(2.8f, Size(50, 40));
    auto resizeUp = cocos2d::ResizeTo::create(2.8f, Size(300, 40));

    auto resizeByDown = cocos2d::ResizeBy::create(1.8f, Size(0, -30));
    auto resizeByUp = cocos2d::ResizeBy::create(1.8f, Size(0, 30));
    addChild(imageViewResize);
    auto rep = RepeatForever::create(
        Sequence::create(
            to_action_ptr(resizeUp),
            to_action_ptr(resizeDown),
            to_action_ptr(resizeByDown),
            to_action_ptr(resizeByUp)
        )
    );
    imageViewResize->runAction(rep);

    // Create another imageview that scale to see the difference
    ImageView* imageViewScale = ImageView::create("cocosui/buttonHighlighted.png");
    imageViewScale->setScale9Enabled(true);
    imageViewScale->setContentSize(Size(50, 40));
    imageViewScale->setPosition(Vec2(widgetSize.width / 2.0f,
                                 widgetSize.height / 2.0f));

    auto scaleDownScale = cocos2d::ScaleTo::create(2.8f, 1.0f);
    auto scaleUpScale = cocos2d::ScaleTo::create(2.8f, 6.0f, 1.0f);

    auto scaleByDownScale = cocos2d::ScaleBy::create(1.8f, 1.0f, 0.25f);
    auto scaleByUpScale = cocos2d::ScaleBy::create(1.8f, 1.0f, 4.0f);
    addChild(imageViewScale);
    auto rep2 = RepeatForever::create(
        Sequence::create(
            to_action_ptr(scaleUpScale),
            to_action_ptr(scaleDownScale),
            to_action_ptr(scaleByDownScale),
            to_action_ptr(scaleByUpScale)
        )
    );
    imageViewScale->runAction(rep2);
}

std::string ActionResize::subtitle() const 
{
    return "ResizeTo / ResizeBy";
}

//------------------------------------------------------------------
//
//    ActionRemoveSelf
//
//------------------------------------------------------------------
void ActionRemoveSelf::onEnter()
{
	ActionsDemo::onEnter();

	alignSpritesLeft(1);

	auto action = Sequence::create(
		to_action_ptr(MoveBy::create( 2, Vec2(240,0))),
		to_action_ptr(RotateBy::create( 2,  540)),
		to_action_ptr(ScaleTo::create(1,0.1f)),
		to_action_ptr(RemoveSelf::create())
        );

	_grossini->runAction(action);
}

std::string ActionRemoveSelf::subtitle() const
{
	return "Sequence: Move + Rotate + Scale + RemoveSelf";
}

//------------------------------------------------------------------
//
//    ActionFloat
//
//------------------------------------------------------------------
void ActionFloatTest::onEnter()
{
    ActionsDemo::onEnter();

    centerSprites(3);

    // create float action with duration and from to value, using lambda function we can easly animate any property of the Node.
    auto actionFloat = ActionFloat::create(2.f, 0, 3, [this](float value) {
        _tamara->setScale(value);
    });

    float grossiniY = _grossini->getPositionY();

    auto actionFloat1 = ActionFloat::create(3.f, grossiniY, grossiniY + 50, [this](float value) {
        _grossini->setPositionY(value);
    });

    auto actionFloat2 = ActionFloat::create(3.f, 3, 1, [this] (float value) {
        _kathia->setScale(value);
    });

    _tamara->runAction(actionFloat);
    _grossini->runAction(actionFloat1);
    _kathia->runAction(actionFloat2);
}

std::string ActionFloatTest::subtitle() const
{
    return "ActionFloat";
}



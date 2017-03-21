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

#include "ActionsEaseTest.h"
#include "../testResource.h"

#include "2d/CCActionEase.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"

using namespace cocos2d;

enum {
    kTagAction1 = 1,
    kTagAction2 = 2,
    kTagSlider = 1,
};


//------------------------------------------------------------------
//
// SpriteDemo
//
//------------------------------------------------------------------
void EaseSpriteDemo::centerSprites(unsigned int numberOfSprites)
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


//------------------------------------------------------------------
//
// SpriteEase
//
//------------------------------------------------------------------

void SpriteEase::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130,0));
    auto move_back = std::unique_ptr<MoveBy>( move->reverse());
    
    auto move_ease_in = std::make_unique<EaseIn>( std::unique_ptr<ActionInterval>(move->clone()), 2.5f);
    auto move_ease_in_back = std::unique_ptr<EaseRateAction>( move_ease_in->reverse());
    
    auto move_ease_out = std::make_unique<EaseOut>( std::unique_ptr<ActionInterval>(move->clone()), 2.5f);
    auto move_ease_out_back = std::unique_ptr<EaseRateAction>( move_ease_out->reverse());
    
    auto delay = 0.25f;

    auto a2 =
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(move),
                std::make_unique<DelayTime>(delay),
                std::move(move_back),
                std::make_unique<DelayTime>(delay)
            ));
    a2->setTag(1);
    _grossini->runAction( std::move( a2));

    auto a1 =
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(move_ease_in),
                std::make_unique<DelayTime>(delay),
                std::move(move_ease_in_back),
                std::make_unique<DelayTime>(delay)
            ));
    a1->setTag(1);
    _tamara->runAction( std::move( a1));

    auto a =
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(move_ease_out),
                std::make_unique<DelayTime>(delay),
                std::move(move_ease_out_back),
                std::make_unique<DelayTime>(delay)
            ));
    a->setTag(1);
    _kathia->runAction( std::move( a));

    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpriteEase::testStopAction)
            .delay(6.25f)
            .repeat(0)
            .paused(isPaused())
    );
}


void SpriteEase::testStopAction(float /*dt*/)
{
    _tamara->stopActionByTag(1);
    _kathia->stopActionByTag(1);
    _grossini->stopActionByTag(1);
}

std::string SpriteEase::subtitle() const
{
    return "EaseIn - EaseOut - Stop";
}


//------------------------------------------------------------------
//
// SpriteEaseInOut
//
//------------------------------------------------------------------

void SpriteEaseInOut::onEnter()
{
    EaseSpriteDemo::onEnter();

    auto move_ease_inout1 = std::make_unique<EaseInOut>(std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x - 130, 0)), 0.65f);
    auto move_ease_inout_back1 = std::unique_ptr<EaseRateAction>( move_ease_inout1->reverse() );
    
    auto move_ease_inout2 = std::make_unique<EaseInOut>(std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x - 130, 0)), 1.35f);
    auto move_ease_inout_back2 = std::unique_ptr<EaseRateAction>( move_ease_inout2->reverse() );

    auto move_ease_inout3 = std::make_unique<EaseInOut>(std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x - 130, 0)), 1.0f);
    auto move_ease_inout_back3 = std::unique_ptr<EaseRateAction>( move_ease_inout3->reverse() );

    auto delay = 0.25f;

    _tamara->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(move_ease_inout1),
                std::make_unique<DelayTime>(delay),
                std::move(move_ease_inout_back1),
                std::make_unique<DelayTime>(delay)
            )));

    _kathia->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(move_ease_inout2),
                std::make_unique<DelayTime>(delay),
                std::move(move_ease_inout_back2),
                std::make_unique<DelayTime>(delay)
            )));

    _grossini->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(move_ease_inout3),
                std::make_unique<DelayTime>(delay),
                std::move(move_ease_inout_back3),
                std::make_unique<DelayTime>(delay)
            )));
}


std::string SpriteEaseInOut::subtitle() const
{
    return "EaseInOut and rates";
}

//------------------------------------------------------------------
//
// SpriteEaseExponential
//
//------------------------------------------------------------------

void SpriteEaseExponential::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130,0));
    auto move_back = std::unique_ptr<MoveBy>( move->reverse() );
    
    auto move_ease_in = std::make_unique<EaseExponentialIn>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_in_back = std::unique_ptr<ActionEase>( move_ease_in->reverse() );
    
    auto move_ease_out = std::make_unique<EaseExponentialOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_out_back = std::unique_ptr<ActionEase>( move_ease_out->reverse() );
    
    auto delay = 0.25f;

    _grossini->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(move),
                std::make_unique<DelayTime>(delay),
                std::move(move_back),
                std::make_unique<DelayTime>(delay)
            )));

    _tamara->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(move_ease_in),
                std::make_unique<DelayTime>(delay),
                std::move(move_ease_in_back),
                std::make_unique<DelayTime>(delay)
            )));

    _kathia->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(move_ease_out),
                std::make_unique<DelayTime>(delay),
                std::move(move_ease_out_back),
                std::make_unique<DelayTime>(delay)
            )));
}


std::string SpriteEaseExponential::subtitle() const
{
    return "ExpIn - ExpOut actions";
}

//------------------------------------------------------------------
//
// SpriteEaseExponentialInOut
//
//------------------------------------------------------------------

void SpriteEaseExponentialInOut::onEnter()
{
    EaseSpriteDemo::onEnter();

    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>( move->reverse() );
    
    auto move_ease = std::make_unique<EaseExponentialInOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_back = std::unique_ptr<ActionEase>( move_ease->reverse() );
    
    auto delay = 0.25f;
    
    _grossini->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(move),
                std::make_unique<DelayTime>(delay),
                std::move(move_back),
                std::make_unique<DelayTime>(delay)
            )));

    _tamara->runAction(
        std::make_unique<RepeatForever>(
            std::make_unique<Sequence>(
                std::move(move_ease),
                std::make_unique<DelayTime>(delay),
                std::move(move_ease_back),
                std::make_unique<DelayTime>(delay)
            )));

    this->positionForTwo();
}


std::string SpriteEaseExponentialInOut::subtitle() const
{
    return "EaseExponentialInOut action";
}
//------------------------------------------------------------------
//
// SpriteEaseSine
//
//------------------------------------------------------------------

void SpriteEaseSine::onEnter()
{
    EaseSpriteDemo::onEnter();

    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>( move->reverse());
    
    auto move_ease_in = std::make_unique<EaseSineIn>( std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_in_back = std::unique_ptr<ActionEase>( move_ease_in->reverse());
    
    auto move_ease_out = std::make_unique<EaseSineOut>( std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_out_back = std::unique_ptr<ActionEase>( move_ease_out->reverse());
    
    auto delay = 0.25f;
        
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease_in),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_in_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq3 = std::make_unique<Sequence>(
        std::move(move_ease_out),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_out_back),
        std::make_unique<DelayTime>(delay)        
    );
    
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
    _kathia->runAction( std::make_unique<RepeatForever>( std::move(seq3)));    
}


std::string SpriteEaseSine::subtitle() const
{
    return "EaseSineIn - EaseSineOut";
}
//------------------------------------------------------------------
//
// SpriteEaseSineInOut
//
//------------------------------------------------------------------

void SpriteEaseSineInOut::onEnter()
{
    EaseSpriteDemo::onEnter();

    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130,0));
    auto move_back = std::unique_ptr<MoveBy>( move->reverse());
    
    auto move_ease = std::make_unique<EaseSineInOut>( std::unique_ptr<ActionInterval>( move->clone()));
    auto move_ease_back = std::unique_ptr<ActionEase>( move_ease->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_back),
        std::make_unique<DelayTime>(delay)
    );

    this->positionForTwo();

    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
}


std::string SpriteEaseSineInOut::subtitle() const
{
    return "EaseSineInOut action";
}
//------------------------------------------------------------------
//
// SpriteEaseElastic
//
//------------------------------------------------------------------

void SpriteEaseElastic::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease_in = std::make_unique<EaseElasticIn>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_in_back = std::unique_ptr<ActionEase>(move_ease_in->reverse());
    
    auto move_ease_out = std::make_unique<EaseElasticOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_out_back = std::unique_ptr<ActionEase>(move_ease_out->reverse());
    
    auto delay = 0.25f;

    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease_in),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_in_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq3 = std::make_unique<Sequence>(
        std::move(move_ease_out),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_out_back),
        std::make_unique<DelayTime>(delay)
    );
    
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
    _kathia->runAction( std::make_unique<RepeatForever>( std::move(seq3)));    
}


std::string SpriteEaseElastic::subtitle() const
{
    return "Elastic In - Out actions";
}

//------------------------------------------------------------------
//
// SpriteEaseElasticInOut
//
//------------------------------------------------------------------

void SpriteEaseElasticInOut::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));

    auto move_ease_inout1 = std::make_unique<EaseElasticInOut>(std::unique_ptr<ActionInterval>(move->clone()), 0.3f);
    auto move_ease_inout_back1 = std::unique_ptr<ActionEase>(move_ease_inout1->reverse());
    
    auto move_ease_inout2 = std::make_unique<EaseElasticInOut>(std::unique_ptr<ActionInterval>(move->clone()), 0.45f);
    auto move_ease_inout_back2 = std::unique_ptr<ActionEase>(move_ease_inout2->reverse());
    
    auto move_ease_inout3 = std::make_unique<EaseElasticInOut>(std::unique_ptr<ActionInterval>(move->clone()), 0.6f);
    auto move_ease_inout_back3 = std::unique_ptr<ActionEase>(move_ease_inout3->reverse());
    
    auto delay = 0.25f;
        
    auto seq1 = std::make_unique<Sequence>(
        std::move(move_ease_inout1),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_inout_back1),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease_inout2),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_inout_back2),
        std::make_unique<DelayTime>(delay)
    );
    auto seq3 = std::make_unique<Sequence>(
        std::move(move_ease_inout3),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_inout_back3),
        std::make_unique<DelayTime>(delay)
    );
    
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _kathia->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq3))); 
}


std::string SpriteEaseElasticInOut::subtitle() const
{
    return "EaseElasticInOut action";
}


//------------------------------------------------------------------
//
// SpriteEaseBounce
//
//------------------------------------------------------------------

void SpriteEaseBounce::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease_in = std::make_unique<EaseBounceIn>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_in_back = std::unique_ptr<ActionEase>(move_ease_in->reverse());
    
    auto move_ease_out = std::make_unique<EaseBounceOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_out_back = std::unique_ptr<ActionEase>(move_ease_out->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease_in),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_in_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq3 = std::make_unique<Sequence>(
        std::move(move_ease_out),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_out_back),
        std::make_unique<DelayTime>(delay)
    );
    
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
    _kathia->runAction( std::make_unique<RepeatForever>( std::move(seq3)));    
}


std::string SpriteEaseBounce::subtitle() const
{
    return "Bounce In - Out actions";
}


//------------------------------------------------------------------
//
// SpriteEaseBounceInOut
//
//------------------------------------------------------------------

void SpriteEaseBounceInOut::onEnter()
{
    EaseSpriteDemo::onEnter();

    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease = std::make_unique<EaseBounceInOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_back = std::unique_ptr<ActionEase>(move_ease->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_back),
        std::make_unique<DelayTime>(delay)
    );
    
    this->positionForTwo();
    
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
}


std::string SpriteEaseBounceInOut::subtitle() const
{
    return "EaseBounceInOut action";
}

//------------------------------------------------------------------
//
// SpriteEaseBack
//
//------------------------------------------------------------------

void SpriteEaseBack::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease_in = std::make_unique<EaseBackIn>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_in_back = std::unique_ptr<ActionEase>(move_ease_in->reverse());
    
    auto move_ease_out = std::make_unique<EaseBackOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_out_back = std::unique_ptr<ActionEase>(move_ease_out->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease_in),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_in_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq3 = std::make_unique<Sequence>(
        std::move(move_ease_out),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_out_back),
        std::make_unique<DelayTime>(delay)
    );
    
    _grossini->runAction(std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction(std::make_unique<RepeatForever>( std::move(seq2)));
    _kathia->runAction(std::make_unique<RepeatForever>( std::move(seq3)));    
}


std::string SpriteEaseBack::subtitle() const
{
    return "Back In - Out actions";
}

//------------------------------------------------------------------
//
// SpriteEaseBackInOut
//
//------------------------------------------------------------------

void SpriteEaseBackInOut::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease = std::make_unique<EaseBackInOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_back = std::unique_ptr<ActionEase>(move_ease->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_back),
        std::make_unique<DelayTime>(delay)
    );
    
    this->positionForTwo();
    
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));    
}


std::string SpriteEaseBackInOut::subtitle() const
{
    return "EaseBackInOut action";
}

//------------------------------------------------------------------
//
// SpriteEaseBezier
//
//------------------------------------------------------------------

void SpriteEaseBezier::onEnter()
{
    EaseSpriteDemo::onEnter();
    
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
    
    auto bezierForward = std::make_unique<BezierBy>(3, bezier);
    auto bezierEaseForward = std::make_unique<EaseBezierAction>(std::move(bezierForward));
    bezierEaseForward->setBezierParamer(0.5, 0.5, 1.0, 1.0);
    
    auto bezierEaseBack = std::unique_ptr<EaseBezierAction>(bezierEaseForward->reverse());
    auto rep = std::make_unique<RepeatForever>(
        std::make_unique<Sequence>(
            std::move(bezierEaseForward),
            std::move(bezierEaseBack)
        )
    );
    
    
    // sprite 2
    _tamara->setPosition(80,160);
	BezierConfig bezier2;
    bezier2.controlPoint_1 = Vec2(100, s.height/2);
    bezier2.controlPoint_2 = Vec2(200, -s.height/2);
    bezier2.endPosition = Vec2(240,160);
    
    auto bezierTo1 = std::make_unique<BezierTo>(2, bezier2);
    auto bezierEaseTo1 = std::make_unique<EaseBezierAction>(std::move(bezierTo1));
    bezierEaseTo1->setBezierParamer(0.5, 0.5, 1.0, 1.0);
    
    // sprite 3
    _kathia->setPosition(400,160);
    auto bezierTo2 = std::make_unique<BezierTo>(2, bezier2);
    auto bezierEaseTo2 = std::make_unique<EaseBezierAction>( std::move(bezierTo2));
    bezierEaseTo2->setBezierParamer(0.0, 0.5, -5.0, 1.0);

    
    _grossini->runAction( std::move( rep));
    _tamara->runAction( std::move( bezierEaseTo1));
    _kathia->runAction( std::move( bezierEaseTo2));
    
}

std::string SpriteEaseBezier::subtitle()const
{
    return "SpriteEaseBezier action";
}

//------------------------------------------------------------------
//
// SpriteEaseQuadratic
//
//------------------------------------------------------------------

void SpriteEaseQuadratic::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease_in = std::make_unique<EaseQuadraticActionIn>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_in_back = std::unique_ptr<ActionEase>(move_ease_in->reverse());
    
    auto move_ease_out = std::make_unique<EaseQuadraticActionOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_out_back = std::unique_ptr<ActionEase>(move_ease_out->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease_in),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_in_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq3 = std::make_unique<Sequence>(
        std::move(move_ease_out),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_out_back),
        std::make_unique<DelayTime>(delay)
    );
    
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
    _kathia->runAction( std::make_unique<RepeatForever>( std::move(seq3)));
}

std::string SpriteEaseQuadratic::subtitle() const
{
    return "SpriteEaseQuadratic action";
}

//------------------------------------------------------------------
//
// SpriteEaseQuadraticInOut
//
//------------------------------------------------------------------

void SpriteEaseQuadraticInOut::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease = std::make_unique<EaseQuadraticActionInOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_back = std::unique_ptr<ActionEase>(move_ease->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_back),
        std::make_unique<DelayTime>(delay)
    );
    
    this->positionForTwo();
    
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
}

std::string SpriteEaseQuadraticInOut::subtitle()const
{
    return "SpriteEaseQuadraticInOut action";
}


//------------------------------------------------------------------
//
// SpriteEaseQuartic
//
//------------------------------------------------------------------

void SpriteEaseQuartic::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease_in = std::make_unique<EaseQuarticActionIn>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_in_back = std::unique_ptr<ActionEase>(move_ease_in->reverse());
    
    auto move_ease_out = std::make_unique<EaseQuarticActionOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_out_back = std::unique_ptr<ActionEase>(move_ease_out->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease_in),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_in_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq3 = std::make_unique<Sequence>(
        std::move(move_ease_out),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_out_back),
        std::make_unique<DelayTime>(delay)
    );
    
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
    _kathia->runAction( std::make_unique<RepeatForever>( std::move(seq3)));
}

std::string SpriteEaseQuartic::subtitle()const
{
    return "SpriteEaseQuartic action";
}

//------------------------------------------------------------------
//
// SpriteEaseQuarticInOut
//
//------------------------------------------------------------------

void SpriteEaseQuarticInOut::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease = std::make_unique<EaseQuarticActionInOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_back = std::unique_ptr<ActionEase>(move_ease->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_back),
        std::make_unique<DelayTime>(delay)
    );
    
    this->positionForTwo();
    
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
}

std::string SpriteEaseQuarticInOut::subtitle()const
{
    return "SpriteEaseQuarticInOut action";
}

//------------------------------------------------------------------
//
// SpriteEaseQuintic
//
//------------------------------------------------------------------

void SpriteEaseQuintic::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease_in = std::make_unique<EaseQuinticActionIn>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_in_back = std::unique_ptr<ActionEase>(move_ease_in->reverse());
    
    auto move_ease_out = std::make_unique<EaseQuinticActionOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_out_back = std::unique_ptr<ActionEase>(move_ease_out->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease_in),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_in_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq3 = std::make_unique<Sequence>(
        std::move(move_ease_out),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_out_back),
        std::make_unique<DelayTime>(delay)
    );
    
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
    _kathia->runAction( std::make_unique<RepeatForever>( std::move(seq3)));
}

std::string SpriteEaseQuintic::subtitle()const
{
    return "SpriteEaseQuintic action";
}


//------------------------------------------------------------------
//
// SpriteEaseQuinticInOut
//
//------------------------------------------------------------------

void SpriteEaseQuinticInOut::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease = std::make_unique<EaseQuinticActionInOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_back = std::unique_ptr<ActionEase>(move_ease->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_back),
        std::make_unique<DelayTime>(delay)
    );
    
    this->positionForTwo();
    
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
}

std::string SpriteEaseQuinticInOut::subtitle()const
{
    return "SpriteEaseQuinticInOut action";
}

//------------------------------------------------------------------
//
// SpriteEaseCircle
//
//------------------------------------------------------------------

void SpriteEaseCircle::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease_in = std::make_unique<EaseCircleActionIn>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_in_back = std::unique_ptr<ActionEase>(move_ease_in->reverse());
    
    auto move_ease_out = std::make_unique<EaseCircleActionOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_out_back = std::unique_ptr<ActionEase>(move_ease_out->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease_in),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_in_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq3 = std::make_unique<Sequence>(
        std::move(move_ease_out),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_out_back),
        std::make_unique<DelayTime>(delay)
    );
    
    _grossini->runAction( std::make_unique<RepeatForever>( std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>( std::move(seq2)));
    _kathia->runAction( std::make_unique<RepeatForever>(std::move(seq3)));
}

std::string SpriteEaseCircle::subtitle()const
{
    return "SpriteEaseCircle action";
}


//------------------------------------------------------------------
//
// SpriteEaseCircleInOut
//
//------------------------------------------------------------------

void SpriteEaseCircleInOut::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease = std::make_unique<EaseCircleActionInOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_back = std::unique_ptr<ActionEase>(move_ease->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_back),
        std::make_unique<DelayTime>(delay)
    );
    
    this->positionForTwo();
    
    _grossini->runAction( std::make_unique<RepeatForever>(std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>(std::move(seq2)));
}

std::string SpriteEaseCircleInOut::subtitle()const
{
    return "SpriteEaseCircleInOut action";
}

//------------------------------------------------------------------
//
// SpriteEaseCubic
//
//------------------------------------------------------------------

void SpriteEaseCubic::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease_in = std::make_unique<EaseCubicActionIn>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_in_back = std::unique_ptr<ActionEase>(move_ease_in->reverse());
    
    auto move_ease_out = std::make_unique<EaseCubicActionOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_out_back = std::unique_ptr<ActionEase>(move_ease_out->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease_in),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_in_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq3 = std::make_unique<Sequence>(
        std::move(move_ease_out),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_out_back),
        std::make_unique<DelayTime>(delay)
    );
    
    _grossini->runAction( std::make_unique<RepeatForever>(std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>(std::move(seq2)));
    _kathia->runAction( std::make_unique<RepeatForever>(std::move(seq3)));
}

std::string SpriteEaseCubic::subtitle()const
{
    return "SpriteEaseCubic action";
}


//------------------------------------------------------------------
//
// SpriteEaseCubicInOut
//
//------------------------------------------------------------------

void SpriteEaseCubicInOut::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto move = std::make_unique<MoveBy>(3, Vec2(VisibleRect::right().x-130, 0));
    auto move_back = std::unique_ptr<MoveBy>(move->reverse());
    
    auto move_ease = std::make_unique<EaseCubicActionInOut>(std::unique_ptr<ActionInterval>(move->clone()));
    auto move_ease_back = std::unique_ptr<ActionEase>(move_ease->reverse());
    
    auto delay = 0.25f;
    
    auto seq1 = std::make_unique<Sequence>(
        std::move(move),
        std::make_unique<DelayTime>(delay),
        std::move(move_back),
        std::make_unique<DelayTime>(delay)
    );
    auto seq2 = std::make_unique<Sequence>(
        std::move(move_ease),
        std::make_unique<DelayTime>(delay),
        std::move(move_ease_back),
        std::make_unique<DelayTime>(delay)
    );
    
    this->positionForTwo();
    
    _grossini->runAction( std::make_unique<RepeatForever>(std::move(seq1)));
    _tamara->runAction( std::make_unique<RepeatForever>(std::move(seq2)));
}

std::string SpriteEaseCubicInOut::title()const
{
    return "SpriteEaseCubicInOut action";
}
//------------------------------------------------------------------
//
// SpeedTest
//
//------------------------------------------------------------------

void SpeedTest::onEnter()
{
    EaseSpriteDemo::onEnter();
    
    auto s = Director::getInstance()->getWinSize();

    // rotate and jump
    auto jump1 = std::make_unique<JumpBy>(4, Vec2(-s.width+80, 0), 100, 4);
    auto jump2 = std::unique_ptr<JumpBy>(jump1->reverse());
    auto rot1 = std::make_unique<RotateBy>(4, 360*2);
    auto rot2 = std::unique_ptr<RotateBy>(rot1->reverse());
    
    auto seq3_1 = std::make_unique<Sequence>(
        std::move(jump2),
        std::move(jump1)
    );
    auto seq3_2 = std::make_unique<Sequence>(
        std::move(rot1),
        std::move(rot2)
    );
    auto spawn = std::make_unique<Spawn>(std::move(seq3_1), std::move(seq3_2));
    auto action = std::make_unique<Speed>(std::make_unique<RepeatForever>(std::move(spawn)), 1.0f);
    action->setTag(kTagAction1);
    
    auto action2 = std::unique_ptr<Speed>( action->clone() );
    auto action3 = std::unique_ptr<Speed>( action->clone() );

    action2->setTag(kTagAction1);
    action3->setTag(kTagAction1);
    
    _grossini->runAction( std::move( action2));
    _tamara->runAction( std::move( action3));
    _kathia->runAction( std::move( action));
    
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &SpeedTest::altertime)
            .delay(1.0f)
            .interval(1.0f)
            .paused(isPaused())
    );
}

void SpeedTest::altertime(float /*dt*/)
{
    auto action1 = static_cast<Speed*>(_grossini->getActionByTag(kTagAction1));
    auto action2 = static_cast<Speed*>(_tamara->getActionByTag(kTagAction1));
    auto action3 = static_cast<Speed*>(_kathia->getActionByTag(kTagAction1));
    
    action1->setSpeed( CCRANDOM_MINUS1_1() * 2 );
    action2->setSpeed( CCRANDOM_MINUS1_1() * 2 );
    action3->setSpeed( CCRANDOM_MINUS1_1() * 2 );
}

std::string SpeedTest::subtitle() const
{
    return "Speed action";
}

//------------------------------------------------------------------
//
// EaseSpriteDemo
//
//------------------------------------------------------------------

ActionsEaseTests::ActionsEaseTests()
{
    ADD_TEST_CASE(SpriteEase);
    ADD_TEST_CASE(SpriteEaseInOut);
    ADD_TEST_CASE(SpriteEaseExponential);
    ADD_TEST_CASE(SpriteEaseExponentialInOut);
    ADD_TEST_CASE(SpriteEaseSine);
    ADD_TEST_CASE(SpriteEaseSineInOut);
    ADD_TEST_CASE(SpriteEaseElastic);
    ADD_TEST_CASE(SpriteEaseElasticInOut);
    ADD_TEST_CASE(SpriteEaseBounce);
    ADD_TEST_CASE(SpriteEaseBounceInOut);
    ADD_TEST_CASE(SpriteEaseBack);
    ADD_TEST_CASE(SpriteEaseBackInOut);
    ADD_TEST_CASE(SpriteEaseBezier);
    ADD_TEST_CASE(SpriteEaseQuadratic);
    ADD_TEST_CASE(SpriteEaseQuadraticInOut);
    ADD_TEST_CASE(SpriteEaseQuartic);
    ADD_TEST_CASE(SpriteEaseQuarticInOut);
    ADD_TEST_CASE(SpriteEaseQuintic);
    ADD_TEST_CASE(SpriteEaseQuinticInOut);
    ADD_TEST_CASE(SpriteEaseCircle);
    ADD_TEST_CASE(SpriteEaseCircleInOut);
    ADD_TEST_CASE(SpriteEaseCubic);
    ADD_TEST_CASE(SpriteEaseCubicInOut);
    ADD_TEST_CASE(SpeedTest);
}

EaseSpriteDemo::EaseSpriteDemo(void)
{
}

EaseSpriteDemo::~EaseSpriteDemo(void)
{
    _grossini->release();
    _tamara->release();
    _kathia->release();
}

void EaseSpriteDemo::positionForTwo()
{    
    _grossini->setPosition(VisibleRect::left().x+60, VisibleRect::bottom().y + VisibleRect::getVisibleRect().size.height*1/5);
    _tamara->setPosition(VisibleRect::left().x+60, VisibleRect::bottom().y + VisibleRect::getVisibleRect().size.height*4/5);
    _kathia->setVisible(false);
}


std::string EaseSpriteDemo::subtitle() const
{
    return "No title";
}

void EaseSpriteDemo::onEnter()
{
    TestCase::onEnter();

    // Or you can create an sprite using a filename. PNG and BMP files are supported. Probably TIFF too
    _grossini = Sprite::create(s_pathGrossini); _grossini->retain();
    _tamara = Sprite::create(s_pathSister1); _tamara->retain();
    _kathia = Sprite::create(s_pathSister2); _kathia->retain();
    
    addChild( _grossini, 3);
    addChild( _kathia, 2);
    addChild( _tamara, 1);

    _grossini->setPosition(VisibleRect::left().x + 60, VisibleRect::bottom().y+VisibleRect::getVisibleRect().size.height*1/5);
    _kathia->setPosition(VisibleRect::left().x + 60, VisibleRect::bottom().y+VisibleRect::getVisibleRect().size.height*2.5f/5);
    _tamara->setPosition(VisibleRect::left().x + 60, VisibleRect::bottom().y+VisibleRect::getVisibleRect().size.height*4/5);
}

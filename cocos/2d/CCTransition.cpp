/****************************************************************************
Copyright (c) 2009-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
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

#include "2d/CCTransition.h"
#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"
#include "2d/CCActionEase.h"
#include "2d/CCActionCamera.h"
#include "2d/CCActionTiledGrid.h"
#include "2d/CCActionGrid.h"
#include "2d/CCLayer.h"
#include "2d/CCRenderTexture.h"
#include "2d/CCNodeGrid.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"

namespace cocos2d {

const unsigned int kSceneFade = 0xFADEFADE;

TransitionScene::TransitionScene()
: _inScene(nullptr)
, _outScene(nullptr)
, _duration(0.0f)
, _isInSceneOnTop(false)
, _isSendCleanupToScene(false)
{
}

TransitionScene::~TransitionScene()
{
    CC_SAFE_RELEASE(_inScene);
    CC_SAFE_RELEASE(_outScene);
}

TransitionScene * TransitionScene::create(float t, Scene *scene)
{
    TransitionScene * pScene = new (std::nothrow) TransitionScene();
    if(pScene && pScene->initWithDuration(t,scene))
    {
        pScene->autorelease();
        return pScene;
    }
    CC_SAFE_DELETE(pScene);
    return nullptr;
}

bool TransitionScene::initWithDuration(float t, Scene *scene)
{
    CCASSERT(scene != nullptr, "Argument scene must be non-nil");

    if (Scene::init())
    {
        _duration = t;

        // retain
        _inScene = scene;
        _inScene->retain();
        _outScene = Director::getInstance()->getRunningScene();
        if (_outScene == nullptr)
        {
            _outScene = Scene::create();
        }
        _outScene->retain();

        CCASSERT( _inScene != _outScene, "Incoming scene must be different from the outgoing scene" );
        
        sceneOrder();

        return true;
    }
    else
    {
        return false;
    }
}

void TransitionScene::sceneOrder()
{
    _isInSceneOnTop = true;
}

void TransitionScene::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    Scene::draw(renderer, transform, flags);

    if( _isInSceneOnTop ) {
        _outScene->visit(renderer, transform, flags);
        _inScene->visit(renderer, transform, flags);
    } else {
        _inScene->visit(renderer, transform, flags);
        _outScene->visit(renderer, transform, flags);
    }
}

void TransitionScene::finish()
{
    // clean up
    _inScene->setVisible(true);
    _inScene->setPosition(0,0);
    _inScene->setScale(1.0f);
    _inScene->setRotation(0.0f);
    _inScene->setAdditionalTransform(nullptr);

    _outScene->setVisible(false);
    _outScene->setPosition(0,0);
    _outScene->setScale(1.0f);
    _outScene->setRotation(0.0f);
    _outScene->setAdditionalTransform(nullptr);

    //[self schedule:@selector(setNewScene:) interval:0];
    Director::getInstance()->getScheduler().schedule(
        TimedJob(this, &TransitionScene::setNewScene)
            .repeat(0)
            .paused(isPaused())
    );
}

void TransitionScene::setNewScene(float /*dt*/)
{    
    // Before replacing, save the "send cleanup to scene"
    Director *director = Director::getInstance();
    _isSendCleanupToScene = director->isSendCleanupToScene();
    
    director->replaceScene(_inScene);
    
    // issue #267
    _outScene->setVisible(true);
}

void TransitionScene::hideOutShowIn()
{
    _inScene->setVisible(true);
    _outScene->setVisible(false);
}


// custom onEnter
void TransitionScene::onEnter()
{
    Scene::onEnter();
    
    // disable events while transitions
    _eventDispatcher->setEnabled(false);
    
    // outScene should not receive the onEnter callback
    // only the onExitTransitionDidStart
    _outScene->onExitTransitionDidStart();
    
    _inScene->onEnter();
}

// custom onExit
void TransitionScene::onExit()
{
    Scene::onExit();
    
    // enable events while transitions
    _eventDispatcher->setEnabled(true);
    _outScene->onExit();

    // _inScene should not receive the onEnter callback
    // only the onEnterTransitionDidFinish
    _inScene->onEnterTransitionDidFinish();
}

// custom cleanup
void TransitionScene::cleanup()
{
    Scene::cleanup();

    if( _isSendCleanupToScene )
        _outScene->cleanup();
}

//
// Oriented Transition
//

TransitionSceneOriented::TransitionSceneOriented()
{
}

TransitionSceneOriented::~TransitionSceneOriented()
{
}

TransitionSceneOriented * TransitionSceneOriented::create(float t, Scene *scene, Orientation orientation)
{
    TransitionSceneOriented * newScene = new (std::nothrow) TransitionSceneOriented();
    newScene->initWithDuration(t,scene,orientation);
    newScene->autorelease();
    return newScene;
}

bool TransitionSceneOriented::initWithDuration(float t, Scene *scene, Orientation orientation)
{
    if ( TransitionScene::initWithDuration(t, scene) )
    {
        _orientation = orientation;
    }
    return true;
}

//
// RotoZoom
//
TransitionRotoZoom::TransitionRotoZoom()
{
}

TransitionRotoZoom* TransitionRotoZoom::create(float t, Scene* scene)                   
{                                                               
    TransitionRotoZoom* newScene = new (std::nothrow) TransitionRotoZoom();
    if(newScene && newScene->initWithDuration(t, scene))
    {                                                           
        newScene->autorelease();
        return newScene;
    }                                                           
    CC_SAFE_DELETE(newScene);                                     
    return nullptr;                                                
}

TransitionRotoZoom::~TransitionRotoZoom()
{
}

void TransitionRotoZoom:: onEnter()
{
    TransitionScene::onEnter();

    _inScene->setScale(0.001f);
    _outScene->setScale(1.0f);

    _inScene->setAnchorPoint(Vec2(0.5f, 0.5f));
    _outScene->setAnchorPoint(Vec2(0.5f, 0.5f));

    auto rotozoom = std::unique_ptr<Sequence>(Sequence::create
    (
        to_action_ptr(
            Spawn::create
            (
                ScaleBy::create(_duration/2, 0.001f),
                RotateBy::create(_duration/2, 360 * 2),
                nullptr
            )
        ),
        to_action_ptr( DelayTime::create(_duration/2) )
    ));

    auto rotozoom_reverse = rotozoom->reverse();

    _outScene->runAction( std::move( rotozoom));
    _inScene->runAction
    (
        std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(rotozoom_reverse),
            to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)))
        ))
    );
}

//
// JumpZoom
//
TransitionJumpZoom::TransitionJumpZoom()
{
}
TransitionJumpZoom::~TransitionJumpZoom()
{
}

TransitionJumpZoom* TransitionJumpZoom::create(float t, Scene* scene)
{
    TransitionJumpZoom* newScene = new (std::nothrow) TransitionJumpZoom();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionJumpZoom::onEnter()
{
    TransitionScene::onEnter();
    Size s = Director::getInstance()->getWinSize();

    _inScene->setScale(0.5f);
    _inScene->setPosition(s.width, 0);
    _inScene->setAnchorPoint(Vec2(0.5f, 0.5f));
    _outScene->setAnchorPoint(Vec2(0.5f, 0.5f));

    ActionInterval *jump = JumpBy::create(_duration/4, Vec2(-s.width,0), s.width/4, 2);
    ActionInterval *scaleIn = ScaleTo::create(_duration/4, 1.0f);
    ActionInterval *scaleOut = ScaleTo::create(_duration/4, 0.5f);

    auto jumpZoomOut = std::unique_ptr<Sequence>( Sequence::create(to_action_ptr(scaleOut), to_action_ptr(jump)));
    auto jumpZoomIn = Sequence::create(to_action_ptr(jump), to_action_ptr(scaleIn));

    ActionInterval *delay = DelayTime::create(_duration/2);

    _outScene->runAction( std::move( jumpZoomOut));
    _inScene->runAction
    (
        std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(delay),
            to_action_ptr(jumpZoomIn),
            to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)))
        ))
    );
}

//
// MoveInL
//
TransitionMoveInL::TransitionMoveInL()
{
}

TransitionMoveInL::~TransitionMoveInL()
{
}

TransitionMoveInL* TransitionMoveInL::create(float t, Scene* scene)
{
    TransitionMoveInL* newScene = new (std::nothrow) TransitionMoveInL();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionMoveInL::onEnter()
{
    TransitionScene::onEnter();
    this->initScenes();

    ActionInterval *a = this->action();

    _inScene->runAction
    (
        std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(this->easeActionWithAction(a)),
            to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)))
        ))
    );
}
 
ActionInterval* TransitionMoveInL::action()
{
    return MoveTo::create(_duration, Vec2(0,0));
}

ActionInterval* TransitionMoveInL::easeActionWithAction(ActionInterval* action)
{
    return EaseOut::create(action, 2.0f);
//    return [EaseElasticOut actionWithAction:action period:0.4f];
}

void TransitionMoveInL::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    _inScene->setPosition(-s.width,0);
}

//
// MoveInR
//
TransitionMoveInR::TransitionMoveInR()
{
}
TransitionMoveInR::~TransitionMoveInR()
{
}

TransitionMoveInR* TransitionMoveInR::create(float t, Scene* scene)
{
    TransitionMoveInR* newScene = new (std::nothrow) TransitionMoveInR();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionMoveInR::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    _inScene->setPosition(s.width,0);
}

//
// MoveInT
//
TransitionMoveInT::TransitionMoveInT()
{
}
TransitionMoveInT::~TransitionMoveInT()
{
}

TransitionMoveInT* TransitionMoveInT::create(float t, Scene* scene)
{
    TransitionMoveInT* newScene = new (std::nothrow) TransitionMoveInT();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionMoveInT::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    _inScene->setPosition(0,s.height);
}

//
// MoveInB
//
TransitionMoveInB::TransitionMoveInB()
{
}
TransitionMoveInB::~TransitionMoveInB()
{
}

TransitionMoveInB* TransitionMoveInB::create(float t, Scene* scene)
{
    TransitionMoveInB* newScene = new (std::nothrow) TransitionMoveInB();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionMoveInB::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    _inScene->setPosition(0,-s.height);
}


//
// SlideInL
//

// The adjust factor is needed to prevent issue #442
// One solution is to use DONT_RENDER_IN_SUBPIXELS images, but NO
// The other issue is that in some transitions (and I don't know why)
// the order should be reversed (In in top of Out or vice-versa).
#define ADJUST_FACTOR 0.5f
TransitionSlideInL::TransitionSlideInL()
{
}

TransitionSlideInL::~TransitionSlideInL()
{
}

void TransitionSlideInL::onEnter()
{
    TransitionScene::onEnter();
    this->initScenes();

    ActionInterval *in = this->action();
    ActionInterval *out = this->action();

    auto inAction = std::unique_ptr<ActionInterval>( easeActionWithAction( in));
    auto outAction = std::unique_ptr<Sequence>( Sequence::create
    (
        to_action_ptr(easeActionWithAction(out)),
        to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)))
    ));
    _inScene->runAction( std::move( inAction));
    _outScene->runAction( std::move( outAction));
}

void TransitionSlideInL::sceneOrder()
{
    _isInSceneOnTop = false;
}

void TransitionSlideInL:: initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    _inScene->setPosition(-(s.width-ADJUST_FACTOR),0);
}

ActionInterval* TransitionSlideInL::action()
{
    Size s = Director::getInstance()->getWinSize();
    return MoveBy::create(_duration, Vec2(s.width-ADJUST_FACTOR,0));
}

ActionInterval* TransitionSlideInL::easeActionWithAction(ActionInterval* action)
{
    return EaseOut::create(action, 2.0f);
}

TransitionSlideInL* TransitionSlideInL::create(float t, Scene* scene)
{
    TransitionSlideInL* newScene = new (std::nothrow) TransitionSlideInL();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

//
// SlideInR
//
TransitionSlideInR::TransitionSlideInR()
{
}
TransitionSlideInR::~TransitionSlideInR()
{
}

TransitionSlideInR* TransitionSlideInR::create(float t, Scene* scene)
{
    TransitionSlideInR* newScene = new (std::nothrow) TransitionSlideInR();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionSlideInR::sceneOrder()
{
    _isInSceneOnTop = true;
}

void TransitionSlideInR::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    _inScene->setPosition(s.width-ADJUST_FACTOR,0);
}


ActionInterval* TransitionSlideInR:: action()
{
    Size s = Director::getInstance()->getWinSize();
    return MoveBy::create(_duration, Vec2(-(s.width-ADJUST_FACTOR),0));
}


//
// SlideInT
//
TransitionSlideInT::TransitionSlideInT()
{
}
TransitionSlideInT::~TransitionSlideInT()
{
}

TransitionSlideInT* TransitionSlideInT::create(float t, Scene* scene)
{
    TransitionSlideInT* newScene = new (std::nothrow) TransitionSlideInT();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionSlideInT::sceneOrder()
{
    _isInSceneOnTop = false;
}

void TransitionSlideInT::initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    _inScene->setPosition(0,s.height-ADJUST_FACTOR);
}


ActionInterval* TransitionSlideInT::action()
{
    Size s = Director::getInstance()->getWinSize();
    return MoveBy::create(_duration, Vec2(0,-(s.height-ADJUST_FACTOR)));
}

//
// SlideInB
//
TransitionSlideInB::TransitionSlideInB()
{
}
TransitionSlideInB::~TransitionSlideInB()
{
}

TransitionSlideInB* TransitionSlideInB::create(float t, Scene* scene)
{
    TransitionSlideInB* newScene = new (std::nothrow) TransitionSlideInB();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionSlideInB::sceneOrder()
{
    _isInSceneOnTop = true;
}

void TransitionSlideInB:: initScenes()
{
    Size s = Director::getInstance()->getWinSize();
    _inScene->setPosition(0,-(s.height-ADJUST_FACTOR));
}


ActionInterval* TransitionSlideInB:: action()
{
    Size s = Director::getInstance()->getWinSize();
    return MoveBy::create(_duration, Vec2(0,s.height-ADJUST_FACTOR));
}

//
// ShrinkGrow Transition
//
TransitionShrinkGrow::TransitionShrinkGrow()
{
}
TransitionShrinkGrow::~TransitionShrinkGrow()
{
}

TransitionShrinkGrow* TransitionShrinkGrow::create(float t, Scene* scene)
{
    TransitionShrinkGrow* newScene = new (std::nothrow) TransitionShrinkGrow();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionShrinkGrow::onEnter()
{
    TransitionScene::onEnter();

    _inScene->setScale(0.001f);
    _outScene->setScale(1.0f);

    _inScene->setAnchorPoint(Vec2(2/3.0f,0.5f));
    _outScene->setAnchorPoint(Vec2(1/3.0f,0.5f));    

    ActionInterval* scaleOut = ScaleTo::create(_duration, 0.01f);
    ActionInterval* scaleIn = ScaleTo::create(_duration, 1.0f);

    _inScene->runAction( std::unique_ptr<ActionInterval>( this->easeActionWithAction( scaleIn)));
    _outScene->runAction
    (
        std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(this->easeActionWithAction(scaleOut)),
            to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)))
        ))
    );
}
ActionInterval* TransitionShrinkGrow:: easeActionWithAction(ActionInterval* action)
{
    return EaseOut::create(action, 2.0f);
}

//
// FlipX Transition
//
TransitionFlipX::TransitionFlipX()
{
}
TransitionFlipX::~TransitionFlipX()
{
}

void TransitionFlipX::onEnter()
{
    TransitionSceneOriented::onEnter();

    _inScene->setVisible(false);

    float inDeltaZ, inAngleZ;
    float outDeltaZ, outAngleZ;

    if( _orientation == TransitionScene::Orientation::RIGHT_OVER )
    {
        inDeltaZ = 90;
        inAngleZ = 270;
        outDeltaZ = 90;
        outAngleZ = 0;
    }
    else
    {
        inDeltaZ = -90;
        inAngleZ = 90;
        outDeltaZ = -90;
        outAngleZ = 0;
    }

    auto inA = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(DelayTime::create(_duration/2)),
            to_action_ptr(Show::create()),
            to_action_ptr(OrbitCamera::create(_duration/2, 1, 0, inAngleZ, inDeltaZ, 0, 0)),
            to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)))
        ));

    auto outA = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(OrbitCamera::create(_duration/2, 1, 0, outAngleZ, outDeltaZ, 0, 0)),
            to_action_ptr(Hide::create()),
            to_action_ptr(DelayTime::create(_duration/2))
        ));

    _inScene->runAction( std::move( inA));
    _outScene->runAction( std::move( outA));
}

TransitionFlipX* TransitionFlipX::create(float t, Scene* s, Orientation o)
{
    TransitionFlipX* newScene = new (std::nothrow) TransitionFlipX();
    newScene->initWithDuration(t, s, o);
    newScene->autorelease();

    return newScene;
}

TransitionFlipX* TransitionFlipX::create(float t, Scene* s)
{
    return TransitionFlipX::create(t, s, TransitionScene::Orientation::RIGHT_OVER);
}

//
// FlipY Transition
//
TransitionFlipY::TransitionFlipY()
{
}
TransitionFlipY::~TransitionFlipY()
{
}

void TransitionFlipY::onEnter()
{
    TransitionSceneOriented::onEnter();

    _inScene->setVisible(false);

    float inDeltaZ, inAngleZ;
    float outDeltaZ, outAngleZ;

    if( _orientation == TransitionScene::Orientation::UP_OVER ) 
    {
        inDeltaZ = 90;
        inAngleZ = 270;
        outDeltaZ = 90;
        outAngleZ = 0;
    } 
    else 
    {
        inDeltaZ = -90;
        inAngleZ = 90;
        outDeltaZ = -90;
        outAngleZ = 0;
    }

    auto inA = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(DelayTime::create(_duration/2)),
            to_action_ptr(Show::create()),
            to_action_ptr(OrbitCamera::create(_duration/2, 1, 0, inAngleZ, inDeltaZ, 90, 0)),
            to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)))
        ));
    auto outA = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(OrbitCamera::create(_duration/2, 1, 0, outAngleZ, outDeltaZ, 90, 0)),
            to_action_ptr(Hide::create()),
            to_action_ptr(DelayTime::create(_duration/2))
        ));

    _inScene->runAction( std::move( inA));
    _outScene->runAction( std::move( outA));

}

TransitionFlipY* TransitionFlipY::create(float t, Scene* s, Orientation o)
{
    TransitionFlipY* newScene = new (std::nothrow) TransitionFlipY();
    newScene->initWithDuration(t, s, o);
    newScene->autorelease();

    return newScene;
}

TransitionFlipY* TransitionFlipY::create(float t, Scene* s)
{
    return TransitionFlipY::create(t, s, TransitionScene::Orientation::UP_OVER);
}

//
// FlipAngular Transition
//
TransitionFlipAngular::TransitionFlipAngular()
{
}

TransitionFlipAngular::~TransitionFlipAngular()
{
}

void TransitionFlipAngular::onEnter()
{
    TransitionSceneOriented::onEnter();

    _inScene->setVisible(false);

    float inDeltaZ, inAngleZ;
    float outDeltaZ, outAngleZ;

    if( _orientation == TransitionScene::Orientation::RIGHT_OVER ) 
    {
        inDeltaZ = 90;
        inAngleZ = 270;
        outDeltaZ = 90;
        outAngleZ = 0;
    } 
    else 
    {
        inDeltaZ = -90;
        inAngleZ = 90;
        outDeltaZ = -90;
        outAngleZ = 0;
    }

    auto inA = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(DelayTime::create(_duration/2)),
            to_action_ptr(Show::create()),
            to_action_ptr(OrbitCamera::create(_duration/2, 1, 0, inAngleZ, inDeltaZ, -45, 0)),
            to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)))
        ));
    auto outA = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(OrbitCamera::create(_duration/2, 1, 0, outAngleZ, outDeltaZ, 45, 0)),
            to_action_ptr(Hide::create()),
            to_action_ptr(DelayTime::create(_duration/2))
        ));

    _inScene->runAction( std::move( inA));
    _outScene->runAction( std::move( outA));
}

TransitionFlipAngular* TransitionFlipAngular::create(float t, Scene* s, Orientation o)
{
    TransitionFlipAngular* newScene = new (std::nothrow) TransitionFlipAngular();
    newScene->initWithDuration(t, s, o);
    newScene->autorelease();

    return newScene;
}

TransitionFlipAngular* TransitionFlipAngular::create(float t, Scene* s)
{
    return TransitionFlipAngular::create(t, s, TransitionScene::Orientation::RIGHT_OVER);
}

//
// ZoomFlipX Transition
//
TransitionZoomFlipX::TransitionZoomFlipX()
{
}
TransitionZoomFlipX::~TransitionZoomFlipX()
{
}

void TransitionZoomFlipX::onEnter()
{
    TransitionSceneOriented::onEnter();

    _inScene->setVisible(false);

    float inDeltaZ, inAngleZ;
    float outDeltaZ, outAngleZ;

    if( _orientation == TransitionScene::Orientation::RIGHT_OVER ) {
        inDeltaZ = 90;
        inAngleZ = 270;
        outDeltaZ = 90;
        outAngleZ = 0;
    } 
    else 
    {
        inDeltaZ = -90;
        inAngleZ = 90;
        outDeltaZ = -90;
        outAngleZ = 0;
    }
    auto inA = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(DelayTime::create(_duration/2)),
            to_action_ptr(
                Spawn::create
                (
                    OrbitCamera::create(_duration/2, 1, 0, inAngleZ, inDeltaZ, 0, 0),
                    ScaleTo::create(_duration/2, 1),
                    Show::create(),
                    nullptr
                )
            ),
            to_action_ptr(
                CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this))
            )
        ));
    auto outA = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(
                Spawn::create
                (
                    OrbitCamera::create(_duration/2, 1, 0, outAngleZ, outDeltaZ, 0, 0),
                    ScaleTo::create(_duration/2, 0.5f),
                    nullptr
                )
            ),
            to_action_ptr(Hide::create()),
            to_action_ptr(DelayTime::create(_duration/2))
        ));

    _inScene->setScale(0.5f);
    _inScene->runAction( std::move( inA));
    _outScene->runAction( std::move( outA));
}

TransitionZoomFlipX* TransitionZoomFlipX::create(float t, Scene* s, Orientation o)
{
    TransitionZoomFlipX* newScene = new (std::nothrow) TransitionZoomFlipX();
    newScene->initWithDuration(t, s, o);
    newScene->autorelease();

    return newScene;
}

TransitionZoomFlipX* TransitionZoomFlipX::create(float t, Scene* s)
{
    return TransitionZoomFlipX::create(t, s, TransitionScene::Orientation::RIGHT_OVER);
}

//
// ZoomFlipY Transition
//
TransitionZoomFlipY::TransitionZoomFlipY()
{
}

TransitionZoomFlipY::~TransitionZoomFlipY()
{
}

void TransitionZoomFlipY::onEnter()
{
    TransitionSceneOriented::onEnter();

    _inScene->setVisible(false);

    float inDeltaZ, inAngleZ;
    float outDeltaZ, outAngleZ;

    if( _orientation== TransitionScene::Orientation::UP_OVER ) {
        inDeltaZ = 90;
        inAngleZ = 270;
        outDeltaZ = 90;
        outAngleZ = 0;
    } else {
        inDeltaZ = -90;
        inAngleZ = 90;
        outDeltaZ = -90;
        outAngleZ = 0;
    }

    auto inA = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(DelayTime::create(_duration/2)),
            to_action_ptr(
                Spawn::create
                (
                    OrbitCamera::create(_duration/2, 1, 0, inAngleZ, inDeltaZ, 90, 0),
                    ScaleTo::create(_duration/2, 1),
                    Show::create(),
                    nullptr
                )
            ),
            to_action_ptr(
                CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this))
            )
        ));

    auto outA = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(
                Spawn::create
                (
                    OrbitCamera::create(_duration/2, 1, 0, outAngleZ, outDeltaZ, 90, 0),
                    ScaleTo::create(_duration/2, 0.5f),
                    nullptr
                )
            ),                            
            to_action_ptr(Hide::create()),
            to_action_ptr(DelayTime::create(_duration/2))
        ));

    _inScene->setScale(0.5f);
    _inScene->runAction( std::move( inA));
    _outScene->runAction( std::move( outA));
}

TransitionZoomFlipY* TransitionZoomFlipY::create(float t, Scene* s, Orientation o)
{
    TransitionZoomFlipY* newScene = new (std::nothrow) TransitionZoomFlipY();
    newScene->initWithDuration(t, s, o);
    newScene->autorelease();

    return newScene;
}

TransitionZoomFlipY* TransitionZoomFlipY::create(float t, Scene* s)
{
    return TransitionZoomFlipY::create(t, s, TransitionScene::Orientation::UP_OVER);
}

//
// ZoomFlipAngular Transition
//
TransitionZoomFlipAngular::TransitionZoomFlipAngular()
{
}
TransitionZoomFlipAngular::~TransitionZoomFlipAngular()
{
}


void TransitionZoomFlipAngular::onEnter()
{
    TransitionSceneOriented::onEnter();

    _inScene->setVisible(false);

    float inDeltaZ, inAngleZ;
    float outDeltaZ, outAngleZ;

    if( _orientation == TransitionScene::Orientation::RIGHT_OVER ) {
        inDeltaZ = 90;
        inAngleZ = 270;
        outDeltaZ = 90;
        outAngleZ = 0;
    } 
    else 
    {
        inDeltaZ = -90;
        inAngleZ = 90;
        outDeltaZ = -90;
        outAngleZ = 0;
    }

    auto inA = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(DelayTime::create(_duration/2)),
            to_action_ptr(
                Spawn::create
                (
                    OrbitCamera::create(_duration/2, 1, 0, inAngleZ, inDeltaZ, -45, 0),
                    ScaleTo::create(_duration/2, 1),
                    Show::create(),
                    nullptr
                )
            ),
            to_action_ptr(Show::create()),
            to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)))
        ));
    auto outA = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(
                Spawn::create
                (
                    OrbitCamera::create(_duration/2, 1, 0 , outAngleZ, outDeltaZ, 45, 0),
                    ScaleTo::create(_duration/2, 0.5f),
                    nullptr
                )
            ),
            to_action_ptr(Hide::create()),
            to_action_ptr(DelayTime::create(_duration/2))
        ));

    _inScene->setScale(0.5f);
    _inScene->runAction( std::move( inA));
    _outScene->runAction( std::move( outA));
}

TransitionZoomFlipAngular* TransitionZoomFlipAngular::create(float t, Scene* s, Orientation o)
{
    TransitionZoomFlipAngular* newScene = new (std::nothrow) TransitionZoomFlipAngular();
    newScene->initWithDuration(t, s, o);
    newScene->autorelease();

    return newScene;
}

TransitionZoomFlipAngular* TransitionZoomFlipAngular::create(float t, Scene* s)
{
    return TransitionZoomFlipAngular::create(t, s, TransitionScene::Orientation::RIGHT_OVER);
}

//
// Fade Transition
//
TransitionFade::TransitionFade()
{
}
TransitionFade::~TransitionFade()
{
}

TransitionFade * TransitionFade::create(float duration, Scene *scene, const Color3B& color)
{
    TransitionFade * transition = new (std::nothrow) TransitionFade();
    transition->initWithDuration(duration, scene, color);
    transition->autorelease();
    return transition;
}

TransitionFade* TransitionFade::create(float duration,Scene* scene)
{
    return TransitionFade::create(duration, scene, Color3B::BLACK);
}

bool TransitionFade::initWithDuration(float duration, Scene *scene, const Color3B& color)
{
    if (TransitionScene::initWithDuration(duration, scene))
    {
        _color.r = color.r;
        _color.g = color.g;
        _color.b = color.b;
        _color.a = 0;
    }
    return true;
}

bool TransitionFade::initWithDuration(float t, Scene *scene)
{
    this->initWithDuration(t, scene, Color3B::BLACK);
    return true;
}

void TransitionFade :: onEnter()
{
    TransitionScene::onEnter();

    LayerColor* l = LayerColor::create(_color);
    _inScene->setVisible(false);

    addChild(l, 2, kSceneFade);
    Node* f = getChildByTag(kSceneFade);

    auto a = std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr(FadeIn::create(_duration/2)),
            to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::hideOutShowIn,this))),
            to_action_ptr(FadeOut::create(_duration/2)),
            to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)))
        ));
    f->runAction( std::move( a));
}

void TransitionFade::onExit()
{
    TransitionScene::onExit();
    this->removeChildByTag(kSceneFade, false);
}

//
// Cross Fade Transition
//
TransitionCrossFade::TransitionCrossFade()
{
}
TransitionCrossFade::~TransitionCrossFade()
{
}

TransitionCrossFade* TransitionCrossFade::create(float t, Scene* scene)
{
    TransitionCrossFade* newScene = new (std::nothrow) TransitionCrossFade();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionCrossFade::draw(Renderer* /*renderer*/, const Mat4 &/*transform*/, uint32_t /*flags*/)
{
    // override draw since both scenes (textures) are rendered in 1 scene
}

void TransitionCrossFade::onEnter()
{
    TransitionScene::onEnter();

    // create a transparent color layer
    // in which we are going to add our rendertextures
    Color4B  color(0,0,0,0);
    Size size = Director::getInstance()->getWinSize();
    LayerColor* layer = LayerColor::create(color);

    // create the first render texture for inScene
    RenderTexture* inTexture = RenderTexture::create((int)size.width, (int)size.height,Texture2D::PixelFormat::RGBA8888,GL_DEPTH24_STENCIL8);

    if (nullptr == inTexture)
    {
        return;
    }

    inTexture->getSprite()->setAnchorPoint( Vec2(0.5f,0.5f) );
    inTexture->setPosition(size.width/2, size.height/2);
    inTexture->setAnchorPoint( Vec2(0.5f,0.5f) );

    // render inScene to its texturebuffer
    inTexture->begin();
    _inScene->visit();
    inTexture->end();

    // create the second render texture for outScene
    RenderTexture* outTexture = RenderTexture::create((int)size.width, (int)size.height,Texture2D::PixelFormat::RGBA8888,GL_DEPTH24_STENCIL8);
    outTexture->getSprite()->setAnchorPoint( Vec2(0.5f,0.5f) );
    outTexture->setPosition(size.width/2, size.height/2);
    outTexture->setAnchorPoint( Vec2(0.5f,0.5f) );

    // render outScene to its texturebuffer
    outTexture->begin();
    _outScene->visit();
    outTexture->end();

    // create blend functions

    BlendFunc blend1 = {GL_ONE, GL_ONE}; // inScene will lay on background and will not be used with alpha
    BlendFunc blend2 = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}; // we are going to blend outScene via alpha 

    // set blendfunctions
    inTexture->getSprite()->setBlendFunc(blend1);
    outTexture->getSprite()->setBlendFunc(blend2);    

    // add render textures to the layer
    layer->addChild(inTexture);
    layer->addChild(outTexture);

    // initial opacity:
    inTexture->getSprite()->setOpacity(255);
    outTexture->getSprite()->setOpacity(255);

    // create the blend action
    auto layerAction = std::unique_ptr<Sequence>( Sequence::create
    (
        to_action_ptr(FadeTo::create(_duration, 0)),
        to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::hideOutShowIn,this))),
        to_action_ptr(CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)))
    ));


    // run the blend action
    outTexture->getSprite()->runAction( std::move( layerAction));

    // add the layer (which contains our two rendertextures) to the scene
    addChild(layer, 2, kSceneFade);
}

// clean up on exit
void TransitionCrossFade::onExit()
{
    // remove our layer and release all containing objects 
    this->removeChildByTag(kSceneFade, false);
    TransitionScene::onExit();
}

//
// TurnOffTilesTransition
//
TransitionTurnOffTiles::TransitionTurnOffTiles()
{
    _outSceneProxy = NodeGrid::create();
    _outSceneProxy->retain();
}

TransitionTurnOffTiles::~TransitionTurnOffTiles()
{
    CC_SAFE_RELEASE(_outSceneProxy);
}

TransitionTurnOffTiles* TransitionTurnOffTiles::create(float t, Scene* scene)
{
    TransitionTurnOffTiles* newScene = new (std::nothrow) TransitionTurnOffTiles();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

// override addScenes, and change the order
void TransitionTurnOffTiles::sceneOrder()
{
    _isInSceneOnTop = false;
}

void TransitionTurnOffTiles::onEnter()
{
    TransitionScene::onEnter();
    _outSceneProxy->setTarget(_outScene);
    _outSceneProxy->onEnter();

    Size s = Director::getInstance()->getWinSize();
    float aspect = s.width / s.height;
    int x = (int)(12 * aspect);
    int y = 12;

    TurnOffTiles* toff = TurnOffTiles::create(_duration, Size(x,y));
    ActionInterval* action = easeActionWithAction(toff);
    _outSceneProxy->runAction
    (
        std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr<FiniteTimeAction>(action),
            to_action_ptr<FiniteTimeAction>(
                CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this))
            ),
            to_action_ptr<FiniteTimeAction>(
                StopGrid::create()
            )
        ))
    );
}

void TransitionTurnOffTiles::onExit()
{
    _outSceneProxy->setTarget(nullptr);
    _outSceneProxy->onExit();
    TransitionScene::onExit();
}

void TransitionTurnOffTiles::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    Scene::draw(renderer, transform, flags);
    
    if( _isInSceneOnTop )
    {
        _outSceneProxy->visit(renderer, transform, flags);
        _inScene->visit(renderer, transform, flags);
    } 
    else
    {
        _inScene->visit(renderer, transform, flags);
        _outSceneProxy->visit(renderer, transform, flags);
    }
}


ActionInterval* TransitionTurnOffTiles:: easeActionWithAction(ActionInterval* action)
{
    return action;
}

//
// SplitCols Transition
//
TransitionSplitCols::TransitionSplitCols()
{
    _gridProxy = NodeGrid::create();
    _gridProxy->retain();
}
TransitionSplitCols::~TransitionSplitCols()
{
    CC_SAFE_RELEASE(_gridProxy);
}

TransitionSplitCols* TransitionSplitCols::create(float t, Scene* scene)
{
    TransitionSplitCols* newScene = new (std::nothrow) TransitionSplitCols();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionSplitCols::onEnter()
{
    TransitionScene::onEnter();

    _gridProxy->setTarget(_outScene);
    _gridProxy->onEnter();

    ActionInterval* split = action();
    ActionInterval* split_reverse = split->reverse();

    auto seq = Sequence::create
    (
        to_action_ptr<FiniteTimeAction>(split),
        to_action_ptr<FiniteTimeAction>(
                CallFunc::create(CC_CALLBACK_0(TransitionSplitCols::switchTargetToInscene,this))
        ),
        to_action_ptr<FiniteTimeAction>(split_reverse)
    );

    _gridProxy->runAction
    ( 
        std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr<FiniteTimeAction>(
                easeActionWithAction(seq)
            ),
            to_action_ptr<FiniteTimeAction>(
                CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,
                                               this))
            ),
            to_action_ptr<FiniteTimeAction>(StopGrid::create())
        ))
    );
}

void TransitionSplitCols::switchTargetToInscene()
{
    _gridProxy->setTarget(_inScene);
}

void TransitionSplitCols::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    Scene::draw(renderer, transform, flags);
    _gridProxy->visit(renderer, transform, flags);
}

void TransitionSplitCols::onExit()
{
    _gridProxy->setTarget(nullptr);
    _gridProxy->onExit();
    TransitionScene::onExit();
}

ActionInterval* TransitionSplitCols:: action()
{
    return SplitCols::create(_duration/2.0f, 3);
}


ActionInterval* TransitionSplitCols::easeActionWithAction(ActionInterval * action)
{
    return EaseInOut::create(action, 3.0f);
}


//
// SplitRows Transition
//
TransitionSplitRows::TransitionSplitRows()
{
}

TransitionSplitRows::~TransitionSplitRows()
{
}

ActionInterval* TransitionSplitRows::action()
{
    return SplitRows::create(_duration/2.0f, 3);
}

TransitionSplitRows* TransitionSplitRows::create(float t, Scene* scene)
{
    TransitionSplitRows* newScene = new (std::nothrow) TransitionSplitRows();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

//
// FadeTR Transition
//
TransitionFadeTR::TransitionFadeTR()
{
    _outSceneProxy = NodeGrid::create();
    _outSceneProxy->retain();
}
TransitionFadeTR::~TransitionFadeTR()
{
    CC_SAFE_RELEASE(_outSceneProxy);
}

TransitionFadeTR* TransitionFadeTR::create(float t, Scene* scene)
{
    TransitionFadeTR* newScene = new (std::nothrow) TransitionFadeTR();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionFadeTR::sceneOrder()
{
    _isInSceneOnTop = false;
}

void TransitionFadeTR::onEnter()
{
    TransitionScene::onEnter();

    _outSceneProxy->setTarget(_outScene);
    _outSceneProxy->onEnter();

    Size s = Director::getInstance()->getWinSize();
    float aspect = s.width / s.height;
    int x = (int)(12 * aspect);
    int y = 12;

    ActionInterval* action  = actionWithSize(Size(x,y));

    _outSceneProxy->runAction
    (
        std::unique_ptr<Sequence>( Sequence::create
        (
            to_action_ptr<FiniteTimeAction>(
                easeActionWithAction(action)
            ),
            to_action_ptr<FiniteTimeAction>(
                CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,
                                               this))
            ),
            to_action_ptr<FiniteTimeAction>(
                StopGrid::create()
            )
        ))
    );
}

void TransitionFadeTR::onExit()
{
    _outSceneProxy->setTarget(nullptr);
    _outSceneProxy->onExit();
    TransitionScene::onExit();
}

void TransitionFadeTR::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    Scene::draw(renderer, transform, flags);
    
    if( _isInSceneOnTop )
    {
        _outSceneProxy->visit(renderer, transform, flags);
        _inScene->visit(renderer, transform, flags);
    } 
    else
    {
        _inScene->visit(renderer, transform, flags);
        _outSceneProxy->visit(renderer, transform, flags);
    }
}

ActionInterval*  TransitionFadeTR::actionWithSize(const Size& size)
{
    return FadeOutTRTiles::create(_duration, size);
}

ActionInterval* TransitionFadeTR:: easeActionWithAction(ActionInterval* action)
{
    return action;
}


//
// FadeBL Transition
//

TransitionFadeBL::TransitionFadeBL()
{
}
TransitionFadeBL::~TransitionFadeBL()
{
}

TransitionFadeBL* TransitionFadeBL::create(float t, Scene* scene)
{
    TransitionFadeBL* newScene = new (std::nothrow) TransitionFadeBL();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

ActionInterval*  TransitionFadeBL::actionWithSize(const Size& size)
{
    return FadeOutBLTiles::create(_duration, size);
}

//
// FadeUp Transition
//
TransitionFadeUp::TransitionFadeUp()
{
}

TransitionFadeUp::~TransitionFadeUp()
{
}

TransitionFadeUp* TransitionFadeUp::create(float t, Scene* scene)
{
    TransitionFadeUp* newScene = new (std::nothrow) TransitionFadeUp();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

ActionInterval* TransitionFadeUp::actionWithSize(const Size& size)
{
    return FadeOutUpTiles::create(_duration, size);
}

//
// FadeDown Transition
//
TransitionFadeDown::TransitionFadeDown()
{
}
TransitionFadeDown::~TransitionFadeDown()
{
}

TransitionFadeDown* TransitionFadeDown::create(float t, Scene* scene)
{
    TransitionFadeDown* newScene = new (std::nothrow) TransitionFadeDown();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

ActionInterval* TransitionFadeDown::actionWithSize(const Size& size)
{
    return FadeOutDownTiles::create(_duration, size);
}

} // namespace cocos2d

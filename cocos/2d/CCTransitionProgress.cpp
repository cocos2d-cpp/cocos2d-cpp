/****************************************************************************
Copyright (c) 2009      Lam Pham
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2012      Ricardo Quesada
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


#include "2d/CCTransitionProgress.h"
#include "base/CCDirector.h"
#include "2d/CCRenderTexture.h"
#include "2d/CCProgressTimer.h"
#include "2d/CCActionInstant.h"
#include "2d/CCActionProgressTimer.h"

namespace cocos2d {

enum {
    kSceneRadial = 0xc001,
};

TransitionProgress::TransitionProgress()
: _to(0.0f)
, _from(0.0f)
, _sceneToBeModified(nullptr)
{

}

// TransitionProgress
void TransitionProgress::onEnter()
{
    TransitionScene::onEnter();

    setupTransition();
    
    // create a transparent color layer
    // in which we are going to add our rendertextures
    Size size = Director::getInstance()->getWinSize();

    // create the second render texture for outScene
    RenderTexture *texture = RenderTexture::create((int)size.width,
                                                   (int)size.height,
                                                   Texture2D::PixelFormat::RGBA8888,
                                                   GL_DEPTH24_STENCIL8);
    texture->getSprite()->setAnchorPoint(Vec2(0.5f,0.5f));
    texture->setPosition(size.width/2, size.height/2);
    texture->setAnchorPoint(Vec2(0.5f,0.5f));

    // render outScene to its texturebuffer
    texture->beginWithClear(0, 0, 0, 1);
    _sceneToBeModified->visit();
    texture->end();


    //    Since we've passed the outScene to the texture we don't need it.
    if (_sceneToBeModified == _outScene)
    {
        hideOutShowIn();
    }
    //    We need the texture in RenderTexture.
    auto node = createProgressTimerNodeWithRenderTexture(texture);

    // create the blend action
    auto layerAction = std::make_unique<Sequence>
        (
            std::make_unique<ProgressFromTo>(_duration, _from, _to),
            std::make_unique<CallFunc>(CC_CALLBACK_0(TransitionScene::finish,this))
        );
    // run the blend action
    node->runAction( std::move( layerAction));

    // add the layer (which contains our two rendertextures) to the scene
    addChild(std::move(node), 2, kSceneRadial);
}

// clean up on exit
void TransitionProgress::onExit()
{
    // remove our layer and release all containing objects
    removeChildByTag(kSceneRadial, true);
    TransitionScene::onExit();
}

void TransitionProgress::sceneOrder()
{
    _isInSceneOnTop = false;
}

void TransitionProgress::setupTransition()
{
    _sceneToBeModified = _outScene;
    _from = 100;
    _to = 0;
}

// TransitionProgressRadialCCW

node_ptr<ProgressTimer> TransitionProgressRadialCCW::createProgressTimerNodeWithRenderTexture(RenderTexture* texture)
{
    Size size = Director::getInstance()->getWinSize();

    auto node = make_node_ptr<ProgressTimer>(texture->getSprite());

    // but it is flipped upside down so we flip the sprite
    node->getSprite()->setFlippedY(true);
    node->setType(ProgressTimer::Type::RADIAL);

    //    Return the radial type that we want to use
    node->setReverseDirection(false);
    node->setPercentage(100);
    node->setPosition(size.width/2, size.height/2);
    node->setAnchorPoint(Vec2(0.5f,0.5f));
    
    return std::move(node);
}

TransitionProgressRadialCCW* TransitionProgressRadialCCW::create(float t, Scene* scene)
{
    TransitionProgressRadialCCW* newScene = new (std::nothrow) TransitionProgressRadialCCW();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

// TransitionProgressRadialCW
TransitionProgressRadialCW* TransitionProgressRadialCW::create(float t, Scene* scene)
{
    TransitionProgressRadialCW* newScene = new (std::nothrow) TransitionProgressRadialCW();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

node_ptr<ProgressTimer> TransitionProgressRadialCW::createProgressTimerNodeWithRenderTexture(RenderTexture* texture)
{
    Size size = Director::getInstance()->getWinSize();
    
    auto node = make_node_ptr<ProgressTimer>(texture->getSprite());
    
    // but it is flipped upside down so we flip the sprite
    node->getSprite()->setFlippedY(true);
    node->setType( ProgressTimer::Type::RADIAL );
    
    //    Return the radial type that we want to use
    node->setReverseDirection(true);
    node->setPercentage(100);
    node->setPosition(size.width/2, size.height/2);
    node->setAnchorPoint(Vec2(0.5f,0.5f));
    
    return std::move(node);
}

// TransitionProgressHorizontal
TransitionProgressHorizontal* TransitionProgressHorizontal::create(float t, Scene* scene)
{
    TransitionProgressHorizontal* newScene = new (std::nothrow) TransitionProgressHorizontal();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

node_ptr<ProgressTimer> TransitionProgressHorizontal::createProgressTimerNodeWithRenderTexture(RenderTexture* texture)
{    
    Size size = Director::getInstance()->getWinSize();

    auto node = make_node_ptr<ProgressTimer>(texture->getSprite());
    
    // but it is flipped upside down so we flip the sprite
    node->getSprite()->setFlippedY(true);
    node->setType( ProgressTimer::Type::BAR);
    
    node->setMidpoint(Vec2(1, 0));
    node->setBarChangeRate(Vec2(1,0));
    
    node->setPercentage(100);
    node->setPosition(size.width/2, size.height/2);
    node->setAnchorPoint(Vec2(0.5f,0.5f));

    return std::move(node);
}

// TransitionProgressVertical
TransitionProgressVertical* TransitionProgressVertical::create(float t, Scene* scene)
{
    TransitionProgressVertical* newScene = new (std::nothrow) TransitionProgressVertical();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

node_ptr<ProgressTimer> TransitionProgressVertical::createProgressTimerNodeWithRenderTexture(RenderTexture* texture)
{    
    Size size = Director::getInstance()->getWinSize();
    
    auto node = make_node_ptr<ProgressTimer>(texture->getSprite());
    
    // but it is flipped upside down so we flip the sprite
    node->getSprite()->setFlippedY(true);
    node->setType(ProgressTimer::Type::BAR);
    
    node->setMidpoint(Vec2(0, 0));
    node->setBarChangeRate(Vec2(0,1));
    
    node->setPercentage(100);
    node->setPosition(size.width/2, size.height/2);
    node->setAnchorPoint(Vec2(0.5f,0.5f));
    
    return std::move(node);
}


// TransitionProgressInOut
TransitionProgressInOut* TransitionProgressInOut::create(float t, Scene* scene)
{
    TransitionProgressInOut* newScene = new (std::nothrow) TransitionProgressInOut();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

void TransitionProgressInOut::sceneOrder()
{
    _isInSceneOnTop = false;
}

void TransitionProgressInOut::setupTransition()
{
    _sceneToBeModified = _inScene;
    _from = 0;
    _to = 100;    
}

node_ptr<ProgressTimer> TransitionProgressInOut::createProgressTimerNodeWithRenderTexture(RenderTexture* texture)
{    
    Size size = Director::getInstance()->getWinSize();
    
    auto node = make_node_ptr<ProgressTimer>(texture->getSprite());
    
    // but it is flipped upside down so we flip the sprite
    node->getSprite()->setFlippedY(true);
    node->setType( ProgressTimer::Type::BAR);
    
    node->setMidpoint(Vec2(0.5f, 0.5f));
    node->setBarChangeRate(Vec2(1, 1));
    
    node->setPercentage(0);
    node->setPosition(size.width/2, size.height/2);
    node->setAnchorPoint(Vec2(0.5f,0.5f));
    
    return std::move(node);
}


// TransitionProgressOutIn
TransitionProgressOutIn* TransitionProgressOutIn::create(float t, Scene* scene)
{
    TransitionProgressOutIn* newScene = new (std::nothrow) TransitionProgressOutIn();
    if(newScene && newScene->initWithDuration(t, scene))
    {
        newScene->autorelease();
        return newScene;
    }
    CC_SAFE_DELETE(newScene);
    return nullptr;
}

node_ptr<ProgressTimer> TransitionProgressOutIn::createProgressTimerNodeWithRenderTexture(RenderTexture* texture)
{    
    Size size = Director::getInstance()->getWinSize();
    
    auto node = make_node_ptr<ProgressTimer>(texture->getSprite());
    
    // but it is flipped upside down so we flip the sprite
    node->getSprite()->setFlippedY(true);
    node->setType( ProgressTimer::Type::BAR );
    
    node->setMidpoint(Vec2(0.5f, 0.5f));
    node->setBarChangeRate(Vec2(1, 1));
    
    node->setPercentage(100);
    node->setPosition(size.width/2, size.height/2);
    node->setAnchorPoint(Vec2(0.5f,0.5f));
    
    return std::move(node);
}

} // namespace cocos2d

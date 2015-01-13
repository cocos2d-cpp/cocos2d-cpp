/****************************************************************************
Copyright (c) 2013 cocos2d-x.org

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

#include "CCFrame.h"
#include "CCTimeLine.h"
#include "CCActionTimeline.h"
#include "2d/CCSpriteFrameCache.h"
#include "2d/CCSpriteFrame.h"
#include <exception>
#include <iostream>

USING_NS_CC;

NS_TIMELINE_BEGIN

// Frame
Frame::Frame()
    : _frameIndex(0)
    , _tween(true)
    , _enterWhenPassed(false)
    , _timeline(nullptr)
    , _node(nullptr)
{
}

Frame::~Frame()
{
}

void Frame::emitEvent()
{
    if (_timeline)
    {
        _timeline->getActionTimeline()->emitFrameEvent(this);
    }
}

void Frame::cloneProperty(Frame* frame)
{
    _frameIndex = frame->getFrameIndex();
    _tween = frame->isTween();
}


// VisibleFrame
VisibleFrame* VisibleFrame::create()
{
    VisibleFrame* frame = new (std::nothrow) VisibleFrame();
    if (frame)
    {
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

VisibleFrame::VisibleFrame()
    : _visible(true)
{
}

void VisibleFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    _node->setVisible(_visible);
}


Frame* VisibleFrame::clone()
{
    VisibleFrame* frame = VisibleFrame::create();
    frame->setVisible(_visible);

    frame->cloneProperty(this);

    return frame;
}



// TextureFrame
TextureFrame* TextureFrame::create()
{
    TextureFrame* frame = new (std::nothrow) TextureFrame();
    if (frame)
    {
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

TextureFrame::TextureFrame()
    : _textureName("")
{
}

void TextureFrame::setNode(Node* node)
{
    Frame::setNode(node);

    _sprite = dynamic_cast<Sprite*>(node);
}

void TextureFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    if(_sprite)
    {
        SpriteFrame* spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(_textureName);

        if(spriteFrame != nullptr)
            _sprite->setSpriteFrame(spriteFrame);
        else
            _sprite->setTexture(_textureName);
    }
}


Frame* TextureFrame::clone()
{
    TextureFrame* frame = TextureFrame::create();
    frame->setTextureName(_textureName);

    frame->cloneProperty(this);

    return frame;
}



// RotationFrame
RotationFrame* RotationFrame::create()
{
    RotationFrame* frame = new (std::nothrow) RotationFrame();
    if (frame)
    {
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

RotationFrame::RotationFrame()
    : _rotation(0)
{
}

void RotationFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    _node->setRotation(_rotation);

    if(_tween)
    {
        _betwennRotation = static_cast<RotationFrame*>(nextFrame)->_rotation - _rotation;
    }
}

void RotationFrame::apply(float percent)
{
    if (_tween && _betwennRotation != 0)
    {
        float rotation = _rotation + percent * _betwennRotation;
        _node->setRotation(rotation);
    }
}

Frame* RotationFrame::clone()
{
    RotationFrame* frame = RotationFrame::create();
    frame->setRotation(_rotation);

    frame->cloneProperty(this);

    return frame;
}



// SkewFrame
SkewFrame* SkewFrame::create()
{
    SkewFrame* frame = new (std::nothrow) SkewFrame();
    if (frame)
    {
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

SkewFrame::SkewFrame()
    : _skewX(0)
    , _skewY(0)
{
}

void SkewFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    _node->setSkewX(_skewX);
    _node->setSkewY(_skewY);

    if(_tween)
    {
        _betweenSkewX = static_cast<SkewFrame*>(nextFrame)->_skewX - _skewX;
        _betweenSkewY = static_cast<SkewFrame*>(nextFrame)->_skewY - _skewY;
    }
}

void SkewFrame::apply(float percent)
{
    if (_tween && (_betweenSkewX != 0 || _betweenSkewY != 0))
    {
        float skewx = _skewX + percent * _betweenSkewX;
        float skewy = _skewY + percent * _betweenSkewY;

        _node->setSkewX(skewx);
        _node->setSkewY(skewy);
    }
}

Frame* SkewFrame::clone()
{
    SkewFrame* frame = SkewFrame::create();
    frame->setSkewX(_skewX);
    frame->setSkewY(_skewY);

    frame->cloneProperty(this);

    return frame;
}




// RotationSkewFrame
RotationSkewFrame* RotationSkewFrame::create()
{
    RotationSkewFrame* frame = new (std::nothrow) RotationSkewFrame();
    if (frame)
    {
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

RotationSkewFrame::RotationSkewFrame()
{
}

void RotationSkewFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    _node->setRotationSkewX(_skewX);
    _node->setRotationSkewY(_skewY);

    if (_tween)
    {
        _betweenSkewX = static_cast<RotationSkewFrame*>(nextFrame)->_skewX - _skewX;
        _betweenSkewY = static_cast<RotationSkewFrame*>(nextFrame)->_skewY - _skewY;
    }
}

void RotationSkewFrame::apply(float percent)
{
    if (_tween && (_betweenSkewX != 0 || _betweenSkewY != 0))
    {
        float skewx = _skewX + percent * _betweenSkewX;
        float skewy = _skewY + percent * _betweenSkewY;

        _node->setRotationSkewX(skewx);
        _node->setRotationSkewY(skewy);
    }
}

Frame* RotationSkewFrame::clone()
{
    RotationSkewFrame* frame = RotationSkewFrame::create();
    frame->setSkewX(_skewX);
    frame->setSkewY(_skewY);

    frame->cloneProperty(this);

    return frame;
}


// PositionFrame
PositionFrame* PositionFrame::create()
{
    PositionFrame* frame = new (std::nothrow) PositionFrame();
    if (frame)
    {
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

PositionFrame::PositionFrame()
    : _position(0,0)
{
}

void PositionFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    _node->setPosition(_position);

    if(_tween)
    {
        _betweenX = static_cast<PositionFrame*>(nextFrame)->_position.x - _position.x;
        _betweenY = static_cast<PositionFrame*>(nextFrame)->_position.y - _position.y;
    }
}

void PositionFrame::apply(float percent)
{
    if (_tween && (_betweenX != 0 || _betweenY != 0))
    {
        Point p;
        p.x = _position.x + _betweenX * percent;
        p.y = _position.y + _betweenY * percent;

        _node->setPosition(p);
    }
}

Frame* PositionFrame::clone()
{
    PositionFrame* frame = PositionFrame::create();
    frame->setPosition(_position);

    frame->cloneProperty(this);

    return frame;
}


// ScaleFrame
ScaleFrame* ScaleFrame::create()
{
    ScaleFrame* frame = new (std::nothrow) ScaleFrame();
    if (frame)
    {
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

ScaleFrame::ScaleFrame()
    : _scaleX(1)
    , _scaleY(1)
{
}

void ScaleFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    _node->setScaleX(_scaleX);
    _node->setScaleY(_scaleY);

    if(_tween)
    {
        _betweenScaleX = static_cast<ScaleFrame*>(nextFrame)->_scaleX - _scaleX;
        _betweenScaleY = static_cast<ScaleFrame*>(nextFrame)->_scaleY - _scaleY;
    }
}

void ScaleFrame::apply(float percent)
{
    if (_tween && (_betweenScaleX != 0 || _betweenScaleY != 0))
    {
        float scaleX = _scaleX + _betweenScaleX * percent;
        float scaleY = _scaleY + _betweenScaleY * percent;

        _node->setScaleX(scaleX);
        _node->setScaleY(scaleY);
    }
}

Frame* ScaleFrame::clone()
{
    ScaleFrame* frame = ScaleFrame::create();
    frame->setScaleX(_scaleX);
    frame->setScaleY(_scaleY);  

    frame->cloneProperty(this);

    return frame;
}


// AnchorPointFrame
AnchorPointFrame* AnchorPointFrame::create()
{
    AnchorPointFrame* frame = new (std::nothrow) AnchorPointFrame();
    if (frame)
    {
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

AnchorPointFrame::AnchorPointFrame()
    : _anchorPoint(0.5f,0.5f)
{
}

void AnchorPointFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    _node->setAnchorPoint(_anchorPoint);
}


Frame* AnchorPointFrame::clone()
{
    AnchorPointFrame* frame = AnchorPointFrame::create();
    frame->setAnchorPoint(_anchorPoint);

    frame->cloneProperty(this);

    return frame;
}



// InnerActionFrame
const std::string InnerActionFrame::AnimationAllName = "-- ALL --";

InnerActionFrame* InnerActionFrame::create()
{
    InnerActionFrame* frame = new (std::nothrow) InnerActionFrame();
    if (frame)
    {
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

InnerActionFrame::InnerActionFrame()
: _innerActionType(InnerActionType::SingleFrame)
, _startFrameIndex(0)
, _endFrameIndex(0)
, _singleFrameIndex(0)
, _enterWithName(false)
, _animationName("")
{

}

void InnerActionFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    int start = _startFrameIndex;
    int end = _endFrameIndex;
    auto actiontimeline = static_cast<ActionTimeline*>(_node->getActionByTag(_node->getTag()));
    if (InnerActionType::SingleFrame == _innerActionType)
    {
        actiontimeline->gotoFrameAndPause(_singleFrameIndex);
        return;
    }
    
    if (_enterWithName)
    {
        if (_animationName == AnimationAllName)
        {
            start = 0;
            end = actiontimeline->getDuration();
        }
        else if(actiontimeline->IsAnimationInfoExists(_animationName))
        {
            AnimationInfo info = actiontimeline->getAnimationInfo(_animationName);
            start = info.startIndex;
            end = info.endIndex;
        }
        else
        {
            CCLOG("Animation %s not exists!", _animationName.c_str());
        }
    }
    
    int duration = _timeline->getActionTimeline()->getDuration();
    int odddiff = duration - _frameIndex - end + start;
    if (odddiff < 0)
    {
       end += odddiff;
    }
    
    if (InnerActionType::NoLoopAction == _innerActionType)
    {
        actiontimeline->gotoFrameAndPlay(start, end, false);
    }
    else if (InnerActionType::LoopAction == _innerActionType)
    {
        actiontimeline->gotoFrameAndPlay(start, end, true);
    }
}

void InnerActionFrame::setStartFrameIndex(int frameIndex) throw()
{
    if(_enterWithName)
    {
        CCLOG(" cannot set start when enter frame with name. setEnterWithName false firstly!");
        throw std::exception();
    }
    _startFrameIndex = frameIndex;
}


void InnerActionFrame::setEndFrameIndex(int frameIndex) throw()
{
    if(_enterWithName)
    {
         CCLOG(" cannot set end when enter frame with name. setEnterWithName false firstly!");
        throw std::exception();
    }
    _endFrameIndex = frameIndex;
}

void InnerActionFrame::setAnimationName(const std::string& animationName) throw()
{
    if(!_enterWithName)
    {
         CCLOG(" cannot set aniamtioname when enter frame with index. setEnterWithName true firstly!");
        throw std::exception();
    }
    _animationName = animationName;
   
}

Frame* InnerActionFrame::clone()
{
    InnerActionFrame* frame = InnerActionFrame::create();
    frame->setInnerActionType(_innerActionType);
    frame->setSingleFrameIndex(_singleFrameIndex);
    if(_enterWithName)
    {
        frame->setEnterWithName(true);
        frame->setAnimationName(_animationName);
    }
    else
    {
        frame->setStartFrameIndex(_startFrameIndex);
        frame->setEndFrameIndex(_endFrameIndex);
    }
    frame->cloneProperty(this);

    return frame;
}


// ColorFrame
ColorFrame* ColorFrame::create()
{
    ColorFrame* frame = new (std::nothrow) ColorFrame();
    if (frame)
    {
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

ColorFrame::ColorFrame()
    : _alpha(255)
    , _color(Color3B(255, 255, 255))
{
}

void ColorFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    _node->setOpacity(_alpha);
    _node->setColor(_color);

    if(_tween)
    {
        _betweenAlpha = static_cast<ColorFrame*>(nextFrame)->_alpha - _alpha;

        const Color3B& color = static_cast<ColorFrame*>(nextFrame)->_color;
        _betweenRed   = color.r - _color.r;
        _betweenGreen = color.g - _color.g;
        _betweenBlue  = color.b - _color.b;
    }
}

void ColorFrame::apply(float percent)
{
    if (_tween && (_betweenAlpha !=0 || _betweenRed != 0 || _betweenGreen != 0 || _betweenBlue != 0))
    {
        GLubyte alpha = _alpha + _betweenAlpha * percent;

        Color3B color;
        color.r = _color.r+ _betweenRed   * percent;
        color.g = _color.g+ _betweenGreen * percent;
        color.b = _color.b+ _betweenBlue  * percent;

        _node->setOpacity(alpha);
        _node->setColor(color);
    }
}

Frame* ColorFrame::clone()
{
    ColorFrame* frame = ColorFrame::create();
    frame->setAlpha(_alpha);
    frame->setColor(_color);

    frame->cloneProperty(this);

    return frame;
}

// AlphaFrame
AlphaFrame* AlphaFrame::create()
{
    AlphaFrame* frame = new (std::nothrow) AlphaFrame();
    if (frame)
    {
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

AlphaFrame::AlphaFrame()
    : _alpha(255)
{
}

void AlphaFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    _node->setOpacity(_alpha);

    if (_tween)
    {
        _betweenAlpha = static_cast<AlphaFrame*>(nextFrame)->_alpha - _alpha;
    }
}

void AlphaFrame::apply(float percent)
{
    if (_tween)
    {
        GLubyte alpha = _alpha + _betweenAlpha * percent;
        _node->setOpacity(alpha);
    }
}

Frame* AlphaFrame::clone()
{
    AlphaFrame* frame = AlphaFrame::create();
    frame->setAlpha(_alpha);
    frame->cloneProperty(this);

    return frame;
}

// EventFrame
EventFrame* EventFrame::create()
{
    EventFrame* frame = new (std::nothrow) EventFrame();
    if (frame)
    {
        frame->init();
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

void EventFrame::init()
{
    _enterWhenPassed = true;
}

EventFrame::EventFrame()
    : _event("")
    , _action(nullptr)
{
}

void EventFrame::setNode(cocos2d::Node* node)
{
    Frame::setNode(node);
    _action = _timeline->getActionTimeline();
}

void EventFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    if(_frameIndex < _action->getStartFrame() || _frameIndex > _action->getEndFrame())
        return;

    if(currentFrameIndex >= _frameIndex)
        emitEvent();
}


Frame* EventFrame::clone()
{
    EventFrame* frame = EventFrame::create();
    frame->setEvent(_event);

    frame->cloneProperty(this);

    return frame;
}


// ZOrderFrame
ZOrderFrame* ZOrderFrame::create()
{
    ZOrderFrame* frame = new (std::nothrow) ZOrderFrame();
    if (frame)
    {
        frame->autorelease();
        return frame;
    }
    CC_SAFE_DELETE(frame);
    return nullptr;
}

ZOrderFrame::ZOrderFrame()
    : _zorder(0)
{
}

void ZOrderFrame::onEnter(Frame *nextFrame, int currentFrameIndex)
{
    if(_node)
        _node->setLocalZOrder(_zorder);
}


Frame* ZOrderFrame::clone()
{
    ZOrderFrame* frame = ZOrderFrame::create();
    frame->setZOrder(_zorder);

    frame->cloneProperty(this);

    return frame;
}

NS_TIMELINE_END

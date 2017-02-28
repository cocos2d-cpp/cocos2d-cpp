/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
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

#include "2d/CCActionInstant.h"
#include "2d/CCNode.h"
#include "2d/CCSprite.h"

namespace cocos2d {

// InstantAction

bool ActionInstant::isDone() const
{
    return true;
}

void ActionInstant::update(float /*dt*/)
{
    step(1.0f);
}

void ActionInstant::at_stop()
{
}

// Show

void Show::step(float /*time*/)
{
    getTarget()->setVisible(true);
}

ActionInstant* Show::reverse() const
{
    return new Hide;
}

Show* Show::clone() const
{
    return new Show;
}

// Hide

void Hide::step(float /*time*/)
{
    getTarget()->setVisible(false);
}

ActionInstant *Hide::reverse() const
{
    return new Show;
}

Hide* Hide::clone() const
{
    return new Hide;
}

// ToggleVisibility

void ToggleVisibility::step(float /*time*/)
{
    getTarget()->setVisible(!getTarget()->isVisible());
}

ToggleVisibility * ToggleVisibility::reverse() const
{
    return new ToggleVisibility;
}

ToggleVisibility * ToggleVisibility::clone() const
{
    return new ToggleVisibility;
}

// Remove Self

RemoveSelf::RemoveSelf(bool isNeedCleanUp)
    :_isNeedCleanUp( isNeedCleanUp )
{
}

void RemoveSelf::step(float /*time*/)
{
    getTarget()->removeFromParentAndCleanup(_isNeedCleanUp);
}

RemoveSelf* RemoveSelf::reverse() const
{
    return new RemoveSelf(_isNeedCleanUp);
}

RemoveSelf* RemoveSelf::clone() const
{
    return new RemoveSelf(_isNeedCleanUp);
}

// FlipX

FlipX::FlipX(bool x)
    : _flipX(x)
{
}

void FlipX::startWithTarget(Node *target)
{
    CC_ASSERT(dynamic_cast<Sprite*>(target));
}

void FlipX::step(float /*time*/)
{
    dynamic_cast<Sprite*>(getTarget())->setFlippedX(_flipX);
}

FlipX* FlipX::reverse() const
{
    return new FlipX(!_flipX);
}

FlipX * FlipX::clone() const
{
    return new FlipX(_flipX);
}

// FlipY

FlipY::FlipY(bool y)
    : _flipY(y)
{
}

void FlipY::startWithTarget(Node *target)
{
    CC_ASSERT(dynamic_cast<Sprite*>(target));
}

void FlipY::step(float /*time*/)
{
    static_cast<Sprite*>(getTarget())->setFlippedY(_flipY);
}

FlipY* FlipY::reverse() const
{
    return new FlipY(!_flipY);
}

FlipY * FlipY::clone() const
{
    return new FlipY(_flipY);
}

// Place

Place::Place(const Vec2& pos)
    : _position( pos )
{}

void Place::step(float /*time*/)
{
    getTarget()->setPosition(_position);
}

Place* Place::reverse() const
{
    return new Place(_position);
}

Place* Place::clone() const
{
    return new Place(_position);
}

// CallFunc

CallFunc::CallFunc(std::function<void()> func)
    : _function( func )
{
    CC_ASSERT(_function);
}

void CallFunc::step(float /*time*/)
{
    _function();
}

CallFunc * CallFunc::reverse() const
{
    return new CallFunc(_function);
}

CallFunc * CallFunc::clone() const
{
    return new CallFunc(_function);
}

// CallFuncN

CallFuncN::CallFuncN(std::function<void(Node*)> func)
    : _function(func)
{
    CC_ASSERT(_function);
}

void CallFuncN::step(float /*time*/)
{
    _function(getTarget());
}

CallFuncN * CallFuncN::reverse() const
{
    return new CallFuncN(_function);
}

CallFuncN * CallFuncN::clone() const
{
    return new CallFuncN(_function);
}

} // namespace cocos2d

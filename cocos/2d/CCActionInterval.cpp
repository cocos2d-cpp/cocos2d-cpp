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

#include "2d/CCActionInterval.h"

#include "2d/CCActionInstant.h"
#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventCustom.h"
#include "base/CCEventDispatcher.h"
#include "platform/CCStdC.h"

#include <cstdarg>

namespace cocos2d {

// Extra action for making a Sequence or Spawn when only adding one action to it.
namespace {

class ExtraAction : public FiniteTimeAction
{
public:
    virtual ExtraAction* clone() const override { return new ExtraAction; }
    virtual ExtraAction* reverse() const { return new ExtraAction; }
    virtual void step(float) {}
    virtual void update(float) {}
    virtual bool isDone() const { return true; }
    virtual void at_stop() {}
};

} // unnamed namespace

// ActionInterval

bool ActionInterval::initWithDuration(float d)
{
    _duration = d;

    // prevent division by 0
    // This comparison could be in update:, but it might decrease the performance
    // by 3% in heavy based action games.
    if (_duration <= FLT_EPSILON)
    {
        _duration = FLT_EPSILON;
    }

    _elapsed = 0;
    _firstTick = true;

    return true;
}

bool ActionInterval::isDone() const
{
    return _elapsed >= _duration;
}

void ActionInterval::update(float dt)
{
    if (_firstTick)
    {
        _firstTick = false;
        _elapsed = 0;
    }
    else
    {
        _elapsed += dt;
    }
    
    
    float updateDt = MAX (0,                                  // needed for rewind. elapsed could be negative
                           MIN(1, _elapsed / _duration)
                           );

    this->step(updateDt);
}

void ActionInterval::setAmplitudeRate(float /*amp*/)
{
    // Abstract class needs implementation
    CCASSERT(0, "Subclass should implement this method!");
}

void ActionInterval::startWithTarget(Node *target)
{
    FiniteTimeAction::startWithTarget(target);
    _elapsed = 0.0f;
    _firstTick = true;
}

// Speed

Speed::Speed(std::unique_ptr<ActionInterval> action, float speed)
    : _speed(speed)
    , _innerAction( std::move( action))
{
}

Speed* Speed::clone() const
{
    return new Speed(std::unique_ptr<ActionInterval>(_innerAction->clone()),
                     _speed);
}

void Speed::startWithTarget(Node* target)
{
    CC_ASSERT(target);

    Action::startWithTarget(target);
    _innerAction->startWithTarget(target);
}

void Speed::at_stop()
{
    _innerAction->stop();
}

void Speed::update(float dt)
{
    _innerAction->update(dt * _speed);
}

void Speed::step(float)
{
}

bool Speed::isDone() const
{
    return _innerAction->isDone();
}

Speed* Speed::reverse() const
{
    return new Speed(std::unique_ptr<ActionInterval>(_innerAction->reverse()),
                     _speed);
}

// Sequence

Sequence::Sequence(actions_container arrayOfActions)
{
    CC_ASSERT(! arrayOfActions.empty());

    auto count = arrayOfActions.size();

    if (count == 1)
    {
        initWithTwoActions(std::move(arrayOfActions.at(0)),
                           std::make_unique<ExtraAction>());
        return;
    }

    auto prev = std::move(arrayOfActions[0]);

    for (size_t i = 1; i < count-1; ++i)
    {
        prev = std::make_unique<Sequence>(
            std::move(prev),
            std::move(arrayOfActions[i])
        );
    }

    initWithTwoActions(std::move(prev), std::move(arrayOfActions.at(count-1)));
}

void Sequence::initWithTwoActions(std::unique_ptr<FiniteTimeAction> actionOne, std::unique_ptr<FiniteTimeAction> actionTwo)
{
    CC_ASSERT(actionOne);
    CC_ASSERT(actionTwo);

    float d = actionOne->getDuration() + actionTwo->getDuration();
    ActionInterval::initWithDuration(d);

    _actions[0] = std::move( actionOne );

    _actions[1] = std::move( actionTwo );
}

Sequence* Sequence::clone() const
{
    std::unique_ptr<FiniteTimeAction> clone0(_actions[0]->clone());
    std::unique_ptr<FiniteTimeAction> clone1(_actions[1]->clone());

    return new Sequence(std::move(clone0), std::move(clone1));
}

void Sequence::startWithTarget(Node *target)
{
    CC_ASSERT(target);

    if (_duration > FLT_EPSILON)
        _split = _actions[0]->getDuration() / _duration;
    
    ActionInterval::startWithTarget(target);
    _last = -1;
}

void Sequence::at_stop()
{
    // Issue #1305
    if( _last != - 1 && _actions[_last])
    {
        _actions[_last]->stop();
    }
}

void Sequence::step(float t)
{
    int found = 0;
    float new_t = 0.0f;

    if( t < _split )
    {
        // action[0]
        found = 0;
        if( _split != 0 )
            new_t = t / _split;
        else
            new_t = 1;
    }
    else
    {
        // action[1]
        found = 1;
        if ( _split == 1 )
            new_t = 1;
        else
            new_t = (t-_split) / (1 - _split );
    }

    if ( found==1 )
    {
        switch (_last)
        {
            case -1:
                // action[0] was skipped, execute it.
                _actions[0]->startWithTarget(getTarget());
                // no break
            case 0:
                // switching to action 1. stop action 0.
                _actions[0]->step(1.0f);
                _actions[0]->stop();
        }
    }
    else if(found==0 && _last==1 )
    {
        // Reverse mode ?
        // FIXME: Bug. this case doesn't contemplate when _last==-1, found=0 and in "reverse mode"
        // since it will require a hack to know if an action is on reverse mode or not.
        // "update" should be overridden, and the "reverseMode" value propagated to inner Sequences.
        _actions[1]->step(0);
        _actions[1]->stop();
    }

    // Last action found and it is done.
    if( found == _last && _actions[found]->isDone() )
    {
        return;
    }

    // Last action found and it is done
    if( found != _last )
    {
        _actions[found]->startWithTarget(getTarget());
    }

    _actions[found]->step(new_t);
    _last = found;
}

Sequence* Sequence::reverse() const
{
    std::unique_ptr<FiniteTimeAction> reverse1(_actions[1]->reverse());
    std::unique_ptr<FiniteTimeAction> reverse0(_actions[0]->reverse());

    return new Sequence(std::move(reverse1), std::move(reverse0));
}

// Repeat

Repeat::Repeat(std::unique_ptr<FiniteTimeAction> action, unsigned int times)
    : _innerAction( std::move( action))
    , _times(times)
    , _total(0)
    , _nextTime(0.0f)
    , _actionInstant( dynamic_cast<ActionInstant*>(action.get()) )
{
    ActionInterval::initWithDuration(_innerAction->getDuration() * _times);
}

Repeat* Repeat::clone() const
{
    return new Repeat(std::unique_ptr<FiniteTimeAction>(_innerAction->clone()),
                      _times);
}

void Repeat::startWithTarget(Node *target)
{
    _total = 0;
    _nextTime = _innerAction->getDuration() / _duration;
    ActionInterval::startWithTarget(target);
    _innerAction->startWithTarget(target);
}

void Repeat::at_stop()
{
    _innerAction->stop();
}

// issue #80. Instead of hooking update:, hook update: since it can be called by any 
// container action like Repeat, Sequence, Ease, etc..
void Repeat::step(float time)
{
    if (time >= _nextTime)
    {
        while (time >= _nextTime && _total < _times)
        {
            _innerAction->step(1.0f);
            _total++;

            _innerAction->stop();
            _innerAction->startWithTarget(getTarget());
            _nextTime = _innerAction->getDuration() / _duration * (_total+1);
        }

        // fix for issue #1288, incorrect end value of repeat
        if (std::abs(time - 1.0f) < FLT_EPSILON && _total < _times)
        {
            _innerAction->step(1.0f);
            
            _total++;
        }

        // don't set an instant action back or update it, it has no use because it has no duration
        if (!_actionInstant)
        {
            if (_total == _times)
            {
                _innerAction->stop();
            }
            else
            {
                // issue #390 prevent jerk, use right update
                _innerAction->step(time - (_nextTime - _innerAction->getDuration()/_duration));
            }
        }
    }
    else
    {
        _innerAction->step(fmodf(time * _times,1.0f));
    }
}

bool Repeat::isDone() const
{
    return _total == _times;
}

Repeat* Repeat::reverse() const
{
    return new Repeat(std::unique_ptr<FiniteTimeAction>(_innerAction->reverse()),
                      _times);
}

// RepeatForever

RepeatForever::RepeatForever(std::unique_ptr<ActionInterval> action)
    : _innerAction( std::move( action))
{}

RepeatForever *RepeatForever::clone() const
{
    return new RepeatForever( std::unique_ptr<ActionInterval>( _innerAction->clone()));
}

RepeatForever *RepeatForever::reverse() const
{
    return new RepeatForever( std::unique_ptr<ActionInterval>( _innerAction->reverse()));
}

void RepeatForever::startWithTarget(Node* target)
{
    ActionInterval::startWithTarget(target);
    _innerAction->startWithTarget(target);
}

void RepeatForever::update(float dt)
{
    _innerAction->update(dt);
    if (_innerAction->isDone())
    {
        float diff = _innerAction->getElapsed() - _innerAction->getDuration();
        if (diff > _innerAction->getDuration())
            diff = fmodf(diff, _innerAction->getDuration());
        _innerAction->startWithTarget(getTarget());
        // to prevent jerk. issue #390, 1247
        _innerAction->update(0.0f);
        _innerAction->update(diff);
    }
}

void RepeatForever::step(float)
{
}

bool RepeatForever::isDone() const
{
    return false;
}

void RepeatForever::at_stop()
{
}

// Spawn

Spawn::Spawn(actions_container arrayOfActions)
    : ActionInterval(0.0f)
{
    CC_ASSERT(arrayOfActions.size());

    auto count = arrayOfActions.size();
    
    if (count == 1)
    {
        initWithTwoActions(std::move(arrayOfActions.at(0)),
                           std::make_unique<ExtraAction>());
        return;
    }

    // else count > 1
    auto prev = std::move( arrayOfActions.at(0) );

    for (size_t i = 1; i < count-1; ++i)
    {
        prev = std::make_unique<Spawn>(std::move(prev), std::move(arrayOfActions.at(i)));
    }
    
    initWithTwoActions(std::move(prev), std::move(arrayOfActions.at(count-1)));
}

void Spawn::initWithTwoActions(std::unique_ptr<FiniteTimeAction> action1,
                               std::unique_ptr<FiniteTimeAction> action2)
{
    CC_ASSERT(action1);
    CC_ASSERT(action2);

    float d1 = action1->getDuration();
    float d2 = action2->getDuration();

    if (ActionInterval::initWithDuration(MAX(d1, d2)))
    {
        _one = std::move(action1);
        _two = std::move(action2);

        if (d1 > d2)
        {
            _two = std::make_unique<Sequence>(std::move(_two), std::make_unique<DelayTime>(d1 - d2));
        } 
        else if (d1 < d2)
        {
            _one = std::make_unique<Sequence>(std::move(_one), std::make_unique<DelayTime>(d2 - d1));
        }
    }
}

Spawn* Spawn::clone() const
{
    std::unique_ptr<FiniteTimeAction> one(_one->clone());
    std::unique_ptr<FiniteTimeAction> two(_two->clone());
    return new Spawn(std::move(one), std::move(two));
}

void Spawn::startWithTarget(Node *target)
{
    CC_ASSERT(target);
    
    ActionInterval::startWithTarget(target);
    _one->startWithTarget(target);
    _two->startWithTarget(target);
}

void Spawn::at_stop()
{
    _one->stop();
    _two->stop();
}

void Spawn::step(float time)
{
    _one->step(time);
    _two->step(time);
}

Spawn* Spawn::reverse() const
{
    std::unique_ptr<FiniteTimeAction> reverse1(_one->reverse());
    std::unique_ptr<FiniteTimeAction> reverse2(_two->reverse());

    return new Spawn(std::move(reverse1), std::move(reverse2));
}

// RotateTo

RotateTo::RotateTo(float duration, float dstAngleX, float dstAngleY)
    : ActionInterval(duration)
    , _is3D(false)
    , _dstAngle(dstAngleX, dstAngleY, 0.0f)
{
}

RotateTo::RotateTo(float duration, const Vec3& dstAngle3D)
    : ActionInterval(duration)
    , _is3D(false)
    , _dstAngle(dstAngle3D)
{
}

RotateTo* RotateTo::clone() const
{
    if(_is3D)
    {
       return new RotateTo(_duration, _dstAngle);
    }
    return new RotateTo(_duration, _dstAngle.x, _dstAngle.y);
}

void RotateTo::calculateAngles(float &startAngle, float &diffAngle, float dstAngle)
{
    if (startAngle > 0)
    {
        startAngle = fmodf(startAngle, 360.0f);
    }
    else
    {
        startAngle = fmodf(startAngle, -360.0f);
    }

    diffAngle = dstAngle - startAngle;
    if (diffAngle > 180)
    {
        diffAngle -= 360;
    }
    if (diffAngle < -180)
    {
        diffAngle += 360;
    }
}

void RotateTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    
    if (_is3D)
    {
        _startAngle = getTarget()->getRotation3D();
    }
    else
    {
        _startAngle.x = getTarget()->getRotationSkewX();
        _startAngle.y = getTarget()->getRotationSkewY();
    }

    calculateAngles(_startAngle.x, _diffAngle.x, _dstAngle.x);
    calculateAngles(_startAngle.y, _diffAngle.y, _dstAngle.y);
    calculateAngles(_startAngle.z, _diffAngle.z, _dstAngle.z);
}

void RotateTo::step(float time)
{
    if (getTarget())
    {
        if(_is3D)
        {
            getTarget()->setRotation3D(Vec3(
                _startAngle.x + _diffAngle.x * time,
                _startAngle.y + _diffAngle.y * time,
                _startAngle.z + _diffAngle.z * time
            ));
        }
        else
        {
#if CC_USE_PHYSICS
            if (_startAngle.x == _startAngle.y && _diffAngle.x == _diffAngle.y)
            {
                getTarget()->setRotation(_startAngle.x + _diffAngle.x * time);
            }
            else
            {
                getTarget()->setRotationSkewX(_startAngle.x + _diffAngle.x * time);
                getTarget()->setRotationSkewY(_startAngle.y + _diffAngle.y * time);
            }
#else
            getTarget()->setRotationSkewX(_startAngle.x + _diffAngle.x * time);
            getTarget()->setRotationSkewY(_startAngle.y + _diffAngle.y * time);
#endif // CC_USE_PHYSICS
        }
    }
}

RotateTo *RotateTo::reverse() const
{
    CCASSERT(false, "RotateTo doesn't support the 'reverse' method");
    return nullptr;
}

void RotateTo::at_stop()
{
}

// RotateBy

RotateBy::RotateBy(float duration, float deltaAngleX, float deltaAngleY)
    : ActionInterval(duration)
    , _is3D(false)
    , _deltaAngle(deltaAngleX, deltaAngleY, 0.0f)
{
}

RotateBy::RotateBy(float duration, const Vec3& deltaAngle3D)
    : ActionInterval(duration)
    , _is3D(true)
    , _deltaAngle(deltaAngle3D)
{
}

RotateBy* RotateBy::clone() const
{
    if (_is3D)
    {
        return new RotateBy(_duration, _deltaAngle);
    }
    return new RotateBy(_duration, _deltaAngle.x, _deltaAngle.y);
}

void RotateBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    if(_is3D)
    {
        _startAngle = target->getRotation3D();
    }
    else
    {
        _startAngle.x = target->getRotationSkewX();
        _startAngle.y = target->getRotationSkewY();
    }
}

void RotateBy::step(float time)
{
    CC_ASSERT(getTarget());

    if(_is3D)
    {
        Vec3 v;
        v.x = _startAngle.x + _deltaAngle.x * time;
        v.y = _startAngle.y + _deltaAngle.y * time;
        v.z = _startAngle.z + _deltaAngle.z * time;
        getTarget()->setRotation3D(v);
    }
    else
    {
#if CC_USE_PHYSICS
        if (_startAngle.x == _startAngle.y && _deltaAngle.x == _deltaAngle.y)
        {
            getTarget()->setRotation(_startAngle.x + _deltaAngle.x * time);
        }
        else
        {
            getTarget()->setRotationSkewX(_startAngle.x + _deltaAngle.x * time);
            getTarget()->setRotationSkewY(_startAngle.y + _deltaAngle.y * time);
        }
#else
        getTarget()->setRotationSkewX(_startAngle.x + _deltaAngle.x * time);
        getTarget()->setRotationSkewY(_startAngle.y + _deltaAngle.y * time);
#endif // CC_USE_PHYSICS
    }
}

RotateBy* RotateBy::reverse() const
{
    if(_is3D)
    {
        Vec3 v;
        v.x = - _deltaAngle.x;
        v.y = - _deltaAngle.y;
        v.z = - _deltaAngle.z;
        return new RotateBy(_duration, v);
    }

    return new RotateBy(_duration, -_deltaAngle.x, -_deltaAngle.y);
}

void RotateBy::at_stop()
{
}

// MoveBy

MoveBy::MoveBy(float duration, Vec3 deltaPosition)
    : ActionInterval(duration)
    , _positionDelta( std::move( deltaPosition))
{
}

MoveBy* MoveBy::clone() const
{
    return new MoveBy(_duration, _positionDelta);
}

void MoveBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _previousPosition = _startPosition = target->getPosition3D();
}

MoveBy* MoveBy::reverse() const
{
    return new MoveBy(_duration, -_positionDelta);
}

void MoveBy::step(float t)
{
    CC_ASSERT(getTarget());

#if CC_ENABLE_STACKABLE_ACTIONS
    Vec3 currentPos = getTarget()->getPosition3D();
    Vec3 diff = currentPos - _previousPosition;
    _startPosition = _startPosition + diff;
    Vec3 newPos =  _startPosition + (_positionDelta * t);
    getTarget()->setPosition3D(newPos);
    _previousPosition = newPos;
#else
    getTarget()->setPosition3D(_startPosition + _positionDelta * t);
#endif // CC_ENABLE_STACKABLE_ACTIONS
}

void MoveBy::at_stop()
{
}

// MoveTo

MoveTo::MoveTo(float duration, Vec3 position)
    : MoveBy(duration, position)
    , _endPosition( std::move( position))
{
}

MoveTo* MoveTo::clone() const
{
    return new MoveTo(_duration, _endPosition);
}

void MoveTo::startWithTarget(Node *target)
{
    MoveBy::startWithTarget(target);
    _positionDelta = _endPosition - target->getPosition3D();
}

MoveTo* MoveTo::reverse() const
{
    CCASSERT(false, "reverse() not supported in MoveTo");
    return nullptr;
}

// SkewTo

SkewTo::SkewTo(float duration, float sx, float sy)
    : ActionInterval(duration)
    , _skewX(0.0)
    , _skewY(0.0)
    , _startSkewX(0.0)
    , _startSkewY(0.0)
    , _endSkewX(sx)
    , _endSkewY(sy)
    , _deltaX(0.0)
    , _deltaY(0.0)
{
}

SkewTo* SkewTo::clone() const
{
    return new SkewTo(_duration, _endSkewX, _endSkewY);
}

SkewTo* SkewTo::reverse() const
{
    CCASSERT(false, "reverse() not supported in SkewTo");
    return nullptr;
}

void SkewTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);

    _startSkewX = target->getSkewX();

    if (_startSkewX > 0)
    {
        _startSkewX = fmodf(_startSkewX, 180.f);
    }
    else
    {
        _startSkewX = fmodf(_startSkewX, -180.f);
    }

    _deltaX = _endSkewX - _startSkewX;

    if (_deltaX > 180)
    {
        _deltaX -= 360;
    }
    if (_deltaX < -180)
    {
        _deltaX += 360;
    }

    _startSkewY = target->getSkewY();

    if (_startSkewY > 0)
    {
        _startSkewY = fmodf(_startSkewY, 360.f);
    }
    else
    {
        _startSkewY = fmodf(_startSkewY, -360.f);
    }

    _deltaY = _endSkewY - _startSkewY;

    if (_deltaY > 180)
    {
        _deltaY -= 360;
    }
    if (_deltaY < -180)
    {
        _deltaY += 360;
    }
}

void SkewTo::step(float t)
{
    getTarget()->setSkewX(_startSkewX + _deltaX * t);
    getTarget()->setSkewY(_startSkewY + _deltaY * t);
}

void SkewTo::at_stop()
{
}

// SkewBy

SkewBy::SkewBy(float t, float sx, float sy)
    : SkewTo(t, sx, sy)
{
    _skewX = sx;
    _skewY = sy;
}

SkewBy* SkewBy::clone() const
{
    return new SkewBy(_duration, _skewX, _skewY);
}

void SkewBy::startWithTarget(Node *target)
{
    SkewTo::startWithTarget(target);
    _deltaX = _skewX;
    _deltaY = _skewY;
    _endSkewX = _startSkewX + _deltaX;
    _endSkewY = _startSkewY + _deltaY;
}

SkewBy* SkewBy::reverse() const
{
    return new SkewBy(_duration, -_skewX, -_skewY);
}

// ResizeTo

ResizeTo::ResizeTo(float duration, const Size& final_size)
    : ActionInterval(duration)
    , _finalSize( final_size )
{
}

ResizeTo* ResizeTo::clone() const
{
    return new ResizeTo(_duration, _finalSize);
}

void ResizeTo::startWithTarget(cocos2d::Node* target)
{
    ActionInterval::startWithTarget(target);
    _initialSize = target->getContentSize();
    _sizeDelta = _finalSize - _initialSize;
}

void ResizeTo::step(float time)
{
    if (getTarget())
    {
        auto new_size = _initialSize + (_sizeDelta * time);
        getTarget()->setContentSize(new_size);
    }
}

void ResizeTo::at_stop()
{
}

//
// ResizeBy
//

std::unique_ptr<ResizeBy> ResizeBy::create(float duration, const cocos2d::Size& deltaSize)
{
    auto ret = std::unique_ptr<ResizeBy>(new ResizeBy);

    if (!ret->initWithDuration(duration, deltaSize))
    {
        return ret;
    } 

    return std::unique_ptr<ResizeBy>();
}

ResizeBy* ResizeBy::clone() const
{
    // no copy constructor
    auto a = new (std::nothrow) ResizeBy();
    a->initWithDuration(_duration, _sizeDelta);
    return a;
}

void ResizeBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _previousSize = _startSize = target->getContentSize();
}

ResizeBy* ResizeBy::reverse() const
{
    cocos2d::Size newSize(-_sizeDelta.width, -_sizeDelta.height);
    return ResizeBy::create(_duration, newSize).release();
}

void ResizeBy::step(float t)
{
    if (getTarget())
    {
        getTarget()->setContentSize(_startSize + (_sizeDelta * t));
    }
}

bool ResizeBy::initWithDuration(float duration, const cocos2d::Size& deltaSize)
{
    bool ret = false;
    
    if (ActionInterval::initWithDuration(duration))
    {
        _sizeDelta = deltaSize;
        ret = true;
    }
    
    return ret;
}

void ResizeBy::at_stop()
{
}

//
// JumpBy
//

std::unique_ptr<JumpBy> JumpBy::create(float duration, const Vec2& position, float height, int jumps)
{
    auto jumpBy = std::unique_ptr<JumpBy>(new JumpBy);

    if (! jumpBy->initWithDuration(duration, position, height, jumps))
    {
        return std::unique_ptr<JumpBy>();
    }
    
    return jumpBy;
}

bool JumpBy::initWithDuration(float duration, const Vec2& position, float height, int jumps)
{
    CCASSERT(jumps>=0, "Number of jumps must be >= 0");
    if (jumps < 0)
    {
        log("JumpBy::initWithDuration error: Number of jumps must be >= 0");
        return false;
    }
    
    if (ActionInterval::initWithDuration(duration) && jumps>=0)
    {
        _delta = position;
        _height = height;
        _jumps = jumps;

        return true;
    }

    return false;
}

JumpBy* JumpBy::clone() const
{
    return JumpBy::create(_duration, _delta, _height, _jumps).release();
}

void JumpBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _previousPos = _startPosition = target->getPosition();
}

void JumpBy::step(float t)
{
    // parabolic jump (since v0.8.2)
    if (getTarget())
    {
        float frac = fmodf( t * _jumps, 1.0f );
        float y = _height * 4 * frac * (1 - frac);
        y += _delta.y * t;

        float x = _delta.x * t;
#if CC_ENABLE_STACKABLE_ACTIONS
        Vec2 currentPos = getTarget()->getPosition();

        Vec2 diff = currentPos - _previousPos;
        _startPosition = diff + _startPosition;

        Vec2 newPos = _startPosition + Vec2(x,y);
        getTarget()->setPosition(newPos);

        _previousPos = newPos;
#else
        getTarget()->setPosition(_startPosition + Vec2(x,y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
    }
}

JumpBy* JumpBy::reverse() const
{
    return JumpBy::create(_duration,
                          Vec2(-_delta.x, -_delta.y),
                          _height,
                          _jumps).release();
}

void JumpBy::at_stop()
{
}

//
// JumpTo
//

std::unique_ptr<JumpTo> JumpTo::create(float duration, const Vec2& position, float height, int jumps)
{
    auto jumpTo = std::unique_ptr<JumpTo>(new JumpTo);

    if (! jumpTo->initWithDuration(duration, position, height, jumps))
    {
        return std::unique_ptr<JumpTo>();
    }
    return jumpTo;
}

bool JumpTo::initWithDuration(float duration, const Vec2& position, float height, int jumps)
{
    CCASSERT(jumps>=0, "Number of jumps must be >= 0");
    if (jumps < 0)
    {
        log("JumpTo::initWithDuration error:Number of jumps must be >= 0");
        return false;
    }

    if (ActionInterval::initWithDuration(duration) && jumps>=0)
    {
        _endPosition = position;
        _height = height;
        _jumps = jumps;

        return true;
    }

    return false;
}

JumpTo* JumpTo::clone() const
{
    return JumpTo::create(_duration, _endPosition, _height, _jumps).release();
}

JumpTo* JumpTo::reverse() const
{
    CCASSERT(false, "reverse() not supported in JumpTo");
    return nullptr;
}

void JumpTo::startWithTarget(Node *target)
{
    JumpBy::startWithTarget(target);
    _delta.set(_endPosition.x - _startPosition.x, _endPosition.y - _startPosition.y);
}

// Bezier cubic formula:
//    ((1 - t) + t)3 = 1 
// Expands to ...
//   (1 - t)3 + 3t(1-t)2 + 3t2(1 - t) + t3 = 1 
static inline float bezierat( float a, float b, float c, float d, float t )
{
    return (powf(1-t,3) * a + 
            3*t*(powf(1-t,2))*b + 
            3*powf(t,2)*(1-t)*c +
            powf(t,3)*d );
}

//
// BezierBy
//

std::unique_ptr<BezierBy> BezierBy::create(float t, const ccBezierConfig& c)
{
    auto bezierBy = std::unique_ptr<BezierBy>(new BezierBy);

    if (! bezierBy->initWithDuration(t, c))
    {
        return std::unique_ptr<BezierBy>();
    }
    
    return bezierBy;
}

bool BezierBy::initWithDuration(float t, const ccBezierConfig& c)
{
    if (ActionInterval::initWithDuration(t))
    {
        _config = c;
        return true;
    }

    return false;
}

void BezierBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _previousPosition = _startPosition = target->getPosition();
}

BezierBy* BezierBy::clone() const
{
    return BezierBy::create(_duration, _config).release();
}

void BezierBy::step(float time)
{
    if (getTarget())
    {
        float xa = 0;
        float xb = _config.controlPoint_1.x;
        float xc = _config.controlPoint_2.x;
        float xd = _config.endPosition.x;

        float ya = 0;
        float yb = _config.controlPoint_1.y;
        float yc = _config.controlPoint_2.y;
        float yd = _config.endPosition.y;

        float x = bezierat(xa, xb, xc, xd, time);
        float y = bezierat(ya, yb, yc, yd, time);

#if CC_ENABLE_STACKABLE_ACTIONS
        Vec2 currentPos = getTarget()->getPosition();
        Vec2 diff = currentPos - _previousPosition;
        _startPosition = _startPosition + diff;

        Vec2 newPos = _startPosition + Vec2(x,y);
        getTarget()->setPosition(newPos);

        _previousPosition = newPos;
#else
        getTarget()->setPosition( _startPosition + Vec2(x,y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
    }
}

BezierBy* BezierBy::reverse() const
{
    ccBezierConfig r;

    r.endPosition = -_config.endPosition;
    r.controlPoint_1 = _config.controlPoint_2 + (-_config.endPosition);
    r.controlPoint_2 = _config.controlPoint_1 + (-_config.endPosition);

    return BezierBy::create(_duration, r).release();
}

void BezierBy::at_stop()
{
}

//
// BezierTo
//

std::unique_ptr<BezierTo> BezierTo::create(float t, const ccBezierConfig& c)
{
    auto bezierTo = std::unique_ptr<BezierTo>(new BezierTo);

    if (! bezierTo->initWithDuration(t, c))
    {
        return std::unique_ptr<BezierTo>();
    }
    
    return bezierTo;
}

bool BezierTo::initWithDuration(float t, const ccBezierConfig &c)
{
    if (ActionInterval::initWithDuration(t))
    {
        _toConfig = c;
        return true;
    }
    
    return false;
}

BezierTo* BezierTo::clone() const
{
    // no copy constructor
    return BezierTo::create(_duration, _toConfig).release();
}

void BezierTo::startWithTarget(Node *target)
{
    BezierBy::startWithTarget(target);
    _config.controlPoint_1 = _toConfig.controlPoint_1 - _startPosition;
    _config.controlPoint_2 = _toConfig.controlPoint_2 - _startPosition;
    _config.endPosition = _toConfig.endPosition - _startPosition;
}

BezierTo* BezierTo::reverse() const
{
    CCASSERT(false, "CCBezierTo doesn't support the 'reverse' method");
    return nullptr;
}

//
// ScaleTo
//
std::unique_ptr<ScaleTo> ScaleTo::create(float duration, float s)
{
    auto scaleTo = std::unique_ptr<ScaleTo>(new ScaleTo);

    if (! scaleTo->initWithDuration(duration, s))
    {
        return std::unique_ptr<ScaleTo>();
    }

    return scaleTo;
}

std::unique_ptr<ScaleTo> ScaleTo::create(float duration, float sx, float sy)
{
    auto scaleTo = std::unique_ptr<ScaleTo>(new ScaleTo);

    if (! scaleTo->initWithDuration(duration, sx, sy))
    {
        return std::unique_ptr<ScaleTo>();
    }

    return scaleTo;
}

std::unique_ptr<ScaleTo> ScaleTo::create(float duration, float sx, float sy, float sz)
{
    auto scaleTo = std::unique_ptr<ScaleTo>(new ScaleTo);

    if (! scaleTo->initWithDuration(duration, sx, sy, sz))
    {
        return std::unique_ptr<ScaleTo>();
    }

    return scaleTo;
}

bool ScaleTo::initWithDuration(float duration, float s)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _endScaleX = s;
        _endScaleY = s;
        _endScaleZ = s;

        return true;
    }

    return false;
}

bool ScaleTo::initWithDuration(float duration, float sx, float sy)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _endScaleX = sx;
        _endScaleY = sy;
        _endScaleZ = 1.f;

        return true;
    }

    return false;
}

bool ScaleTo::initWithDuration(float duration, float sx, float sy, float sz)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _endScaleX = sx;
        _endScaleY = sy;
        _endScaleZ = sz;

        return true;
    }

    return false;
}

ScaleTo* ScaleTo::clone() const
{
    // no copy constructor
    return ScaleTo::create(_duration, _endScaleX, _endScaleY, _endScaleZ).release();
}

ScaleTo* ScaleTo::reverse() const
{
    CCASSERT(false, "reverse() not supported in ScaleTo");
    return nullptr;
}

void ScaleTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _startScaleX = target->getScaleX();
    _startScaleY = target->getScaleY();
    _startScaleZ = target->getScaleZ();
    _deltaX = _endScaleX - _startScaleX;
    _deltaY = _endScaleY - _startScaleY;
    _deltaZ = _endScaleZ - _startScaleZ;
}

void ScaleTo::step(float time)
{
    if (getTarget())
    {
        getTarget()->setScaleX(_startScaleX + _deltaX * time);
        getTarget()->setScaleY(_startScaleY + _deltaY * time);
        getTarget()->setScaleZ(_startScaleZ + _deltaZ * time);
    }
}

void ScaleTo::at_stop()
{
}

//
// ScaleBy
//

std::unique_ptr<ScaleBy> ScaleBy::create(float duration, float s)
{
    auto scaleBy = std::unique_ptr<ScaleBy>(new ScaleBy);

    if (! scaleBy->initWithDuration(duration, s))
    {
        return std::unique_ptr<ScaleBy>();
    }
    
    return scaleBy;
}

std::unique_ptr<ScaleBy> ScaleBy::create(float duration, float sx, float sy, float sz)
{
    auto scaleBy = std::unique_ptr<ScaleBy>(new ScaleBy);

    if (! scaleBy->initWithDuration(duration, sx, sy, sz))
    {
        return std::unique_ptr<ScaleBy>();
    }
    
    return scaleBy;
}

ScaleBy* ScaleBy::clone() const
{
    // no copy constructor
    return ScaleBy::create(_duration, _endScaleX, _endScaleY, _endScaleZ).release();
}

void ScaleBy::startWithTarget(Node *target)
{
    ScaleTo::startWithTarget(target);
    _deltaX = _startScaleX * _endScaleX - _startScaleX;
    _deltaY = _startScaleY * _endScaleY - _startScaleY;
    _deltaZ = _startScaleZ * _endScaleZ - _startScaleZ;
}

ScaleBy* ScaleBy::reverse() const
{
    return ScaleBy::create(_duration, 1 / _endScaleX, 1 / _endScaleY, 1/ _endScaleZ).release();
}

//
// Blink
//

std::unique_ptr<Blink> Blink::create(float duration, int blinks)
{
    auto blink = std::unique_ptr<Blink>(new Blink);

    if (! blink->initWithDuration(duration, blinks))
    {
        return std::unique_ptr<Blink>();
    }

    return blink;
}

bool Blink::initWithDuration(float duration, int blinks)
{
    CCASSERT(blinks>=0, "blinks should be >= 0");
    if (blinks < 0)
    {
        log("Blink::initWithDuration error:blinks should be >= 0");
        return false;
    }
    
    if (ActionInterval::initWithDuration(duration) && blinks>=0)
    {
        _times = blinks;
        return true;
    }

    return false;
}

void Blink::at_stop()
{
    if (nullptr != getTarget())
        getTarget()->setVisible(_originalState);
}

void Blink::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _originalState = target->isVisible();
}

Blink* Blink::clone() const
{
    // no copy constructor
    return Blink::create(_duration, _times).release();
}

void Blink::step(float time)
{
    if (getTarget() && ! isDone())
    {
        float slice = 1.0f / _times;
        float m = fmodf(time, slice);
        getTarget()->setVisible(m > slice / 2 ? true : false);
    }
}

Blink* Blink::reverse() const
{
    return Blink::create(_duration, _times).release();
}

//
// FadeIn
//

std::unique_ptr<FadeIn> FadeIn::create(float d)
{
    auto action = std::unique_ptr<FadeIn>(new FadeIn);

    if (! action->initWithDuration(d, 255.0f))
    {
        return std::unique_ptr<FadeIn>();
    }

    return action;
}

FadeIn* FadeIn::clone() const
{
    // no copy constructor
    return FadeIn::create(_duration).release();
}

void FadeIn::setReverseAction(std::unique_ptr<FadeOut> ac)
{
    _reverseAction = std::move( ac );
}


FadeTo* FadeIn::reverse() const
{
    auto action = FadeOut::create(_duration);
    action->setReverseAction(std::unique_ptr<FadeIn>(clone()));
    return action.release();
}

void FadeIn::startWithTarget(cocos2d::Node *target)
{
    ActionInterval::startWithTarget(target);
    
    if (nullptr != _reverseAction)
        this->_toOpacity = this->_reverseAction->_fromOpacity;
    else
        _toOpacity = 255.0f;
    
    if (target)
        _fromOpacity = target->getOpacity();
}

//
// FadeOut
//

std::unique_ptr<FadeOut> FadeOut::create(float d)
{
    auto action = std::unique_ptr<FadeOut>(new FadeOut);

    if (! action->initWithDuration(d,0.0f))
    {
        return std::unique_ptr<FadeOut>();
    }
    
    return action;
}

FadeOut* FadeOut::clone() const
{
    // no copy constructor
    return FadeOut::create(_duration).release();
}

void FadeOut::startWithTarget(cocos2d::Node *target)
{
    ActionInterval::startWithTarget(target);
    
    if (nullptr != _reverseAction)
        _toOpacity = _reverseAction->_fromOpacity;
    else
        _toOpacity = 0.0f;
    
    if (target)
        _fromOpacity = target->getOpacity();
}

void FadeOut::setReverseAction(std::unique_ptr<FadeIn> ac)
{
    _reverseAction = std::move(ac);
}


FadeTo* FadeOut::reverse() const
{
    auto action = FadeIn::create(_duration);
    action->setReverseAction( std::unique_ptr<FadeOut>( clone()));
    return action.release();
}

//
// FadeTo
//

std::unique_ptr<FadeTo> FadeTo::create(float duration, GLubyte opacity)
{
    auto fadeTo = std::unique_ptr<FadeTo>(new FadeTo);

    if (! fadeTo->initWithDuration(duration, opacity))
    {
        return std::unique_ptr<FadeTo>();
    }
    
    return fadeTo;
}

bool FadeTo::initWithDuration(float duration, GLubyte opacity)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _toOpacity = opacity;
        return true;
    }

    return false;
}

FadeTo* FadeTo::clone() const
{
    // no copy constructor
    return FadeTo::create(_duration, _toOpacity).release();
}

FadeTo* FadeTo::reverse() const
{
    CCASSERT(false, "reverse() not supported in FadeTo");
    return nullptr;
}

void FadeTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);

    if (target)
    {
        _fromOpacity = target->getOpacity();
    }
}

void FadeTo::step(float time)
{
    if (getTarget())
    {
        getTarget()->setOpacity((GLubyte)(_fromOpacity + (_toOpacity - _fromOpacity) * time));
    }
}

void FadeTo::at_stop()
{
}

//
// TintTo
//
std::unique_ptr<TintTo> TintTo::create(float duration, GLubyte red, GLubyte green, GLubyte blue)
{
    auto tintTo = std::unique_ptr<TintTo>(new TintTo);

    if (! tintTo->initWithDuration(duration, red, green, blue))
    {
        return std::unique_ptr<TintTo>();
    }
    
    return tintTo;
}

std::unique_ptr<TintTo> TintTo::create(float duration, const Color3B& color)
{
    return create(duration, color.r, color.g, color.b);
}

bool TintTo::initWithDuration(float duration, GLubyte red, GLubyte green, GLubyte blue)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _to = Color3B(red, green, blue);
        return true;
    }

    return false;
}

TintTo* TintTo::clone() const
{
    // no copy constructor
    return TintTo::create(_duration, _to.r, _to.g, _to.b).release();
}

TintTo* TintTo::reverse() const
{
    CCASSERT(false, "reverse() not supported in TintTo");
    return nullptr;
}

void TintTo::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    if (getTarget())
    {
        _from = getTarget()->getColor();
    }
}

void TintTo::step(float time)
{
    if (getTarget())
    {
        getTarget()->setColor(Color3B(GLubyte(_from.r + (_to.r - _from.r) * time),
            (GLubyte)(_from.g + (_to.g - _from.g) * time),
            (GLubyte)(_from.b + (_to.b - _from.b) * time)));
    }
}

void TintTo::at_stop()
{
}

//
// TintBy
//

std::unique_ptr<TintBy> TintBy::create(float duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue)
{
    auto tintBy = std::unique_ptr<TintBy>(new TintBy);

    if (! tintBy->initWithDuration(duration, deltaRed, deltaGreen, deltaBlue))
    {
        return std::unique_ptr<TintBy>();
    }
    
    return tintBy;
}

bool TintBy::initWithDuration(float duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _deltaR = deltaRed;
        _deltaG = deltaGreen;
        _deltaB = deltaBlue;

        return true;
    }

    return false;
}

TintBy* TintBy::clone() const
{
    // no copy constructor
    return TintBy::create(_duration, _deltaR, _deltaG, _deltaB).release();
}

void TintBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);

    if (target)
    {
        Color3B color = target->getColor();
        _fromR = color.r;
        _fromG = color.g;
        _fromB = color.b;
    }    
}

void TintBy::step(float time)
{
    if (getTarget())
    {
        getTarget()->setColor(Color3B((GLubyte)(_fromR + _deltaR * time),
            (GLubyte)(_fromG + _deltaG * time),
            (GLubyte)(_fromB + _deltaB * time)));
    }    
}

TintBy* TintBy::reverse() const
{
    return TintBy::create(_duration, -_deltaR, -_deltaG, -_deltaB).release();
}

void TintBy::at_stop()
{
}

//
// DelayTime
//
DelayTime::DelayTime(float d)
{
    ActionInterval::initWithDuration(d);
}

DelayTime* DelayTime::clone() const
{
    return new DelayTime(_duration);
}

void DelayTime::step(float /*time*/)
{
    return;
}

DelayTime* DelayTime::reverse() const
{
    return new DelayTime(_duration);
}

void DelayTime::at_stop()
{
}

//
// ReverseTime
//

std::unique_ptr<ReverseTime> ReverseTime::create(std::unique_ptr<FiniteTimeAction> action)
{
    auto reverseTime = std::unique_ptr<ReverseTime>(new ReverseTime);

    if (! reverseTime->initWithAction( std::move(action) ))
    {
        return std::unique_ptr<ReverseTime>();
    }

    return reverseTime;
}

bool ReverseTime::initWithAction(std::unique_ptr<FiniteTimeAction> action)
{
    CC_ASSERT(action);
    CC_ASSERT(action);

    if (action == nullptr || action == _other)
    {
        log("ReverseTime::initWithAction error: action is null or action equal to _other");
        return false;
    }

    if (ActionInterval::initWithDuration(action->getDuration()))
    {
        _other = std::move( action );
        return true;
    }

    return false;
}

ReverseTime* ReverseTime::clone() const
{
    // no copy constructor
    return ReverseTime::create(std::unique_ptr<FiniteTimeAction>(_other->clone())).release();
}

void ReverseTime::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _other->startWithTarget(target);
}

void ReverseTime::at_stop()
{
    _other->stop();
}

void ReverseTime::step(float time)
{
    _other->step(1 - time);
}

ReverseTime* ReverseTime::reverse() const
{
    CC_ASSERT(false);
}

//
// Animate
//
std::unique_ptr<Animate> Animate::create(std::unique_ptr<Animation> animation)
{
    auto animate = std::unique_ptr<Animate>(new Animate);

    if (! animate->initWithAnimation( std::move( animation)))
    {
        return std::unique_ptr<Animate>();
    }
    
    return animate;
}

Animate::Animate()
: _splitTimes(new std::vector<float>)
, _nextFrame(0)
, _origFrame(nullptr)
, _currFrameIndex(0)
, _executedLoops(0)
, _animation()
, _frameDisplayedEvent(nullptr)
{

}

Animate::~Animate()
{
    CC_SAFE_RELEASE(_origFrame);
    CC_SAFE_DELETE(_splitTimes);
    CC_SAFE_RELEASE(_frameDisplayedEvent);
}

bool Animate::initWithAnimation(std::unique_ptr<Animation> animation)
{
    CCASSERT( animation, "Animate: argument Animation must be non-nullptr");
    if ( ! animation)
    {
        log("Animate::initWithAnimation: argument Animation must be non-nullptr");
        return false;
    }

    float singleDuration = animation->getDuration();

    if ( ActionInterval::initWithDuration(singleDuration * animation->getLoops() ) )
    {
        _nextFrame = 0;
        _origFrame = nullptr;
        _executedLoops = 0;

        _splitTimes->reserve(animation->getFrames().size());

        float accumUnitsOfTime = 0;
        float newUnitOfTimeValue = singleDuration / animation->getTotalDelayUnits();

        auto& frames = animation->getFrames();

        for (auto& frame : frames)
        {
            float value = (accumUnitsOfTime * newUnitOfTimeValue) / singleDuration;
            accumUnitsOfTime += frame->getDelayUnits();
            _splitTimes->push_back(value);
        }    

        setAnimation( std::move( animation));

        return true;
    }
    return false;
}

void Animate::setAnimation(std::unique_ptr<Animation> animation)
{
    _animation = std::move(animation);
}

Animate* Animate::clone() const
{
    // no copy constructor
    return Animate::create( std::unique_ptr<Animation>( _animation->clone())).release();
}

void Animate::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    Sprite *sprite = static_cast<Sprite*>(target);

    CC_SAFE_RELEASE(_origFrame);

    if (_animation->getRestoreOriginalFrame())
    {
        _origFrame = sprite->getSpriteFrame();
        _origFrame->retain();
    }
    _nextFrame = 0;
    _executedLoops = 0;
}

void Animate::at_stop()
{
    if (_animation->getRestoreOriginalFrame() && getTarget())
    {
        auto blend = static_cast<Sprite*>(getTarget())->getBlendFunc();
        static_cast<Sprite*>(getTarget())->setSpriteFrame(_origFrame);
        static_cast<Sprite*>(getTarget())->setBlendFunc(blend);
    }
}

void Animate::step(float t)
{
    // if t==1, ignore. Animation should finish with t==1
    if( t < 1.0f )
    {
        t *= _animation->getLoops();

        // new loop?  If so, reset frame counter
        unsigned int loopNumber = (unsigned int)t;
        if( loopNumber > _executedLoops )
        {
            _nextFrame = 0;
            _executedLoops++;
        }

        // new t for animations
        t = fmodf(t, 1.0f);
    }

    auto& frames = _animation->getFrames();
    auto numberOfFrames = frames.size();
    SpriteFrame *frameToDisplay = nullptr;

    for (size_t i = _nextFrame; i < numberOfFrames; i++)
    {
        float splitTime = _splitTimes->at(i);

        if( splitTime <= t )
        {
            auto blend = static_cast<Sprite*>(getTarget())->getBlendFunc();
            _currFrameIndex = i;
            AnimationFrame* frame = frames.at(_currFrameIndex).get();
            frameToDisplay = frame->getSpriteFrame();
            static_cast<Sprite*>(getTarget())->setSpriteFrame(frameToDisplay);
            static_cast<Sprite*>(getTarget())->setBlendFunc(blend);

            const ValueMap& dict = frame->getUserInfo();
            if ( !dict.empty() )
            {
                if (_frameDisplayedEvent == nullptr)
                    _frameDisplayedEvent = new (std::nothrow) EventCustom(AnimationFrameDisplayedNotification);
                
                _frameDisplayedEventInfo.target = getTarget();
                _frameDisplayedEventInfo.userInfo = &dict;
                _frameDisplayedEvent->setUserData(&_frameDisplayedEventInfo);
                Director::getInstance()->getEventDispatcher()->dispatchEvent(_frameDisplayedEvent);
            }
            _nextFrame = i+1;
        }
        // Issue 1438. Could be more than one frame per tick, due to low frame rate or frame delta < 1/FPS
        else {
            break;
        }
    }
}

Animate* Animate::reverse() const
{
    auto & oldArray = _animation->getFrames();
    std::vector<retaining_ptr<AnimationFrame>> newArray;

    newArray.reserve(oldArray.size());

    for (auto it = oldArray.crbegin(), end = oldArray.crend(); it != end; ++it)
    {
        if (!*it)
        {
            break;
        }

        newArray.push_back( to_retaining_ptr((*it)->clone()) );
    }

    std::unique_ptr<Animation> newAnim(
        new Animation(
            std::move(newArray),
            _animation->getDelayPerUnit(),
            _animation->getLoops()
        ));

    newAnim->setRestoreOriginalFrame(_animation->getRestoreOriginalFrame());
    return Animate::create( std::move( newAnim)).release();
}

// TargetedAction

TargetedAction::TargetedAction()
: _action(nullptr)
, _forcedTarget(nullptr)
{

}

TargetedAction::~TargetedAction()
{
    CC_SAFE_RELEASE(_forcedTarget);
}

std::unique_ptr<TargetedAction> TargetedAction::create(Node* target, std::unique_ptr<FiniteTimeAction> action)
{
    auto p = std::unique_ptr<TargetedAction>(new TargetedAction);

    if (! p->initWithTarget(target, std::move(action)))
    {
        return std::unique_ptr<TargetedAction>();
    }
    
    return p;
}


bool TargetedAction::initWithTarget(Node* target, std::unique_ptr<FiniteTimeAction> action)
{
    if(ActionInterval::initWithDuration(action->getDuration()))
    {
        CC_SAFE_RETAIN(target);
        _forcedTarget = target;
        _action = std::move( action );
        return true;
    }
    return false;
}

TargetedAction* TargetedAction::clone() const
{
    return TargetedAction::create(_forcedTarget,
                                  std::unique_ptr<FiniteTimeAction>(_action->clone()))
        .release();
}

TargetedAction* TargetedAction::reverse() const
{
    auto a = new TargetedAction;
    a->initWithTarget(_forcedTarget, std::unique_ptr<FiniteTimeAction>(_action->reverse()));
    return a;
}

void TargetedAction::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _action->startWithTarget(_forcedTarget);
}

void TargetedAction::at_stop()
{
    _action->stop();
}

void TargetedAction::step(float time)
{
    _action->step(time);
}

void TargetedAction::setForcedTarget(Node* forcedTarget)
{
    if( _forcedTarget != forcedTarget )
    {
        CC_SAFE_RETAIN(forcedTarget);
        CC_SAFE_RELEASE(_forcedTarget);
        _forcedTarget = forcedTarget;
    }
}

// ActionFloat

std::unique_ptr<ActionFloat> ActionFloat::create(float duration, float from, float to, ActionFloatCallback callback)
{
    auto ref = std::unique_ptr<ActionFloat>(new ActionFloat);

    if (! ref->initWithDuration(duration, from, to, callback))
    {
        return std::unique_ptr<ActionFloat>();
    }

    return ref;
}

bool ActionFloat::initWithDuration(float duration, float from, float to, ActionFloatCallback callback)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _from = from;
        _to = to;
        _callback = callback;
        return true;
    }
    return false;
}

ActionFloat* ActionFloat::clone() const
{
    return ActionFloat::create(_duration, _from, _to, _callback).release();
}

void ActionFloat::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _delta = _to - _from;
}

void ActionFloat::step(float delta)
{
    float value = _to - _delta * (1 - delta);

    if (_callback)
    {
        // report back value to caller
        _callback(value);
    }
}

ActionFloat* ActionFloat::reverse() const
{
    return ActionFloat::create(_duration, _to, _from, _callback).release();
}

void ActionFloat::at_stop()
{
}

} // namespace cocos2d

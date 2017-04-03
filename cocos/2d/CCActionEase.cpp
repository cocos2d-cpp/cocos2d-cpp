/****************************************************************************
Copyright (c) 2008-2009 Jason Booth
Copyright (c) 2010-2012 cocos2d-x.org
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

/*
 * Elastic, Back and Bounce actions based on code from:
 * http://github.com/NikhilK/silverlightfx/
 *
 * by http://github.com/NikhilK
 */

#include "2d/CCActionEase.h"
#include "2d/CCTweenFunction.h"

namespace cocos2d {

#ifndef M_PI_X_2
#define M_PI_X_2 (float)M_PI * 2.0f
#endif

//
// EaseAction
//
ActionEase::ActionEase(std::unique_ptr<ActionInterval> action)
    : _inner( std::move( action))
{
    ActionInterval::initWithDuration(_inner->getDuration());
}

void ActionEase::startWithTarget(Node *target)
{
    CC_ASSERT(target);
    ActionInterval::startWithTarget(target);
    _inner->startWithTarget(getTarget());
}

void ActionEase::at_stop()
{
    _inner->stop();
}

void ActionEase::step(float time)
{
    _inner->step(time);
}

ActionInterval* ActionEase::getInnerAction()
{
    return _inner.get();
}

//
// EaseRateAction
//

//
// NOTE: Converting these macros into Templates is desirable, but please see
// issue #16159 [https://github.com/cocos2d/cocos2d-x/pull/16159] for further info
//
#define EASE_TEMPLATE_IMPL(CLASSNAME, TWEEN_FUNC, REVERSE_CLASSNAME) \
    \
void CLASSNAME::step(float time) \
{ \
    _inner->step(TWEEN_FUNC(time)); \
} \
CLASSNAME* CLASSNAME::clone() const \
{ \
    return new CLASSNAME(std::unique_ptr<ActionInterval>(_inner->clone())); \
} \
ActionEase* CLASSNAME::reverse() const \
{ \
    return new REVERSE_CLASSNAME(std::unique_ptr<ActionInterval>(_inner->reverse())); \
}

EASE_TEMPLATE_IMPL(EaseExponentialIn, tweenfunc::expoEaseIn, EaseExponentialOut);
EASE_TEMPLATE_IMPL(EaseExponentialOut, tweenfunc::expoEaseOut, EaseExponentialIn);
EASE_TEMPLATE_IMPL(EaseExponentialInOut, tweenfunc::expoEaseInOut, EaseExponentialInOut);
EASE_TEMPLATE_IMPL(EaseSineIn, tweenfunc::sineEaseIn, EaseSineOut);
EASE_TEMPLATE_IMPL(EaseSineOut, tweenfunc::sineEaseOut, EaseSineIn);
EASE_TEMPLATE_IMPL(EaseSineInOut, tweenfunc::sineEaseInOut, EaseSineInOut);
EASE_TEMPLATE_IMPL(EaseBounceIn, tweenfunc::bounceEaseIn, EaseBounceOut);
EASE_TEMPLATE_IMPL(EaseBounceOut, tweenfunc::bounceEaseOut, EaseBounceIn);
EASE_TEMPLATE_IMPL(EaseBounceInOut, tweenfunc::bounceEaseInOut, EaseBounceInOut);
EASE_TEMPLATE_IMPL(EaseBackIn, tweenfunc::backEaseIn, EaseBackOut);
EASE_TEMPLATE_IMPL(EaseBackOut, tweenfunc::backEaseOut, EaseBackIn);
EASE_TEMPLATE_IMPL(EaseBackInOut, tweenfunc::backEaseInOut, EaseBackInOut);
EASE_TEMPLATE_IMPL(EaseQuadraticActionIn, tweenfunc::quadraticIn, EaseQuadraticActionIn);
EASE_TEMPLATE_IMPL(EaseQuadraticActionOut, tweenfunc::quadraticOut, EaseQuadraticActionOut);
EASE_TEMPLATE_IMPL(EaseQuadraticActionInOut, tweenfunc::quadraticInOut, EaseQuadraticActionInOut);
EASE_TEMPLATE_IMPL(EaseQuarticActionIn, tweenfunc::quartEaseIn, EaseQuarticActionIn);
EASE_TEMPLATE_IMPL(EaseQuarticActionOut, tweenfunc::quartEaseOut, EaseQuarticActionOut);
EASE_TEMPLATE_IMPL(EaseQuarticActionInOut, tweenfunc::quartEaseInOut, EaseQuarticActionInOut);
EASE_TEMPLATE_IMPL(EaseQuinticActionIn, tweenfunc::quintEaseIn, EaseQuinticActionIn);
EASE_TEMPLATE_IMPL(EaseQuinticActionOut, tweenfunc::quintEaseOut, EaseQuinticActionOut);
EASE_TEMPLATE_IMPL(EaseQuinticActionInOut, tweenfunc::quintEaseInOut, EaseQuinticActionInOut);
EASE_TEMPLATE_IMPL(EaseCircleActionIn, tweenfunc::circEaseIn, EaseCircleActionIn);
EASE_TEMPLATE_IMPL(EaseCircleActionOut, tweenfunc::circEaseOut, EaseCircleActionOut);
EASE_TEMPLATE_IMPL(EaseCircleActionInOut, tweenfunc::circEaseInOut, EaseCircleActionInOut);
EASE_TEMPLATE_IMPL(EaseCubicActionIn, tweenfunc::cubicEaseIn, EaseCubicActionIn);
EASE_TEMPLATE_IMPL(EaseCubicActionOut, tweenfunc::cubicEaseOut, EaseCubicActionOut);
EASE_TEMPLATE_IMPL(EaseCubicActionInOut, tweenfunc::cubicEaseInOut, EaseCubicActionInOut);

//
// NOTE: Converting these macros into Templates is desirable, but please see
// issue #16159 [https://github.com/cocos2d/cocos2d-x/pull/16159] for further info
//
#define EASERATE_TEMPLATE_IMPL(CLASSNAME, TWEEN_FUNC) \
void CLASSNAME::step(float time) \
{ \
    _inner->step(TWEEN_FUNC(time, _rate)); \
} \
CLASSNAME* CLASSNAME::clone() const \
{ \
    return new CLASSNAME(std::unique_ptr<ActionInterval>(_inner->clone()), _rate); \
} \
EaseRateAction* CLASSNAME::reverse() const \
{ \
    return new CLASSNAME(std::unique_ptr<ActionInterval>(_inner->reverse()), 1.0f / _rate); \
}

EASERATE_TEMPLATE_IMPL(EaseIn, tweenfunc::easeIn);
EASERATE_TEMPLATE_IMPL(EaseOut, tweenfunc::easeOut);
EASERATE_TEMPLATE_IMPL(EaseInOut, tweenfunc::easeInOut);

//
// NOTE: Converting these macros into Templates is desirable, but please see
// issue #16159 [https://github.com/cocos2d/cocos2d-x/pull/16159] for further info
//
#define EASEELASTIC_TEMPLATE_IMPL(CLASSNAME, TWEEN_FUNC, REVERSE_CLASSNAME) \
\
void CLASSNAME::step(float time) \
{ \
    _inner->step(TWEEN_FUNC(time, _period)); \
} \
CLASSNAME* CLASSNAME::clone() const \
{ \
    return new CLASSNAME(std::unique_ptr<ActionInterval>(_inner->clone()), _period); \
} \
EaseElastic* CLASSNAME::reverse() const \
{ \
    return new REVERSE_CLASSNAME(std::unique_ptr<ActionInterval>(_inner->reverse()), _period); \
}

EASEELASTIC_TEMPLATE_IMPL(EaseElasticIn, tweenfunc::elasticEaseIn, EaseElasticOut);
EASEELASTIC_TEMPLATE_IMPL(EaseElasticOut, tweenfunc::elasticEaseOut, EaseElasticIn);
EASEELASTIC_TEMPLATE_IMPL(EaseElasticInOut, tweenfunc::elasticEaseInOut, EaseElasticInOut);

//
// EaseBezierAction
//

EaseBezierAction::EaseBezierAction(std::unique_ptr<ActionInterval> action)
    : ActionEase( std::move( action))
{
}

void EaseBezierAction::setBezierParamer(float p0, float p1, float p2, float p3)
{
    _p0 = p0;
    _p1 = p1;
    _p2 = p2;
    _p3 = p3;
}

EaseBezierAction* EaseBezierAction::clone() const
{
    auto ret = new EaseBezierAction( std::unique_ptr<ActionInterval>( _inner->clone()));
    ret->setBezierParamer(_p0,_p1,_p2,_p3);
    return ret;
}

void EaseBezierAction::step(float time)
{
    _inner->step(tweenfunc::bezieratFunction(_p0,_p1,_p2,_p3,time));
}

EaseBezierAction* EaseBezierAction::reverse() const
{
    auto reverseAction = new EaseBezierAction( std::unique_ptr<ActionInterval>( _inner->reverse()));
    reverseAction->setBezierParamer(_p3,_p2,_p1,_p0);
    return reverseAction;
}

} // namespace cocos2d

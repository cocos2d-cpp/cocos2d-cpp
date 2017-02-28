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

#ifndef __ACTION_CCEASE_ACTION_H__
#define __ACTION_CCEASE_ACTION_H__

#include "2d/CCActionInterval.h"
#include "2d/CCTweenFunction.h"

namespace cocos2d {

// Ease actions are created from other interval actions.
// The ease action will change the timeline of the inner action.

class CC_DLL ActionEase : public ActionInterval
{
public:
    ActionEase(std::unique_ptr<ActionInterval>);
    
    virtual ActionInterval* getInnerAction();
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;

protected:

    virtual void at_stop() override;

protected:
    std::unique_ptr<ActionInterval> _inner;
};

//
// NOTE: Converting these macros into Templates is desirable, but please see
// issue #16159 [https://github.com/cocos2d/cocos2d-x/pull/16159] for further info
//
#define EASE_TEMPLATE_DECL_CLASS(CLASSNAME) \
class CC_DLL CLASSNAME : public ActionEase \
{ \
public: \
    CLASSNAME(std::unique_ptr<ActionInterval> action)\
        : ActionEase(std::move(action)) \
    {} \
    \
    virtual CLASSNAME* clone() const override; \
    virtual void step(float time) override; \
    virtual ActionEase* reverse() const override; \
};

// The timeline of inner action will be changed by:
// { 2 }^{ 10*(time-1) }-1*0.001
EASE_TEMPLATE_DECL_CLASS(EaseExponentialIn);

// The timeline of inner action will be changed by:
// 1-{ 2 }^{ -10*(time-1) }
EASE_TEMPLATE_DECL_CLASS(EaseExponentialOut);

// If time * 2 < 1, the timeline of inner action will be changed by:
// 0.5*{ 2 }^{ 10*(time-1) }.
// else, the timeline of inner action will be changed by:
// 0.5*(2-{ 2 }^{ -10*(time-1) })
EASE_TEMPLATE_DECL_CLASS(EaseExponentialInOut);

// The timeline of inner action will be changed by:
// 1-cos(time*\frac { \pi  }{ 2 } )
EASE_TEMPLATE_DECL_CLASS(EaseSineIn);

// The timeline of inner action will be changed by:
// sin(time*\frac { \pi  }{ 2 } )
EASE_TEMPLATE_DECL_CLASS(EaseSineOut);

// The timeline of inner action will be changed by:
// -0.5*(cos(\pi *time)-1)
EASE_TEMPLATE_DECL_CLASS(EaseSineInOut);

// This action doesn't use a bijective function.
//  Actions like Sequence might have an unexpected result when used with this action.
EASE_TEMPLATE_DECL_CLASS(EaseBounceIn);

// This action doesn't use a bijective function.
// Actions like Sequence might have an unexpected result when used with this action.
EASE_TEMPLATE_DECL_CLASS(EaseBounceOut);

// This action doesn't use a bijective function.
// Actions like Sequence might have an unexpected result when used with this action.
EASE_TEMPLATE_DECL_CLASS(EaseBounceInOut);

// This action doesn't use a bijective function.
// Actions like Sequence might have an unexpected result when used with this action.
EASE_TEMPLATE_DECL_CLASS(EaseBackIn);

// This action doesn't use a bijective function.
// Actions like Sequence might have an unexpected result when used with this action.
EASE_TEMPLATE_DECL_CLASS(EaseBackOut);

// This action doesn't use a bijective function.
// Actions like Sequence might have an unexpected result when used with this action.
EASE_TEMPLATE_DECL_CLASS(EaseBackInOut);

EASE_TEMPLATE_DECL_CLASS(EaseQuadraticActionIn);

EASE_TEMPLATE_DECL_CLASS(EaseQuadraticActionOut);

EASE_TEMPLATE_DECL_CLASS(EaseQuadraticActionInOut);

EASE_TEMPLATE_DECL_CLASS(EaseQuarticActionIn);

EASE_TEMPLATE_DECL_CLASS(EaseQuarticActionOut);

EASE_TEMPLATE_DECL_CLASS(EaseQuarticActionInOut);

EASE_TEMPLATE_DECL_CLASS(EaseQuinticActionIn);

EASE_TEMPLATE_DECL_CLASS(EaseQuinticActionOut);

EASE_TEMPLATE_DECL_CLASS(EaseQuinticActionInOut);

EASE_TEMPLATE_DECL_CLASS(EaseCircleActionIn);

EASE_TEMPLATE_DECL_CLASS(EaseCircleActionOut);

EASE_TEMPLATE_DECL_CLASS(EaseCircleActionInOut);

EASE_TEMPLATE_DECL_CLASS(EaseCubicActionIn);

EASE_TEMPLATE_DECL_CLASS(EaseCubicActionOut);

EASE_TEMPLATE_DECL_CLASS(EaseCubicActionInOut);

// Base class for Easing actions with rate parameters
// Ease the inner action with specified rate.

class CC_DLL EaseRateAction : public ActionEase
{
public:
    EaseRateAction(std::unique_ptr<ActionInterval> action, float rate)
        : ActionEase( std::move( action))
        , _rate(rate)
        {}

protected:
    float _rate;
};

//
// NOTE: Converting these macros into Templates is desirable, but please see
// issue #16159 [https://github.com/cocos2d/cocos2d-x/pull/16159] for further info
//

#define EASERATE_TEMPLATE_DECL_CLASS(CLASSNAME) \
class CC_DLL CLASSNAME : public EaseRateAction \
{ \
public: \
    CLASSNAME(std::unique_ptr<ActionInterval> action, float rate) \
        : EaseRateAction(std::move(action), rate) \
    {} \
    \
    virtual void step(float time) override; \
    virtual CLASSNAME* clone() const override; \
    virtual EaseRateAction* reverse() const override; \
};

// The timeline of inner action will be changed by:
// { time }^{ rate }
EASERATE_TEMPLATE_DECL_CLASS(EaseIn);

// The timeline of inner action will be changed by:
// { time }^ { (1/rate) }
EASERATE_TEMPLATE_DECL_CLASS(EaseOut);

// If time * 2 < 1, the timeline of inner action will be changed by:
// 0.5*{ time }^{ rate }
// Else, the timeline of inner action will be changed by:
// 1.0-0.5*{ 2-time }^{ rate }
EASERATE_TEMPLATE_DECL_CLASS(EaseInOut);

// *** EaseElastic ***

class CC_DLL EaseElastic : public ActionEase
{
public:
    EaseElastic(std::unique_ptr<ActionInterval> action, float period = 0.3f)
        : ActionEase( std::move( action))
        , _period(period)
    {}

protected:
    float _period;
};

//
// NOTE: Converting these macros into Templates is desirable, but please see
// issue #16159 [https://github.com/cocos2d/cocos2d-x/pull/16159] for further info
//
#define EASEELASTIC_TEMPLATE_DECL_CLASS(CLASSNAME) \
class CC_DLL CLASSNAME : public EaseElastic \
{ \
public: \
    explicit CLASSNAME(std::unique_ptr<ActionInterval> action, float rate = 0.3f) \
        : EaseElastic(std::move(action), rate) \
    {} \
    \
    virtual void step(float time) override; \
    virtual CLASSNAME* clone() const override; \
    virtual EaseElastic* reverse() const override; \
};

// If time == 0 or time == 1, the timeline of inner action will not be changed.
// Else, the timeline of inner action will be changed by:
// -{ 2 }^{ 10*(time-1) }*sin((time-1-\frac { period }{ 4 } )*\pi * 2 / period)
EASEELASTIC_TEMPLATE_DECL_CLASS(EaseElasticIn);

// If time == 0 or time == 1, the timeline of inner action will not be changed.
// Else, the timeline of inner action will be changed by:
// { 2 }^{ -10*time }*sin((time-\frac { period }{ 4 } )*\pi *2/period)+1
// This action doesn't use a bijective function.
// Actions like Sequence might have an unexpected result when used with this action.
EASEELASTIC_TEMPLATE_DECL_CLASS(EaseElasticOut);

// This action doesn't use a bijective function.
// Actions like Sequence might have an unexpected result when used with this action.
EASEELASTIC_TEMPLATE_DECL_CLASS(EaseElasticInOut);


class CC_DLL EaseBezierAction : public cocos2d::ActionEase
{
public:
    explicit EaseBezierAction(std::unique_ptr<ActionInterval>);

    virtual void step(float time) override;
    virtual EaseBezierAction* clone() const override;
    virtual EaseBezierAction* reverse() const override;

    void setBezierParamer(float p0, float p1, float p2, float p3);

private:
    float _p0;
    float _p1;
    float _p2;
    float _p3;
};

} // namespace cocos2d

#endif // __ACTION_CCEASE_ACTION_H__

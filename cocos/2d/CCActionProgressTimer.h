/****************************************************************************
Copyright (C) 2010      Lam Pham
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
#ifndef __ACTION_CCPROGRESS_TIMER_H__
#define __ACTION_CCPROGRESS_TIMER_H__

#include "2d/CCActionInterval.h"

namespace cocos2d {

class ProgressTimer;

// Progress to percentage.
// This action show the target node from current percentage to the specified percentage.
// You should specify the destination percentage when creating the action.

class CC_DLL ProgressFromTo : public ActionInterval
{
public:
    ProgressFromTo(float duration, float percentFrom, float percentTo);

    virtual ProgressFromTo* clone() const override;
    virtual ProgressFromTo* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    
protected:

    virtual void at_stop() override;

protected:
    ProgressTimer* _cachedTarget;
    float _from;
    float _to;
};

// Progress from a percentage to another percentage.

class CC_DLL ProgressTo : public ProgressFromTo
{
public:
    ProgressTo(float duration, float to)
        : ProgressFromTo(duration, 0.0f, to)
    {}

    virtual ProgressTo* clone() const override;
    virtual ProgressTo* reverse() const override;
    virtual void startWithTarget(Node *target) override;
};

} // namespace cocos2d

#endif // __ACTION_CCPROGRESS_TIMER_H__

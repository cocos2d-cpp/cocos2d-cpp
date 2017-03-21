/****************************************************************************
Copyright (C) 2010      Lam Pham
Copyright (c) 2010-2012 cocos2d-x.org
CopyRight (c) 2013-2016 Chukong Technologies Inc.
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
#include "2d/CCActionProgressTimer.h"
#include "2d/CCProgressTimer.h"

namespace cocos2d {

// implementation of ProgressFromTo

ProgressFromTo::ProgressFromTo(float duration, float percentFrom, float percentTo)
    : _cachedTarget( nullptr )
    , _from( percentFrom )
    , _to( percentTo )
{
    ActionInterval::initWithDuration(duration);
}

ProgressFromTo* ProgressFromTo::clone() const
{
    return new ProgressFromTo(_duration, _from, _to);
}

ProgressFromTo* ProgressFromTo::reverse() const
{
    return new ProgressFromTo(_duration, _to, _from);
}

void ProgressFromTo::startWithTarget(Node *target)
{
    _cachedTarget = dynamic_cast<ProgressTimer*>(target);
    CC_ASSERT(_cachedTarget);
    ActionInterval::startWithTarget(target);
}

void ProgressFromTo::step(float time)
{
    if (_cachedTarget)
    {
        _cachedTarget->setPercentage(_from + (_to - _from) * time);
    }
}

void ProgressFromTo::at_stop()
{
}

// implementation of ProgressTo

ProgressTo* ProgressTo::clone() const
{
    return new ProgressTo(_duration, _to);
}


ProgressTo* ProgressTo::reverse() const
{
    CCASSERT(false, "reverse() not supported in ProgressTo");
    return nullptr;
}

void ProgressTo::startWithTarget(Node *target)
{
    ProgressFromTo::startWithTarget(target);
    if (_cachedTarget)
    {
        _from = _cachedTarget->getPercentage();
    }
}

} // namespace cocos2d

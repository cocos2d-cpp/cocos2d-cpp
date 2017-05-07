/****************************************************************************
Copyright (c) 2009      lhunath (Maarten Billemont)
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
#ifndef __CCACTIONTWEEN_H__
#define __CCACTIONTWEEN_H__

#include "2d/CCActionInterval.h"

namespace cocos2d {

/**
If you want to use ActionTween on a node.
You should implement the node follow these steps:
1. The node should be inherit from ActionTweenDelegate.
2. Override the virtual method updateTweenAction in the node.
Then once you running ActionTween on the node, the method updateTweenAction will be invoked.
*/
class CC_DLL ActionTweenDelegate
{
public:
    virtual ~ActionTweenDelegate() {}

    // The callback function when ActionTween is running.
    virtual void updateTweenAction(float value, const std::string& key) = 0;
};

/** ActionTween

 ActionTween is an action that lets you update any property of an object.
 For example, if you want to modify the "width" property of a target from 200 to 300 in 2 seconds, then:

 @code
     auto modifyWidth = std::make_unique<ActionTween>(2, "width", 200, 300);
     target->runAction( std::move( modifyWidth));
 @endcode

 Another example: ScaleTo action could be rewritten using PropertyAction:

 @code
     // scaleA and scaleB are equivalents
     auto scaleA = new ScaleTo(2, 3);                 // (duration, to)
     auto scaleB = new ActionTween(2, "scale", 1, 3); // (duration, key, from, to)
 @endcode

 */
class CC_DLL ActionTween : public ActionInterval
{
public:
    ActionTween(float duration, std::string key, float from, float to);

    // Overrides
    virtual void startWithTarget(Node *target) override;
    virtual void step(float dt) override;
    virtual ActionTween* reverse() const override;
    virtual ActionTween *clone() const override;
    
protected:

    virtual void at_stop() override;

private:
    std::string _key;
    float       _from;
    float       _to;
    float       _delta;
};

} // namespace cocos2d

#endif /* __CCACTIONTWEEN_H__ */



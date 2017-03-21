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

#ifndef __CCINSTANT_ACTION_H__
#define __CCINSTANT_ACTION_H__

#include <functional>
#include "2d/CCAction.h"

namespace cocos2d {

/* 
 * Instant actions are immediate actions. They don't have a duration like the IntervalAction actions.
 */
class CC_DLL ActionInstant : public FiniteTimeAction
{
public:
    virtual bool isDone() const override;

    virtual void update(float dt) override;

    virtual void at_stop() override;
};

class CC_DLL Show : public ActionInstant
{
public:
    virtual void step(float time) override;
    virtual ActionInstant* reverse() const override;
    virtual Show* clone() const override;
};

class CC_DLL Hide : public ActionInstant
{
public:
    virtual void step(float time) override;
    virtual ActionInstant* reverse() const override;
    virtual Hide* clone() const override;
};

class CC_DLL ToggleVisibility : public ActionInstant
{
public:
    virtual void step(float time) override;
    virtual ToggleVisibility* reverse() const override;
    virtual ToggleVisibility* clone() const override;
};

class CC_DLL RemoveSelf : public ActionInstant
{
public:
    explicit RemoveSelf(bool isNeedCleanUp = true);

    virtual void step(float time) override;
    virtual RemoveSelf* clone() const override;
    virtual RemoveSelf* reverse() const override;
    
private:
    bool _isNeedCleanUp;
};

class CC_DLL FlipX : public ActionInstant
{
public:
    // Flips the sprite horizontally if true.
    explicit FlipX(bool x);

    void startWithTarget(Node *target);
    virtual void step(float time) override;
    virtual FlipX* reverse() const override;
    virtual FlipX* clone() const override;
    
private:
    bool    _flipX;
};

class CC_DLL FlipY : public ActionInstant
{
public:
    // Flips the sprite vertically if true.
    explicit FlipY(bool y);

    void startWithTarget(Node *target);
    virtual void step(float time) override;
    virtual FlipY* reverse() const override;
    virtual FlipY* clone() const override;
    
private:
    bool    _flipY;
};

/** @class Place
* @brief Places the node in a certain position.
*/
class CC_DLL Place : public ActionInstant
{
public:
    // Creates a Place action with a position.
    explicit Place(const Vec2& pos);

    virtual void step(float time) override;
    virtual Place* reverse() const override;
    virtual Place* clone() const override;
    
private:
    Vec2 _position;
};

class CC_DLL CallFunc : public ActionInstant
{
public:
    explicit CallFunc(std::function<void()> func);

public:
    virtual void step(float time) override;
    virtual CallFunc* reverse() const override;
    virtual CallFunc* clone() const override;
    
private:
    std::function<void()> _function;
};

class CC_DLL CallFuncN : public ActionInstant
{
public:
    explicit CallFuncN(std::function<void(Node*)> func);

public:
    virtual void step(float time) override;
    virtual CallFuncN* reverse() const override;
    virtual CallFuncN* clone() const override;
    
private:
    std::function<void(Node*)> _function;
};

} // namespace cocos2d

#endif //__CCINSTANT_ACTION_H__

/****************************************************************************
 Copyright (c) 2008-2010 Ricardo Quesada
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2011      Zynga Inc.
 Copyright (c) 2013-2016 Chukong Technologies Inc.

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

#ifndef __CCINSTANT_ACTION_H__
#define __CCINSTANT_ACTION_H__

#include <functional>
#include "2d/CCAction.h"

namespace cocos2d {

/**
 * @addtogroup actions
 * @{
 */

/** @class ActionInstant
* @brief Instant actions are immediate actions. They don't have a duration like the IntervalAction actions.
**/
class CC_DLL ActionInstant : public FiniteTimeAction
{
public:
    //
    // Overrides
    //
    virtual ActionInstant * reverse() const override
    {
        CC_ASSERT(0);
        return nullptr;
    }

    virtual bool isDone() const override;
    /**
     * @param dt In seconds.
     */
    virtual void update(float dt) override;
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;

    virtual void at_stop() override;
};

/** @class Show
* @brief Show the node.
**/
class CC_DLL Show : public ActionInstant
{
public:
    /** Allocates and initializes the action.
     *
     * @return  An autoreleased Show object.
     */
    static Show * create();

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    virtual ActionInstant* reverse() const override;
    virtual Show* clone() const override;

protected:
    Show(){}
    virtual ~Show(){}

private:
    Show(const Show &) = delete;
    const Show & operator=(const Show &) = delete;
};

/** @class Hide
* @brief Hide the node.
*/
class CC_DLL Hide : public ActionInstant
{
public:
    /** Allocates and initializes the action.
     *
     * @return An autoreleased Hide object.
     */
    static Hide * create();

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    virtual ActionInstant* reverse() const override;
    virtual Hide* clone() const override;

protected:
    Hide(){}
    virtual ~Hide(){}

private:
    Hide(const Hide &) = delete;
    const Hide & operator=(const Hide &) = delete;
};

/** @class ToggleVisibility
* @brief Toggles the visibility of a node.
*/
class CC_DLL ToggleVisibility : public ActionInstant
{
public:
    /** Allocates and initializes the action.
     *
     * @return An autoreleased ToggleVisibility object.
     */
    static ToggleVisibility * create();

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    virtual ToggleVisibility* reverse() const override;
    virtual ToggleVisibility* clone() const override;

protected:
    ToggleVisibility(){}
    virtual ~ToggleVisibility(){}

private:
    ToggleVisibility(const ToggleVisibility &) = delete;
    const ToggleVisibility & operator=(const ToggleVisibility &) = delete;
};

/** @class RemoveSelf
* @brief Remove the node.
*/
class CC_DLL RemoveSelf : public ActionInstant
{
public:
    /** Create the action.
     *
     * @param isNeedCleanUp Is need to clean up, the default value is true.
     * @return An autoreleased RemoveSelf object.
     */
    static RemoveSelf * create(bool isNeedCleanUp = true);

    //
    // Override
    //
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    virtual RemoveSelf* clone() const override;
    virtual RemoveSelf* reverse() const override;
    
protected:
    RemoveSelf() : _isNeedCleanUp(true){}
    virtual ~RemoveSelf(){}

    /** init the action */
    bool init(bool isNeedCleanUp);

protected:
    bool _isNeedCleanUp;

private:
    RemoveSelf(const RemoveSelf &) = delete;
    const RemoveSelf & operator=(const RemoveSelf &) = delete;
};

/** @class FlipX
* @brief Flips the sprite horizontally.
* @since v0.99.0
*/
class CC_DLL FlipX : public ActionInstant
{
public:
    /** Create the action.
     *
     * @param x Flips the sprite horizontally if true.
     * @return  An autoreleased FlipX object.
     */
    static FlipX * create(bool x);

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    virtual FlipX* reverse() const override;
    virtual FlipX* clone() const override;
    
protected:
    FlipX() :_flipX(false) {}
    virtual ~FlipX() {}

    /** init the action */
    bool initWithFlipX(bool x);

protected:
    bool    _flipX;

private:
    FlipX(const FlipX &) = delete;
    const FlipX & operator=(const FlipX &) = delete;
};

/** @class FlipY
* @brief Flips the sprite vertically.
* @since v0.99.0
*/
class CC_DLL FlipY : public ActionInstant
{
public:
    /** Create the action.
     *
     * @param y Flips the sprite vertically if true.
     * @return An autoreleased FlipY object.
     */
    static FlipY * create(bool y);

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    virtual FlipY* reverse() const override;
    virtual FlipY* clone() const override;
    
protected:
    FlipY() :_flipY(false) {}
    virtual ~FlipY() {}

    /** init the action */
    bool initWithFlipY(bool y);

protected:
    bool    _flipY;

private:
    FlipY(const FlipY &) = delete;
    const FlipY & operator=(const FlipY &) = delete;
};

/** @class Place
* @brief Places the node in a certain position.
*/
class CC_DLL Place : public ActionInstant
{
public:

    /** Creates a Place action with a position.
     *
     * @param pos  A certain position.
     * @return  An autoreleased Place object.
     */
    static Place * create(const Vec2& pos);

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    virtual Place* reverse() const override;
    virtual Place* clone() const override;
    
protected:
    Place(){}
    virtual ~Place(){}

    /** Initializes a Place action with a position */
    bool initWithPosition(const Vec2& pos);

protected:
    Vec2 _position;

private:
    Place(const Place &) = delete;
    const Place & operator=(const Place &) = delete;
};


/** @class CallFunc
* @brief Calls a 'callback'.
*/
class CC_DLL CallFunc : public ActionInstant
{
public:
    /** Creates the action with the callback of type std::function<void()>.
     This is the preferred way to create the callback.
     * When this function bound in js or lua ,the input param will be changed.
     * In js: var create(var func, var this, var [data]) or var create(var func).
     * In lua:local create(local funcID).
     *
     * @param func  A callback function need to be executed.
     * @return  An autoreleased CallFunc object.
     */
    static CallFunc * create(const std::function<void()>& func);

public:
    /** Executes the callback.
     */
    virtual void execute();

    //
    // Overrides
    //
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    virtual CallFunc* reverse() const override;
    virtual CallFunc* clone() const override;
    
protected:
    CallFunc()
    : _function(nullptr)
    {
    }
    virtual ~CallFunc();

    /** initializes the action with the std::function<void()>
     * @lua NA
     */
    bool initWithFunction(const std::function<void()>& func);

protected:

    /** function that will be called */
    std::function<void()> _function;

private:
    CallFunc(const CallFunc &) = delete;
    const CallFunc & operator=(const CallFunc &) = delete;
};

/** @class CallFuncN
* @brief Calls a 'callback' with the node as the first argument. N means Node.
* @js NA
*/
class CC_DLL CallFuncN : public CallFunc
{
public:
    /** Creates the action with the callback of type std::function<void()>.
     This is the preferred way to create the callback.
     *
     * @param func  A callback function need to be executed.
     * @return  An autoreleased CallFuncN object.
     */
    static CallFuncN * create(const std::function<void(Node*)>& func);

    //
    // Overrides
    //
    virtual CallFuncN* clone() const override;
    virtual void execute() override;
    
protected:
    CallFuncN():_functionN(nullptr){}
    virtual ~CallFuncN(){}

    /** initializes the action with the std::function<void(Node*)> */
    bool initWithFunction(const std::function<void(Node*)>& func);
    
protected:
    /** function that will be called with the "sender" as the 1st argument */
    std::function<void(Node*)> _functionN;

private:
    CallFuncN(const CallFuncN &) = delete;
    const CallFuncN & operator=(const CallFuncN &) = delete;
};

// end of actions group
/// @}

} // namespace cocos2d

#endif //__CCINSTANT_ACTION_H__

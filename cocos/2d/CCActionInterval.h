/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.
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

#ifndef __ACTION_CCINTERVAL_ACTION_H__
#define __ACTION_CCINTERVAL_ACTION_H__

#include <vector>

#include "2d/CCAction.h"
#include "2d/CCAnimation.h"
#include "base/CCProtocols.h"

namespace cocos2d {

class Node;
class SpriteFrame;
class EventCustom;

/*
An interval action is an action that takes place within a certain period of time.
It has an start time, and a finish time. The finish time is the parameter
duration plus the start time.

These ActionInterval actions have some interesting properties, like:
- They can run normally (default)
- They can run reversed with the reverse method
- They can run with the time altered with the Accelerate, AccelDeccel and Speed actions.

For example, you can simulate a Ping Pong effect running the action normally and
then running it again in Reverse mode.

Example:

auto action = std::make_unique<MoveBy>(1.0f, Vec2::ONE);
auto rev_action = std::unique_ptr<MoveBy>(action->reverse());
auto pingPongAction = std::make_unique<Sequence>(std::move(action), std::move(rev_action));
*/

class CC_DLL ActionInterval : public FiniteTimeAction
{
public:
    ActionInterval()
    {}

    explicit ActionInterval(float d)
    {
        initWithDuration(d);
    }

    /** How many seconds had elapsed since the actions started to run.
     *
     * @return The seconds had elapsed since the actions started to run.
     */
    float getElapsed() { return _elapsed; }

    void setAmplitudeRate(float amp);

    virtual bool isDone() const override;
    /**
     * @param dt in seconds
     */
    virtual void update(float dt) override;
    virtual void startWithTarget(Node *target) override;
    virtual ActionInterval* reverse() const override
    {
        CC_ASSERT(0);
        return nullptr;
    }

    virtual ActionInterval* clone() const override = 0;

protected:
    /** initializes the action */
    bool initWithDuration(float d);

protected:
    float _elapsed;
    bool   _firstTick;
};

/** @class Speed
 * @brief Changes the speed of an action, making it take longer (speed>1)
 * or shorter (speed<1) time.
 * Useful to simulate 'slow motion' or 'fast forward' effect.
 * @warning This action can't be Sequenceable because it is not an IntervalAction.
 */
class CC_DLL Speed : public Action
{
public:
    Speed(std::unique_ptr<ActionInterval> action, float speed);

    /*
    float getSpeed() const { return _speed; }
    void setSpeed(float speed) { _speed = speed; }

    void setInnerAction(ActionInterval *action);
    ActionInterval* getInnerAction() const { return _innerAction; }
    */

    virtual Speed* clone() const override;
    virtual Speed* reverse() const override;

    virtual void startWithTarget(Node* target) override;

    virtual void update(float dt) override;
    virtual void step(float time) override;
    virtual bool isDone() const override;
    
protected:

    virtual void at_stop() override;

protected:
    float _speed;
    std::unique_ptr<ActionInterval> _innerAction;

private:
    Speed(const Speed &) = delete;
    const Speed & operator=(const Speed &) = delete;
};

/** @class Sequence
 * @brief Runs actions sequentially, one after another.
 */
class CC_DLL Sequence : public ActionInterval
{
public:
    using actions_container = std::vector<std::unique_ptr<FiniteTimeAction>>;

    Sequence(actions_container arrayOfActions);

    template<typename ...AA>
    Sequence(actions_container array, actions_container::value_type action, AA ...actions)
        : Sequence([](auto array, auto action) {
                       array.push_back(std::move(action));
                       return array;
                   } (std::move(array), actions_container::value_type(action.release())),
                   std::forward<AA>(actions)...)
    {
    }

    template<typename A, typename ...AA>
    Sequence(actions_container array, A action, AA ...actions)
        : Sequence([](auto array, auto action) {
                       array.push_back(std::move(action));
                       return array;
                   } (std::move(array), std::move(action)),
                   std::forward<AA>(actions)...)
    {
        static_assert(
            std::is_convertible<decltype(action.get()), FiniteTimeAction*>::value,
            "Sequence construtors accept only unique_ptr<Derived_from_FiniteTimeAction>'s"
        );
    }

    template<typename ...AA>
    Sequence(actions_container::value_type action, AA ...actions)
        : Sequence([](auto a) -> actions_container {
                       actions_container vec;
                       vec.push_back( std::move( a));
                       return vec;
                   }(std::move(action)),
                   std::forward<AA>(actions)...)
    {
    }

    template<typename A, typename ...AA>
    Sequence(A action, AA ...actions)
        : Sequence(actions_container::value_type(action.release()), std::forward<AA>(actions)...)
    {
        static_assert(
            std::is_convertible<decltype(action.get()), FiniteTimeAction*>::value,
            "Sequence construtors accept only unique_ptr<Derived_from_FiniteTimeAction>'s"
        );
    }

    virtual Sequence* clone() const override;
    virtual Sequence* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    
protected:

    virtual void at_stop() override;

private:
    void initWithTwoActions(std::unique_ptr<FiniteTimeAction> actionOne,
                            std::unique_ptr<FiniteTimeAction> actionTwo);

protected:
    std::unique_ptr<FiniteTimeAction> _actions[2];
    float _split;
    int _last;
};

/** @class Repeat
 * @brief Repeats an action a number of times.
 * To repeat an action forever use the RepeatForever action.
 */
class CC_DLL Repeat : public ActionInterval
{
public:
    Repeat(std::unique_ptr<FiniteTimeAction> action, unsigned int times);

    virtual Repeat* clone() const override;
    virtual Repeat* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    /**
     * @param dt In seconds.
     */
    virtual void step(float time) override;
    virtual bool isDone() const override;
    
protected:

    virtual void at_stop() override;

protected:
    std::unique_ptr<FiniteTimeAction> _innerAction;
    unsigned int _times;
    unsigned int _total;
    float _nextTime;
    bool _actionInstant;

private:
    Repeat(const Repeat &) = delete;
    const Repeat & operator=(const Repeat &) = delete;
};

/** @class RepeatForever
 * @brief Repeats an action for ever.
 To repeat the an action for a limited number of times use the Repeat action.
 * @warning This action can't be Sequenceable because it is not an IntervalAction.
 */
class CC_DLL RepeatForever : public ActionInterval
{
public:
    RepeatForever(std::unique_ptr<ActionInterval>);

    //
    // Overrides
    //
    virtual RepeatForever* clone() const override;
    virtual RepeatForever* reverse() const override;
    virtual void startWithTarget(Node* target) override;
    /**
     * @param dt In seconds.
     */
    virtual void update(float dt) override;
    virtual void step(float time) override;
    virtual bool isDone() const override;
    
protected:
    RepeatForever()
    : _innerAction(nullptr)
    {}

    /** initializes the action */
    bool initWithAction(ActionInterval *action);

    virtual void at_stop() override;

protected:
    /** Inner action */
    std::unique_ptr<ActionInterval> _innerAction;

private:
    RepeatForever(const RepeatForever &) = delete;
    const RepeatForever & operator=(const RepeatForever &) = delete;
};

// Spawn a new action immediately

class CC_DLL Spawn : public ActionInterval
{
public:
    using actions_container = std::vector<std::unique_ptr<FiniteTimeAction>>;

    Spawn(actions_container arrayOfActions);

    template<typename ...AA>
    Spawn(actions_container array, actions_container::value_type action, AA ...actions)
        : Spawn([](auto array, auto action) {
                       array.push_back(std::move(action));
                       return array;
                   } (std::move(array), actions_container::value_type(action.release())),
                   std::forward<AA>(actions)...)
    {
    }

    template<typename A, typename ...AA>
    Spawn(actions_container array, A action, AA ...actions)
        : Spawn([](auto array, auto action) {
                       array.push_back(std::move(action));
                       return array;
                   } (std::move(array), std::move(action)),
                   std::forward<AA>(actions)...)
    {
        static_assert(
            std::is_convertible<decltype(action.get()), FiniteTimeAction*>::value,
            "Spawn construtors accept only unique_ptr<Derived_from_FiniteTimeAction>'s"
        );
    }

    template<typename ...AA>
    Spawn(actions_container::value_type action, AA ...actions)
        : Spawn([](auto action) -> actions_container {
                    actions_container vec;
                    vec.push_back( std::move( action));
                    return vec;
                }(std::move(action)),
                std::forward<AA>(actions)...)
    {
    }

    template<typename A, typename ...AA>
    Spawn(A action, AA ...actions)
        : Spawn(actions_container::value_type(action.release()), std::forward<AA>(actions)...)
    {
        static_assert(
            std::is_convertible<decltype(action.get()), FiniteTimeAction*>::value,
            "Spawn construtors accept only unique_ptr<Derived_from_FiniteTimeAction>'s"
        );
    }

    virtual Spawn* clone() const override;
    virtual Spawn* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    
protected:
    void initWithTwoActions(std::unique_ptr<FiniteTimeAction> action1,
                            std::unique_ptr<FiniteTimeAction> action2);

    virtual void at_stop() override;

protected:
    std::unique_ptr<FiniteTimeAction> _one;
    std::unique_ptr<FiniteTimeAction> _two;

private:
    Spawn(const Spawn &) = delete;
    const Spawn & operator=(const Spawn &) = delete;
};

// Rotates a Node object to a certain angle by modifying it's rotation attribute.
// The direction will be decided by the shortest angle.
// Angles in degreesCW

class CC_DLL RotateTo : public ActionInterval
{
public:
    RotateTo(float duration, float dstAngleX, float dstAngleY);

    RotateTo(float duration, float dstAngle)
        : RotateTo(duration, dstAngle, dstAngle)
        {}

    RotateTo(float duration, const Vec3& dstAngle3D);

    virtual RotateTo* clone() const override;
    virtual RotateTo* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    
protected:
    /** 
     * calculates the start and diff angles
     * @param dstAngle in degreesCW
     */
    void calculateAngles(float &startAngle, float &diffAngle, float dstAngle);
    
    virtual void at_stop() override;

protected:
    bool _is3D;
    Vec3 _dstAngle;
    Vec3 _startAngle;
    Vec3 _diffAngle;
};

// Rotates a Node object clockwise a number of degrees by modifying it's rotation attribute.
// deltaAngle In degreesCW.

class CC_DLL RotateBy : public ActionInterval
{
public:
    RotateBy(float duration, float deltaAngleZ_X, float deltaAngleZ_Y);

    RotateBy(float duration, float deltaAngle)
        : RotateBy(duration, deltaAngle, deltaAngle)
    {}

    RotateBy(float duration, const Vec3& deltaAngle3D);

    virtual RotateBy* clone() const override;
    virtual RotateBy* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    
protected:
    
    virtual void at_stop() override;

protected:
    bool _is3D;
    Vec3 _deltaAngle;
    Vec3 _startAngle;
};

// Moves a Node object x,y pixels by modifying it's position attribute.
// x and y are relative to the position of the object.
// Several MoveBy actions can be concurrently called, and the resulting
// movement will be the sum of individual movements.

class CC_DLL MoveBy : public ActionInterval
{
public:
    MoveBy(float duration, const Vec2& deltaPosition)
        : MoveBy(duration, Vec3(deltaPosition.x, deltaPosition.y, 0))
        {}

    MoveBy(float duration, Vec3 deltaPosition);

    virtual MoveBy* clone() const override;
    virtual MoveBy* reverse() const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    
protected:

    virtual void at_stop() override;

protected:
    Vec3 _positionDelta;
    Vec3 _startPosition;
    Vec3 _previousPosition;
};

// Moves a Node object to the position x,y. x and y are absolute coordinates by modifying it's position attribute.
// Several MoveTo actions can be concurrently called, and the resulting
// movement will be the sum of individual movements.

class CC_DLL MoveTo : public MoveBy
{
public:
    MoveTo(float duration, const Vec2& position)
        : MoveTo(duration, Vec3(position.x, position.y, 0))
        {}

    MoveTo(float duration, Vec3 position);

    virtual MoveTo* clone() const override;
    virtual MoveTo* reverse() const  override;
    virtual void startWithTarget(Node *target) override;
    
protected:

    Vec3 _endPosition;
};

// Skews a Node object to given angles by modifying it's skewX and skewY attributes

class CC_DLL SkewTo : public ActionInterval
{
public:
    SkewTo(float t, float sx, float sy);

    virtual SkewTo* clone() const override;
    virtual SkewTo* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    
protected:

    virtual void at_stop() override;

protected:
    float _skewX;
    float _skewY;
    float _startSkewX;
    float _startSkewY;
    float _endSkewX;
    float _endSkewY;
    float _deltaX;
    float _deltaY;
};

// Skews a Node object by skewX and skewY degrees.

class CC_DLL SkewBy : public SkewTo
{
public:
    SkewBy(float t, float deltaSkewX, float deltaSkewY);

    virtual void startWithTarget(Node *target) override;
    virtual SkewBy* clone() const override;
    virtual SkewBy* reverse() const override;
};

// Resize a Node object to the final size by modifying it's Size attribute.

class  CC_DLL ResizeTo : public ActionInterval 
{
public:
    ResizeTo(float duration, const Size& final_size);

    virtual ResizeTo* clone() const override;
    void startWithTarget(Node* target) override;
    void step(float time) override;

protected:

    virtual void at_stop() override;

protected:
    Size _initialSize;
    Size _finalSize;
    Size _sizeDelta;
};


// Resize a Node object by a Size. Works on all nodes where setContentSize is effective. But it's mostly useful for nodes where 9-slice is enabled

class CC_DLL ResizeBy : public ActionInterval 
{
public:
    ResizeBy(float duration, Size deltaSize);
    
    virtual ResizeBy* clone() const override;
    virtual ResizeBy* reverse() const  override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;

protected:

    virtual void at_stop() override;

protected:
    Size _sizeDelta;
    Size _startSize;
    Size _previousSize;
};


// Moves a Node object simulating a parabolic jump movement by modifying it's position attribute.

class CC_DLL JumpBy : public ActionInterval
{
public:
    JumpBy(float duration, Vec2 distance, float height, int nJumps);

    virtual JumpBy* clone() const override;
    virtual JumpBy* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    
protected:

    virtual void at_stop() override;

protected:
    Vec2           _startPosition;
    Vec2           _delta;
    float           _height;
    int             _jumps;
    Vec2           _previousPos;
};

/** @class JumpTo
 * @brief Moves a Node object to a parabolic position simulating a jump movement by modifying it's position attribute.
*/ 
class CC_DLL JumpTo : public JumpBy
{
public:
    JumpTo(float duration, Vec2 position, float height, int nJumps);

    virtual void startWithTarget(Node *target) override;
    virtual JumpTo* clone() const override;
    virtual JumpTo* reverse() const override;

protected:
    Vec2 _endPosition;
};

// Bezier configuration structure

struct BezierConfig {
    Vec2 endPosition;
    Vec2 controlPoint_1;
    Vec2 controlPoint_2;
};

// An action that moves the target with a cubic Bezier curve by a certain distance.

class CC_DLL BezierBy : public ActionInterval
{
public:
    BezierBy(float duration, const BezierConfig& c);

    virtual BezierBy* clone() const override;
    virtual BezierBy* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    
protected:

    virtual void at_stop() override;

protected:
    BezierConfig _config;

    Vec2 _startPosition;
    Vec2 _previousPosition;
};

// An action that moves the target with a cubic Bezier curve to a destination point.

class CC_DLL BezierTo : public BezierBy
{
public:
    BezierTo(float duration, const BezierConfig& c);

    virtual void startWithTarget(Node *target) override;
    virtual BezierTo* clone() const override;
    virtual BezierTo* reverse() const override;
    
protected:
    BezierConfig _toConfig;
};

/** @class ScaleTo
 @brief Scales a Node object to a zoom factor by modifying it's scale attribute.
 @warning This action doesn't support "reverse".
 @warning The physics body contained in Node doesn't support this action.
 */
class CC_DLL ScaleTo : public ActionInterval
{
public:
    virtual ~ScaleTo() {}

    /** 
     * Creates the action with the same scale factor for X and Y.
     * @param duration Duration time, in seconds.
     * @param s Scale factor of x and y.
     * @return An autoreleased ScaleTo object.
     */
    static std::unique_ptr<ScaleTo> create(float duration, float s);

    /** 
     * Creates the action with and X factor and a Y factor.
     * @param duration Duration time, in seconds.
     * @param sx Scale factor of x.
     * @param sy Scale factor of y.
     * @return An autoreleased ScaleTo object.
     */
    static std::unique_ptr<ScaleTo> create(float duration, float sx, float sy);

    /** 
     * Creates the action with X Y Z factor.
     * @param duration Duration time, in seconds.
     * @param sx Scale factor of x.
     * @param sy Scale factor of y.
     * @param sz Scale factor of z.
     * @return An autoreleased ScaleTo object.
     */
    static std::unique_ptr<ScaleTo> create(float duration, float sx, float sy, float sz);

    //
    // Overrides
    //
    virtual ScaleTo* clone() const override;
    virtual ScaleTo* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    
protected:
    ScaleTo() {}

    /** 
     * initializes the action with the same scale factor for X and Y
     * @param duration in seconds
     */
    bool initWithDuration(float duration, float s);
    /** 
     * initializes the action with and X factor and a Y factor 
     * @param duration in seconds
     */
    bool initWithDuration(float duration, float sx, float sy);
    /** 
     * initializes the action with X Y Z factor 
     * @param duration in seconds
     */
    bool initWithDuration(float duration, float sx, float sy, float sz);

    virtual void at_stop() override;

protected:
    float _scaleX;
    float _scaleY;
    float _scaleZ;
    float _startScaleX;
    float _startScaleY;
    float _startScaleZ;
    float _endScaleX;
    float _endScaleY;
    float _endScaleZ;
    float _deltaX;
    float _deltaY;
    float _deltaZ;

private:
    ScaleTo(const ScaleTo &) = delete;
    const ScaleTo & operator=(const ScaleTo &) = delete;
};

/** @class ScaleBy
 * @brief Scales a Node object a zoom factor by modifying it's scale attribute.
 @warning The physics body contained in Node doesn't support this action.
*/
class CC_DLL ScaleBy : public ScaleTo
{
public:
    virtual ~ScaleBy() {}

    /** 
     * Creates the action with the same scale factor for X and Y.
     * @param duration Duration time, in seconds.
     * @param s Scale factor of x and y.
     * @return An autoreleased ScaleBy object.
     */
    static std::unique_ptr<ScaleBy> create(float duration, float s);

    /** 
     * Creates the action with X Y Z factor.
     * @param duration Duration time, in seconds.
     * @param sx Scale factor of x.
     * @param sy Scale factor of y.
     * @param sz Scale factor of z.
     * @return An autoreleased ScaleBy object.
     */
    static std::unique_ptr<ScaleBy> create(float duration, float sx, float sy, float sz = 1.0f);

    //
    // Overrides
    //
    virtual void startWithTarget(Node *target) override;
    virtual ScaleBy* clone() const override;
    virtual ScaleBy* reverse() const override;

protected:
    ScaleBy() {}

private:
    ScaleBy(const ScaleBy &) = delete;
    const ScaleBy & operator=(const ScaleBy &) = delete;
};

/** @class Blink
 * @brief Blinks a Node object by modifying it's visible attribute.
*/
class CC_DLL Blink : public ActionInterval
{
public:
    virtual ~Blink() {}
    /** 
     * Creates the action.
     * @param duration Duration time, in seconds.
     * @param blinks Blink times.
     * @return An autoreleased Blink object.
     */
    static std::unique_ptr<Blink> create(float duration, int blinks);

    //
    // Overrides
    //
    virtual Blink* clone() const override;
    virtual Blink* reverse() const override;
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    virtual void startWithTarget(Node *target) override;
    
protected:
    Blink() {}

    /** 
     * initializes the action 
     * @param duration in seconds
     */
    bool initWithDuration(float duration, int blinks);
    
    virtual void at_stop() override;

protected:
    int _times;
    bool _originalState;

private:
    Blink(const Blink &) = delete;
    const Blink & operator=(const Blink &) = delete;
};


/** @class FadeTo
 * @brief Fades an object that implements the RGBAProtocol protocol. It modifies the opacity from the current value to a custom one.
 @warning This action doesn't support "reverse"
 */
class CC_DLL FadeTo : public ActionInterval
{
    friend class FadeIn;
    friend class FadeOut;
public:
    virtual ~FadeTo() {}
    /** 
     * Creates an action with duration and opacity.
     * @param duration Duration time, in seconds.
     * @param opacity A certain opacity, the range is from 0 to 255.
     * @return An autoreleased FadeTo object.
     */
    static std::unique_ptr<FadeTo> create(float duration, GLubyte opacity);

    //
    // Overrides
    //
    virtual FadeTo* clone() const override;
    virtual FadeTo* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    
protected:
    FadeTo() {}

    /** 
     * initializes the action with duration and opacity 
     * @param duration in seconds
     */
    bool initWithDuration(float duration, GLubyte opacity);

    virtual void at_stop() override;

private:
    GLubyte _toOpacity;
    GLubyte _fromOpacity;

private:
    FadeTo(const FadeTo &) = delete;
    const FadeTo & operator=(const FadeTo &) = delete;
};


/** @class FadeIn
 * @brief Fades In an object that implements the RGBAProtocol protocol. It modifies the opacity from 0 to 255.
 The "reverse" of this action is FadeOut
 */
class CC_DLL FadeOut;

class CC_DLL FadeIn : public FadeTo
{
public:
    virtual ~FadeIn() {}
    /** 
     * Creates the action.
     * @param d Duration time, in seconds.
     * @return An autoreleased FadeIn object.
     */
    static std::unique_ptr<FadeIn> create(float d);

    //
    // Overrides
    //
    virtual void startWithTarget(Node *target) override;
    virtual FadeIn* clone() const override;
    virtual FadeTo* reverse() const override;

    /**
     * @js NA
     */
    void setReverseAction(std::unique_ptr<FadeOut> ac);

protected:
    FadeIn() = default;

private:
    FadeIn(const FadeIn &) = delete;
    const FadeIn & operator=(const FadeIn &) = delete;

private:
    std::unique_ptr<FadeOut> _reverseAction;
};

/** @class FadeOut
 * @brief Fades Out an object that implements the RGBAProtocol protocol. It modifies the opacity from 255 to 0.
 The "reverse" of this action is FadeIn
*/
class CC_DLL FadeOut : public FadeTo
{
public:
    virtual ~FadeOut() {}
    /** 
     * Creates the action.
     * @param d Duration time, in seconds.
     */
    static std::unique_ptr<FadeOut> create(float d);

    //
    // Overrides
    //
    virtual void startWithTarget(Node *target) override;
    virtual FadeOut* clone() const override;
    virtual FadeTo* reverse() const override;

    /**
     * @js NA
     */
    void setReverseAction(std::unique_ptr<FadeIn> ac);

protected:
    FadeOut() = default;

private:
    std::unique_ptr<FadeIn> _reverseAction;

private:
    FadeOut(const FadeOut &) = delete;
    const FadeOut & operator=(const FadeOut &) = delete;
};

/** @class TintTo
 * @brief Tints a Node that implements the NodeRGB protocol from current tint to a custom one.
 @warning This action doesn't support "reverse"
 @since v0.7.2
*/
class CC_DLL TintTo : public ActionInterval
{
public:
    virtual ~TintTo() {}
    /** 
     * Creates an action with duration and color.
     * @param duration Duration time, in seconds.
     * @param red Red Color, from 0 to 255.
     * @param green Green Color, from 0 to 255.
     * @param blue Blue Color, from 0 to 255.
     * @return An autoreleased TintTo object.
     */
    static std::unique_ptr<TintTo> create(float duration, GLubyte red, GLubyte green, GLubyte blue);
    /**
     * Creates an action with duration and color.
     * @param duration Duration time, in seconds.
     * @param color It's a Color3B type.
     * @return An autoreleased TintTo object.
     */
    static std::unique_ptr<TintTo> create(float duration, const Color3B& color);

    //
    // Overrides
    //
    virtual TintTo* clone() const override;
    virtual TintTo* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    
protected:
    TintTo() {}

    /** initializes the action with duration and color */
    bool initWithDuration(float duration, GLubyte red, GLubyte green, GLubyte blue);

    virtual void at_stop() override;

protected:
    Color3B _to;
    Color3B _from;

private:
    TintTo(const TintTo &) = delete;
    const TintTo & operator=(const TintTo &) = delete;
};

/** @class TintBy
 @brief Tints a Node that implements the NodeRGB protocol from current tint to a custom one.
 @since v0.7.2
 */
class CC_DLL TintBy : public ActionInterval
{
public:
    virtual ~TintBy() {}
    /** 
     * Creates an action with duration and color.
     * @param duration Duration time, in seconds.
     * @param deltaRed Delta red color.
     * @param deltaGreen Delta green color.
     * @param deltaBlue Delta blue color.
     * @return An autoreleased TintBy object.
     */
    static std::unique_ptr<TintBy> create(float duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue);

    //
    // Overrides
    //
    virtual TintBy* clone() const override;
    virtual TintBy* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    
protected:
    TintBy() {}

    /** initializes the action with duration and color */
    bool initWithDuration(float duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue);

    virtual void at_stop() override;

protected:
    GLshort _deltaR;
    GLshort _deltaG;
    GLshort _deltaB;

    GLshort _fromR;
    GLshort _fromG;
    GLshort _fromB;

private:
    TintBy(const TintBy &) = delete;
    const TintBy & operator=(const TintBy &) = delete;
};

// Delays the action a certain amount of seconds.

class CC_DLL DelayTime : public ActionInterval
{
public:
    explicit DelayTime(float d);

    virtual void step(float time) override;
    virtual DelayTime* reverse() const override;
    virtual DelayTime* clone() const override;

protected:

    virtual void at_stop() override;
};

/** @class ReverseTime
 * @brief Executes an action in reverse order, from time=duration to time=0
 
 @warning Use this action carefully. This action is not
 sequenceable. Use it as the default "reversed" method
 of your own actions, but using it outside the "reversed"
 scope is not recommended.
*/
class CC_DLL ReverseTime : public ActionInterval
{
public:
    static std::unique_ptr<ReverseTime> create(std::unique_ptr<FiniteTimeAction> action);

    //
    // Overrides
    //
    virtual ReverseTime* reverse() const override;
    virtual ReverseTime* clone() const override;
    virtual void startWithTarget(Node *target) override;
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    
protected:
    ReverseTime() = default;

    bool initWithAction(std::unique_ptr<FiniteTimeAction> action);

    virtual void at_stop() override;

protected:
    std::unique_ptr<FiniteTimeAction> _other;

private:
    ReverseTime(const ReverseTime &) = delete;
    const ReverseTime & operator=(const ReverseTime &) = delete;
};

class Texture2D;
/** @class Animate
 * @brief Animates a sprite given the name of an Animation.
 */
class CC_DLL Animate : public ActionInterval
{
public:
    virtual ~Animate();
    /** Creates the action with an Animation and will restore the original frame when the animation is over.
     *
     * @param animation A certain animation.
     * @return An autoreleased Animate object.
     */
    static std::unique_ptr<Animate> create(std::unique_ptr<Animation>);

    /** Sets the Animation object to be animated 
     * 
     * @param animation certain animation.
     */
    void setAnimation(std::unique_ptr<Animation>);
    /** returns the Animation object that is being animated 
     *
     * @return Gets the animation object that is being animated.
     */
    Animation* getAnimation() { return _animation.get(); }
    const Animation* getAnimation() const { return _animation.get(); }

    /**
     * Gets the index of sprite frame currently displayed.
     * @return int  the index of sprite frame currently displayed.
     */
    int getCurrentFrameIndex() { return _currFrameIndex; }
    //
    // Overrides
    //
    virtual Animate* clone() const override;
    virtual Animate* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    /**
     * @param t In seconds.
     */
    virtual void step(float t) override;
    
protected:
    Animate();

    /** initializes the action with an Animation and will restore the original frame when the animation is over */
    bool initWithAnimation(std::unique_ptr<Animation>);

    virtual void at_stop() override;

protected:
    std::vector<float>* _splitTimes;
    int             _nextFrame;
    SpriteFrame*    _origFrame;
    int _currFrameIndex;
    unsigned int    _executedLoops;
    std::unique_ptr<Animation> _animation;

    EventCustom*    _frameDisplayedEvent;
    AnimationFrame::DisplayedEventInfo _frameDisplayedEventInfo;

private:
    Animate(const Animate &) = delete;
    const Animate & operator=(const Animate &) = delete;
};

/** @class TargetedAction
 * @brief Overrides the target of an action so that it always runs on the target
 * specified at action creation rather than the one specified by runAction.
 */
class CC_DLL TargetedAction : public ActionInterval
{
public:
    virtual ~TargetedAction();
    /** Create an action with the specified action and forced target.
     * 
     * @param target The target needs to override.
     * @param action The action needs to override.
     * @return An autoreleased TargetedAction object.
     */
    static std::unique_ptr<TargetedAction> create(Node* target, std::unique_ptr<FiniteTimeAction> action);

    /** Sets the target that the action will be forced to run with.
     *
     * @param forcedTarget The target that the action will be forced to run with.
     */
    void setForcedTarget(Node* forcedTarget);
    /** returns the target that the action is forced to run with. 
     *
     * @return The target that the action is forced to run with.
     */
    Node* getForcedTarget() { return _forcedTarget; }
    const Node* getForcedTarget() const { return _forcedTarget; }

    //
    // Overrides
    //
    virtual TargetedAction* clone() const override;
    virtual TargetedAction* reverse() const  override;
    virtual void startWithTarget(Node *target) override;
    /**
     * @param time In seconds.
     */
    virtual void step(float time) override;
    
protected:
    TargetedAction();

    /** Init an action with the specified action and forced target */
    bool initWithTarget(Node* target, std::unique_ptr<FiniteTimeAction> action);

    virtual void at_stop() override;

protected:
    std::unique_ptr<FiniteTimeAction> _action;
    Node* _forcedTarget;

private:
    TargetedAction(const TargetedAction &) = delete;
    const TargetedAction & operator=(const TargetedAction &) = delete;
};

/**
 * @class ActionFloat
 * @brief Action used to animate any value in range [from,to] over specified time interval
 */
class CC_DLL ActionFloat : public ActionInterval
{
public:
    virtual ~ActionFloat() {};
    /**
     *  Callback function used to report back result
     */
    typedef std::function<void(float value)> ActionFloatCallback;

    /**
     * Creates FloatAction with specified duration, from value, to value and callback to report back
     * results
     * @param duration of the action
     * @param from value to start from
     * @param to value to be at the end of the action
     * @param callback to report back result
     *
     * @return An autoreleased ActionFloat object
     */
    static std::unique_ptr<ActionFloat> create(float duration, float from, float to, ActionFloatCallback callback);

    /**
     * Overridden ActionInterval methods
     */
    void startWithTarget(Node* target) override;
    void step(float delta) override;
    ActionFloat* reverse() const override;
    virtual ActionFloat* clone() const override;

protected:
    ActionFloat() {};

    bool initWithDuration(float duration, float from, float to, ActionFloatCallback callback);

    virtual void at_stop() override;

protected:
    /* From value */
    float _from;
    /* To value */
    float _to;
    /* delta time */
    float _delta;

    /* Callback to report back results */
    ActionFloatCallback _callback;
private:
    ActionFloat(const ActionFloat &) = delete;
    const ActionFloat & operator=(const ActionFloat &) = delete;
};

} // namespace cocos2d

#endif //__ACTION_CCINTERVAL_ACTION_H__

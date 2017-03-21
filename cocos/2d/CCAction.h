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

#ifndef __ACTIONS_CCACTION_H__
#define __ACTIONS_CCACTION_H__

#include "2d/CCNode.h"
#include "math/CCGeometry.h"

namespace cocos2d {

class Node;

enum {
    kActionUpdate
};

/** 
 * @brief Base class for Action objects.
 */
class CC_DLL Action
{
public:

    using tag_t   = int32_t;
    using flags_t = uint32_t;

public:

    virtual ~Action();

    /** Default tag used for all the actions. */
    static constexpr tag_t INVALID_TAG = -1;

    /** Called before the action start. It will also set the target. 
     *
     * @param target A certain target.
     */
    virtual void startWithTarget(Node *target);

    virtual Action* clone() const = 0;

    virtual Action* reverse() const = 0;

    virtual std::string description() const;

public:

    bool last_update(float dt)
    {
        update(dt);

        if (isDone())
        {
            stop();
            return true;
        }

        return false;
    }

    /** Called every frame with it's delta time, dt in seconds. DON'T override unless you know what you are doing. 
     *
     * @param dt In seconds.
     */
    virtual void update(float dt) = 0;

    /** 
     * Called once per frame. time a value between 0 and 1.

     * For example:
     * - 0 Means that the action just started.
     * - 0.5 Means that the action is in the middle.
     * - 1 Means that the action is over.
     *
     * @param time A value between 0 and 1.
     */
    virtual void step(float time) = 0;

    virtual bool isDone() const = 0;

    void stop();

    bool hasStopped() const { return _hasStopped; }

protected:

    virtual void at_stop() = 0;

public:

    Node* getTarget() const { return _target; }

    tag_t getTag() const    { return _tag; }
    void  setTag(tag_t tag) { _tag = tag; }

    flags_t getFlags() const        { return _flags; }
    void    setFlags(flags_t flags) { _flags = flags; }

protected:

    Action();

private:
    Node*   _target;
    tag_t   _tag;
    flags_t _flags;
    bool    _hasStopped;

private:
    Action(const Action &) = delete;
    const Action & operator=(const Action &) = delete;
};

/** @class FiniteTimeAction
 * @brief
 * Base class actions that do have a finite time duration.
 * Possible actions:
 * - An action with a duration of 0 seconds.
 * - An action with a duration of 35.5 seconds.
 * Infinite time actions are valid.
 */
class CC_DLL FiniteTimeAction : public Action
{
public:
    virtual ~FiniteTimeAction(){}

    /** Get duration in seconds of the action. 
     *
     * @return The duration in seconds of the action.
     */
    float getDuration() const { return _duration; }
    /** Set duration in seconds of the action. 
     *
     * @param duration In seconds of the action.
     */
    void setDuration(float duration) { _duration = duration; }

    virtual FiniteTimeAction* reverse() const override = 0;

    virtual FiniteTimeAction* clone() const override = 0;

protected:
    FiniteTimeAction()
    : _duration(0)
    {}

protected:
    //! Duration in seconds.
    float _duration;
private:
    FiniteTimeAction(const FiniteTimeAction &) = delete;
    const FiniteTimeAction & operator=(const FiniteTimeAction &) = delete;
};

class ActionInterval;
class RepeatForever;

// Follow is an action that "follows" a node.
// Instead of using Camera as a "follower", use this action instead.
// For example:
// layer->runAction(std::make_unique<Follow>(hero));

class CC_DLL Follow final : public Action
{
public:
    Follow(Node *followedNode, const Rect& boundary = Rect::ZERO)
        : Follow(followedNode, 0.0f, 0.0f, boundary)
        {}
    
    /**
     * Creates the action with a set boundary or with no boundary with offsets.
     *
     * @param followedNode  The node to be followed.
     * @param rect  The boundary. If \p rect is equal to Rect::ZERO, it'll work
     *              with no boundary.
     * @param xOffset The horizontal offset from the center of the screen from which the
     *               node  is to be followed.It can be positive,negative or zero.If
     *               set to zero the node will be horizontally centered followed.
     *  @param yOffset The vertical offset from the center of the screen from which the
     *                 node is to be followed.It can be positive,negative or zero.
     *                 If set to zero the node will be vertically centered followed.
     *   If both xOffset and yOffset are set to zero,then the node will be horizontally and vertically centered followed.
     */

    Follow(Node* followedNode,float xOffset,float yOffset,const Rect& boundary = Rect::ZERO);
    
    /** Return boundarySet.
     *
     * @return Return boundarySet.
     */
    bool isBoundarySet() const { return _boundarySet; }
    /** Alter behavior - turn on/off boundary. 
     *
     * @param value Turn on/off boundary.
     */
    void setBoundarySet(bool value) { _boundarySet = value; }
    
    //
    // Override
    //
    virtual Follow* clone() const override;
    virtual Follow* reverse() const override;
    /**
     * @param dt in seconds.
     */
    virtual void update(float dt) override;
    virtual bool isDone() const override;

    virtual void step(float time) override;
    virtual void at_stop() override;

protected:

    /**
     * Initializes the action with a set boundary or with no boundary with offsets.
     *
     * @param followedNode  The node to be followed.
     * @param rect  The boundary. If \p rect is equal to Rect::ZERO, it'll work
     *              with no boundary.
     * @param xOffset The horizontal offset from the center of the screen from which the
     *                node  is to be followed.It can be positive,negative or zero.If
     *                set to zero the node will be horizontally centered followed.
     * @param yOffset The vertical offset from the center of the screen from which the
     *                node is to be followed.It can be positive,negative or zero.
     *                If set to zero the node will be vertically centered followed.
     *   If both xOffset and yOffset are set to zero,then the node will be horizontally and vertically centered followed.

     */
    bool initWithTargetAndOffset(Node *followedNode,float xOffset,float yOffset,const Rect& rect = Rect::ZERO);

protected:
    /** Node to follow. */
    node_ptr<Node> _followedNode;

    /** Whether camera should be limited to certain area. */
    bool _boundarySet;

    /** If screen size is bigger than the boundary - update not needed. */
    bool _boundaryFullyCovered;

    /** Fast access to the screen dimensions. */
    Vec2 _halfScreenSize;
    Vec2 _fullScreenSize;

    /** World boundaries. */
    float _leftBoundary;
    float _rightBoundary;
    float _topBoundary;
    float _bottomBoundary;
    
    /** Horizontal (x) and vertical (y) offset values. */
    float _offsetX;
    float _offsetY;
    
    Rect _worldRect;

private:
    Follow(const Follow &) = delete;
    const Follow & operator=(const Follow &) = delete;
};

} // namespace cocos2d

#endif // __ACTIONS_CCACTION_H__

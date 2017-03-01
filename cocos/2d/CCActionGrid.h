/****************************************************************************
Copyright (c) 2009      On-Core
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
#ifndef __ACTION_CCGRID_ACTION_H__
#define __ACTION_CCGRID_ACTION_H__

#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"

namespace cocos2d {

class GridBase;
class NodeGrid;

// Grid actions are the actions take effect on GridBase.

class CC_DLL GridAction : public ActionInterval
{
public:
    GridAction(float duration, Size gridSize);

    virtual GridBase* getGrid();

    virtual GridAction* reverse() const override;
    virtual void startWithTarget(Node *target) override;

protected:
    virtual void at_stop() override;
    
    void cacheTargetAsGridNode();

protected:
    Size _gridSize;
    NodeGrid* _gridNodeTarget;
};

// Grid3D actions can modify a non-tiled grid.

class CC_DLL Grid3DAction : public GridAction
{
public:
    Grid3DAction(float duration, Size gridSize)
        : GridAction(duration, std::move(gridSize))
    {}

    virtual GridBase* getGrid() override;

    // The vertex will be used on the certain position of grid.
    void setVertex(const Vec2& position, const Vec3& vertex);
    // Get the vertex that belongs to certain position in the grid.
    Vec3 getVertex(const Vec2& position) const;

    // Get the non-transformed vertex that belongs to certain position in the grid.
    Vec3 getOriginalVertex(const Vec2& position) const;

    // Return the effect grid rect.
    Rect getGridRect() const;
};

class CC_DLL TiledGrid3DAction : public GridAction
{
public:
    TiledGrid3DAction(float duration, Size gridSize)
        : GridAction(duration, std::move(gridSize))
    {}

    virtual GridBase* getGrid() override;

    // Get the tile that belongs to a certain position of the grid.
    Quad3 getTile(const Vec2& position) const;
    void setTile(const Vec2& position, const Quad3& coords);

    // Get the non-transformed tile that belongs to a certain position of the grid.
    Quad3 getOriginalTile(const Vec2& position) const;
};

/**
@brief AccelDeccelAmplitude action.
*/
class CC_DLL AccelDeccelAmplitude : public ActionInterval
{
public:
    // Create the action with an inner action that has the amplitude property, and a duration time.
    AccelDeccelAmplitude(std::unique_ptr<ActionInterval> action, float duration);

    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    virtual AccelDeccelAmplitude* clone() const override;
    virtual AccelDeccelAmplitude* reverse() const override;
    
protected:

    virtual void at_stop() override;

protected:
    std::unique_ptr<ActionInterval> _other;
    float _rate;
};

// AccelAmplitude action.

class CC_DLL AccelAmplitude : public ActionInterval
{
public:
    // Create the action with an inner action that has the amplitude property, and a duration time.
    AccelAmplitude(std::unique_ptr<ActionInterval> action, float duration);

    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    virtual AccelAmplitude* clone() const override;
    virtual AccelAmplitude* reverse() const override;
    
protected:

    virtual void at_stop() override;

protected:
    std::unique_ptr<ActionInterval> _other;
    float _rate;
};

// DeccelAmplitude action.

class CC_DLL DeccelAmplitude : public ActionInterval
{
public:
    // Creates the action with an inner action that has the amplitude property, and a duration time.
    DeccelAmplitude(std::unique_ptr<ActionInterval> action, float duration);

    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    virtual DeccelAmplitude* clone() const override;
    virtual DeccelAmplitude* reverse() const override;
    
protected:

    virtual void at_stop() override;

protected:
    std::unique_ptr<ActionInterval> _other;
    float _rate;
};

 // Don't call this action if another grid action is active.
 // Call if you want to remove the grid effect. Example:
 //   Sequence::create(Lens3D::create(...), StopGrid::create(), nullptr);
class CC_DLL StopGrid : public ActionInstant
{
public:
    /** 
    @brief Create a StopGrid Action.
    @return Return a pointer of StopGrid. When the creation failed, return nil.
    */
    virtual void startWithTarget(Node *target) override;
    virtual void step(float) override { /*nothing*/ }
    virtual StopGrid* clone() const override;
    virtual StopGrid* reverse() const override;
    
protected:
    void cacheTargetAsGridNode();

protected:
    NodeGrid* _gridNodeTarget;
};

// ReuseGrid action.

class CC_DLL ReuseGrid : public ActionInstant
{
public:
    // Create an action with the number of times that the current grid will be reused.
    // times Specify times the grid will be reused.
    explicit ReuseGrid(int times);

    // Override
    virtual void startWithTarget(Node *target) override;
    virtual void step(float) override { /*nothing*/ }
    virtual ReuseGrid* clone() const override;
    virtual ReuseGrid* reverse() const override;

protected:
    NodeGrid* _gridNodeTarget;
    
    void cacheTargetAsGridNode();
    
    int _times;
};

} // namespace cocos2d

#endif // __ACTION_CCGRID_ACTION_H__

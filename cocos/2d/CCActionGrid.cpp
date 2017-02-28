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

#include "2d/CCActionGrid.h"

#include "2d/CCGrid.h"
#include "2d/CCNodeGrid.h"
#include "base/CCDirector.h"

namespace cocos2d {
// implementation of GridAction

GridAction::GridAction(float duration, Size gridSize)
    : _gridSize( gridSize )
{
    ActionInterval::initWithDuration(duration);
}

void GridAction::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    cacheTargetAsGridNode();

    GridBase *targetGrid = _gridNodeTarget->getGrid();

    if (targetGrid && targetGrid->getReuseGrid() > 0)
    {
        if (targetGrid->isActive() && targetGrid->getGridSize().width == _gridSize.width
            && targetGrid->getGridSize().height == _gridSize.height)
        {
            targetGrid->reuse();
        }
        else
        {
            CCASSERT(0, "Invalid grid parameters!");
        }
    }
    else
    {
        if (targetGrid && targetGrid->isActive())
        {
            targetGrid->setActive(false);
        }

        auto newgrid = this->getGrid();
        _gridNodeTarget->setGrid(newgrid);
        _gridNodeTarget->getGrid()->setActive(true);
    }
}

void GridAction::cacheTargetAsGridNode()
{
    _gridNodeTarget = dynamic_cast<NodeGrid*> (getTarget());
    CCASSERT(_gridNodeTarget, "GridActions can only used on NodeGrid");
}

GridAction* GridAction::reverse() const
{
    // FIXME: This conversion isn't safe.
    return (GridAction*)ReverseTime::create(std::unique_ptr<FiniteTimeAction>(clone())).release();
}

GridBase* GridAction::getGrid()
{
    CCASSERT(0, "Subclass should implement this method!");
    return nullptr;
}

void GridAction::at_stop()
{
}

// implementation of Grid3DAction

GridBase* Grid3DAction::getGrid()
{
    return Grid3D::create(_gridSize, _gridNodeTarget->getGridRect());
}

void Grid3DAction::setVertex(const Vec2& position, const Vec3& vertex)
{
    Grid3D *g = (Grid3D*)_gridNodeTarget->getGrid();
    g->setVertex(position, vertex);
}

Vec3 Grid3DAction::getVertex(const Vec2& position) const
{
    Grid3D *g = (Grid3D*)_gridNodeTarget->getGrid();
    return g->getVertex(position);
}

Vec3 Grid3DAction::getOriginalVertex(const Vec2& position) const
{
    Grid3D *g = (Grid3D*)_gridNodeTarget->getGrid();
    return g->getOriginalVertex(position);
}

Rect Grid3DAction::getGridRect() const
{
    Grid3D *g = (Grid3D*)_gridNodeTarget->getGrid();
    return g->getGridRect();
}

// implementation of TiledGrid3DAction

GridBase* TiledGrid3DAction::getGrid(void)
{
    return TiledGrid3D::create(_gridSize, _gridNodeTarget->getGridRect());
}

Quad3 TiledGrid3DAction::getTile(const Vec2& pos) const
{
    TiledGrid3D *g = (TiledGrid3D*)_gridNodeTarget->getGrid();
    return g->getTile(pos);
}

Quad3 TiledGrid3DAction::getOriginalTile(const Vec2& pos) const
{
    TiledGrid3D *g = (TiledGrid3D*)_gridNodeTarget->getGrid();
    return g->getOriginalTile(pos);
}

void TiledGrid3DAction::setTile(const Vec2& pos, const Quad3& coords)
{
    TiledGrid3D *g = (TiledGrid3D*)_gridNodeTarget->getGrid();
    return g->setTile(pos, coords);
}

// implementation AccelDeccelAmplitude

AccelDeccelAmplitude::AccelDeccelAmplitude(std::unique_ptr<ActionInterval> action, float duration)
    : _other( std::move( action))
    , _rate( 1.0f )
{
    CC_ASSERT(action);
    ActionInterval::initWithDuration(duration);
}

AccelDeccelAmplitude* AccelDeccelAmplitude::clone() const
{
    return new AccelDeccelAmplitude(std::unique_ptr<ActionInterval>(_other->clone()), _rate);
}

void AccelDeccelAmplitude::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _other->startWithTarget(target);
}

void AccelDeccelAmplitude::step(float time)
{
    float f = time * 2;

    if (f > 1)
    {
        f -= 1;
        f = 1 - f;
    }

    ((AccelDeccelAmplitude*)(_other.get()))->setAmplitudeRate(powf(f, _rate));
}

AccelDeccelAmplitude* AccelDeccelAmplitude::reverse() const
{
    return new AccelDeccelAmplitude(std::unique_ptr<ActionInterval>(_other->reverse()), _duration);
}

void AccelDeccelAmplitude::at_stop()
{
}

// implementation of AccelAmplitude

AccelAmplitude::AccelAmplitude(std::unique_ptr<ActionInterval> action, float duration)
    : _other( std::move( action))
    , _rate( 1.0f )
{
    CC_ASSERT(action);
    ActionInterval::initWithDuration(duration);
}

AccelAmplitude* AccelAmplitude::clone() const
{
    return new AccelAmplitude(std::unique_ptr<ActionInterval>(_other->clone()), _duration);
}

void AccelAmplitude::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _other->startWithTarget(target);
}

void AccelAmplitude::step(float time)
{
    ((AccelAmplitude*)(_other.get()))->setAmplitudeRate(powf(time, _rate));
    _other->step(time);
}

AccelAmplitude* AccelAmplitude::reverse() const
{
    return new AccelAmplitude(std::unique_ptr<ActionInterval>(_other->reverse()), _duration);
}

void AccelAmplitude::at_stop()
{
}

// DeccelAmplitude

DeccelAmplitude::DeccelAmplitude(std::unique_ptr<ActionInterval> action, float duration)
    : _other( std::move( action))
    , _rate( 1.0f )
{
    CC_ASSERT(action);
    ActionInterval::initWithDuration(duration);
}

void DeccelAmplitude::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _other->startWithTarget(target);
}

void DeccelAmplitude::step(float time)
{
    ((DeccelAmplitude*)(_other.get()))->setAmplitudeRate(powf((1 - time), _rate));
    _other->step(time);
}

DeccelAmplitude* DeccelAmplitude::clone() const
{
    return new DeccelAmplitude(std::unique_ptr<ActionInterval>(_other->clone()), _duration);
}

DeccelAmplitude* DeccelAmplitude::reverse() const
{
    return new DeccelAmplitude(std::unique_ptr<ActionInterval>(_other->reverse()), _duration);
}

void DeccelAmplitude::at_stop()
{
}

// implementation of StopGrid

void StopGrid::startWithTarget(Node *target)
{
    ActionInstant::startWithTarget(target);
    cacheTargetAsGridNode();
    GridBase *grid = _gridNodeTarget->getGrid();
    if (grid && grid->isActive())
    {
        grid->setActive(false);
    }
}

void StopGrid::cacheTargetAsGridNode()
{
    _gridNodeTarget = dynamic_cast<NodeGrid*> (getTarget());
    CCASSERT(_gridNodeTarget, "GridActions can only used on NodeGrid");
}

StopGrid* StopGrid::clone() const
{
    return new StopGrid;
}

StopGrid* StopGrid::reverse() const
{
    // no reverse, just clone it
    return clone();
}

// implementation of ReuseGrid

ReuseGrid::ReuseGrid(int times)
    : _times(times)
{
}

void ReuseGrid::startWithTarget(Node *target)
{
    ActionInstant::startWithTarget(target);
    cacheTargetAsGridNode();

    if (_gridNodeTarget->getGrid() && _gridNodeTarget->getGrid()->isActive())
    {
        _gridNodeTarget->getGrid()->setReuseGrid(_gridNodeTarget->getGrid()->getReuseGrid() + _times);
    }
}

void ReuseGrid::cacheTargetAsGridNode()
{
    _gridNodeTarget = dynamic_cast<NodeGrid*> (getTarget());
    CCASSERT(_gridNodeTarget, "GridActions can only used on NodeGrid");
}

ReuseGrid* ReuseGrid::clone() const
{
    return new ReuseGrid(_times);
}

ReuseGrid* ReuseGrid::reverse() const
{
    // no reverse, just clone it
    return clone();
}

} // namespace cocos2d

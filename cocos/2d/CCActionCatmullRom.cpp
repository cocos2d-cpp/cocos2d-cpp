/*
 * Copyright (c) 2008 Radu Gruian
 * Copyright (c) 2011 Vit Valentin
 * Copyright (c) 2012 cocos2d-x.org
 * Copyright (c) 2013-2016 Chukong Technologies Inc.
 * Copyright (c) 2017      Iakov Sergeev <yahont@github>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *
 * Original code by Radu Gruian: http://www.codeproject.com/Articles/30838/Overhauser-Catmull-Rom-Splines-for-Camera-Animatio.So
 *
 * Adapted to cocos2d-x by Vit Valentin
 *
 * Adapted from cocos2d-x to cocos2d-iphone by Ricardo Quesada
 */
#include "base/ccMacros.h"
#include "2d/CCActionCatmullRom.h"
#include "2d/CCNode.h"

namespace cocos2d {

static std::vector<Vec2> reverseForToActions(std::vector<Vec2> points)
{
    std::reverse(points.begin(), points.end());
    return points;
}

static std::vector<Vec2> reverseForByActions(std::vector<Vec2> points)
{
    //
    // convert "absolutes" to "diffs"
    //
    Vec2 p = points.at(0);
    for (size_t i = 1; i < points.size(); ++i)
    {
        Vec2 current = points.at(i);
        points.at(i) = current - p;
        p = current;
    }

    // convert to "diffs" to "reverse absolute"

    std::reverse(points.begin(), points.end());

    // 1st element (which should be 0,0) should be here too
    
    p = -points.back();
    points.erase(points.begin() + (points.size() - 1));
    
    points.insert(points.begin(), p);
    
    for (size_t i = 1; i < points.size(); ++i)
    {
        Vec2 abs = (-points[i]) + p;
        points[i] = abs;
        p = abs;
    }

    return points;
}

// CatmullRom Spline formula:
Vec2 ccCardinalSplineAt(Vec2 const& p0, Vec2 const& p1, Vec2 const& p2, Vec2 const& p3, float tension, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;
    
    /*
     * Formula: s(-ttt + 2tt - t)P1 + s(-ttt + tt)P2 + (2ttt - 3tt + 1)P2 + s(ttt - 2tt + t)P3 + (-2ttt + 3tt)P3 + s(ttt - tt)P4
     */
    float s = (1 - tension) / 2;

    float b1 = s * ((-t3 + (2 * t2)) - t);                 // s(-t3 + 2 t2 - t)P1
    float b2 = s * (-t3 + t2) + (2 * t3 - 3 * t2 + 1);     // s(-t3 + t2)P2 + (2 t3 - 3 t2 + 1)P2
    float b3 = s * (t3 - 2 * t2 + t) + (-2 * t3 + 3 * t2); // s(t3 - 2 t2 + t)P3 + (-2 t3 + 3 t2)P3
    float b4 = s * (t3 - t2);                              // s(t3 - t2)P4
    
    float x = (p0.x * b1 + p1.x * b2 + p2.x * b3 + p3.x * b4);
    float y = (p0.y * b1 + p1.y * b2 + p2.y * b3 + p3.y * b4);

    return Vec2(x,y);
}

/* Implementation of CardinalSplineTo
 */

CardinalSplineTo::CardinalSplineTo(float duration, std::vector<Vec2> points, float tension)
    : _points( std::move( points))
    , _deltaT(0.0f)
    , _tension(tension)
{
    ActionInterval::initWithDuration(duration);
}

void CardinalSplineTo::startWithTarget(cocos2d::Node *target)
{
    ActionInterval::startWithTarget(target);

    CC_ASSERT(1 < _points.size());
    _deltaT = 1.0f / (_points.size() - 1);

    _previousPosition = target->getPosition();
    _accumulatedDiff.setZero();
}

CardinalSplineTo* CardinalSplineTo::clone() const
{
    return new CardinalSplineTo(_duration, _points, _tension);
}

void CardinalSplineTo::step(float time)
{
    int p;
    float lt;

    // eg.
    // p..p..p..p..p..p..p
    // 1..2..3..4..5..6..7
    // want p to be 1, 2, 3, 4, 5, 6

    CC_ASSERT(_points.size());

    if (time == 1)
    {
        p = _points.size() - 1;
        lt = 1;
    }
    else 
    {
        p = time / _deltaT;
        lt = (time - _deltaT * (float)p) / _deltaT;
    }

    const auto control_point = [=](int p) -> Vec2 const& {
        if (p < 0)
            return _points[0];
        else if (p < (int)_points.size())
            return _points[p];
        return _points.back();
    };

    // Interpolate
    Vec2 newPos = ccCardinalSplineAt(
        control_point(p - 1),
        control_point(p + 0),
        control_point(p + 1),
        control_point(p + 2),
        _tension,
        lt
    );

#if CC_ENABLE_STACKABLE_ACTIONS
    // Support for stacked actions
    Node *node = getTarget();
    Vec2 diff = node->getPosition() - _previousPosition;
    if( diff.x !=0 || diff.y != 0 )
    {
        _accumulatedDiff = _accumulatedDiff + diff;
        newPos = newPos + _accumulatedDiff;
    }
#endif

    this->updatePosition(newPos);
}

void CardinalSplineTo::updatePosition(cocos2d::Vec2 &newPos)
{
    getTarget()->setPosition(newPos);
    _previousPosition = newPos;
}

CardinalSplineTo* CardinalSplineTo::reverse() const
{
    return new CardinalSplineTo(_duration, reverseForToActions(_points), _tension);
}

void CardinalSplineTo::at_stop()
{
}

/* CardinalSplineBy
 */

CardinalSplineBy::CardinalSplineBy(float duration, std::vector<Vec2> points, float tension)
    : CardinalSplineTo(duration, std::move(points), tension)
{
}

void CardinalSplineBy::startWithTarget(cocos2d::Node *target)
{    
    CardinalSplineTo::startWithTarget(target);
    _startPosition = target->getPosition();
}

void CardinalSplineBy::updatePosition(cocos2d::Vec2 &newPos)
{
    Vec2 p = newPos + _startPosition;
    getTarget()->setPosition(p);
    _previousPosition = p;
}

CardinalSplineBy* CardinalSplineBy::reverse() const
{
    return new CardinalSplineBy(_duration, reverseForByActions(_points), _tension);
}

CardinalSplineBy* CardinalSplineBy::clone() const
{
    return new CardinalSplineBy(_duration, _points, _tension);
}

void CardinalSplineBy::at_stop()
{
}

/* CatmullRomTo
 */

CatmullRomTo::CatmullRomTo(float dt, std::vector<Vec2> points)
    : CardinalSplineTo(dt, points, 0.5f)
{
}

CatmullRomTo* CatmullRomTo::clone() const
{
    return new CatmullRomTo(_duration, _points);
}

CatmullRomTo* CatmullRomTo::reverse() const
{
    return new CatmullRomTo(_duration, reverseForToActions(_points));
}


/* CatmullRomBy
 */

CatmullRomBy::CatmullRomBy(float dt, std::vector<Vec2> points)
    : CardinalSplineBy(dt, points, 0.5f)
{
}

CatmullRomBy* CatmullRomBy::clone() const
{
    return new CatmullRomBy(_duration, _points);
}

CatmullRomBy* CatmullRomBy::reverse() const
{
    return new CatmullRomBy(_duration, reverseForByActions(_points));
}

} // namespace cocos2d

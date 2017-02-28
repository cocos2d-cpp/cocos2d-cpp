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


#ifndef __CCACTION_CATMULLROM_H__
#define __CCACTION_CATMULLROM_H__

#include <vector>

#include "2d/CCActionInterval.h"
#include "math/CCGeometry.h"

namespace cocos2d {

class Node;

// Cardinal Spline path.
// http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Cardinal_spline

class CC_DLL CardinalSplineTo : public ActionInterval
{
public:

    CardinalSplineTo(float duration, std::vector<Vec2> controlPoints, float tension);

    // It will update the target position and change the _previousPosition to newPos
    virtual void updatePosition(Vec2 &newPos);

    std::vector<Vec2> const& getPoints() { return _points; }

    virtual CardinalSplineTo *clone() const override;
    virtual CardinalSplineTo* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;

protected:
    virtual void at_stop() override;

protected:
    /** Array of control points */
    std::vector<Vec2> _points;
    float _deltaT;
    float _tension;
    Vec2 _previousPosition;
    Vec2 _accumulatedDiff;
};

// Cardinal Spline path.
// http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Cardinal_spline

class CC_DLL CardinalSplineBy : public CardinalSplineTo
{
public:

    CardinalSplineBy(float duration, std::vector<Vec2> points, float tension);

    CardinalSplineBy();

    virtual void startWithTarget(Node *target) override;
    virtual void updatePosition(Vec2 &newPos) override;
    virtual CardinalSplineBy *clone() const override;
    virtual CardinalSplineBy* reverse() const override;

protected:
    virtual void at_stop() override;

protected:
    Vec2 _startPosition;
};

// An action that moves the target with a CatmullRom curve to a destination point.
// A Catmull Rom is a Cardinal Spline with a tension of 0.5.
// http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull.E2.80.93Rom_spline

class CC_DLL CatmullRomTo : public CardinalSplineTo
{
public:

    CatmullRomTo(float dt, std::vector<Vec2> points);

    virtual CatmullRomTo *clone() const override;
    virtual CatmullRomTo *reverse() const override;
};

// An action that moves the target with a CatmullRom curve by a certain distance.
// A Catmull Rom is a Cardinal Spline with a tension of 0.5.
// http://en.wikipedia.org/wiki/Cubic_Hermite_spline#Catmull.E2.80.93Rom_spline

class CC_DLL CatmullRomBy : public CardinalSplineBy
{
public:
    CatmullRomBy(float dt, std::vector<Vec2> points);

    virtual CatmullRomBy *clone() const override;
    virtual CatmullRomBy *reverse() const override;
};

// Returns the Cardinal Spline position for a given set of control points, tension and time
Vec2 ccCardinalSplineAt(Vec2 const& p0, Vec2 const& p1, Vec2 const& p2, Vec2 const& p3, float tension, float t);

} // namespace cocos2d

#endif // __CCACTION_CATMULLROM_H__

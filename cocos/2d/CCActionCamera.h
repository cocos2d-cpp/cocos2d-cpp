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

#ifndef __CCCAMERA_ACTION_H__
#define __CCCAMERA_ACTION_H__

#include "2d/CCActionInterval.h"
#include "math/CCMath.h"

namespace cocos2d {

class Camera;

/**
 * @addtogroup actions
 * @{
 */

/**
 *@brief Base class for Camera actions.
 *@ingroup Actions
 */
class CC_DLL ActionCamera : public ActionInterval
{
public:
    ActionCamera();

    // Overrides
    virtual void startWithTarget(Node *target) override;

    virtual ActionCamera *clone() const override = 0;

    void setEye(const Vec3 &eye);
    void setEye(float x, float y, float z);
    const Vec3& getEye() const { return _eye; }

    void setCenter(const Vec3 &center);
    const Vec3& getCenter() const { return _center; }

    void setUp(const Vec3 &up);
    const Vec3& getUp() const { return _up; }

protected:

    void restore();
    void updateTransform();

    Vec3 _center;
    Vec3 _eye;
    Vec3 _up;
};

// Orbits the camera around the center of the screen using spherical coordinates.

class CC_DLL OrbitCamera : public ActionCamera
{
public:
    OrbitCamera(float t, float radius, float deltaRadius, float angleZ, float deltaAngleZ, float angleX, float deltaAngleX);
    
    // Positions the camera according to spherical coordinates. 
    void sphericalRadius(float *radius, float *zenith, float *azimuth);

    // Overrides
    virtual void startWithTarget(Node *target) override;
    virtual ReverseTime* reverse() const override;
    virtual OrbitCamera *clone() const override;
    virtual void step(float time) override;
    
protected:

    virtual void at_stop() override;

protected:
    float _radius;
    float _deltaRadius;
    float _angleZ;
    float _deltaAngleZ;
    float _angleX;
    float _deltaAngleX;

    float _radZ;
    float _radDeltaZ;
    float _radX;
    float _radDeltaX;
};

} // namespace cocos2d

#endif //__CCCAMERA_ACTION_H__

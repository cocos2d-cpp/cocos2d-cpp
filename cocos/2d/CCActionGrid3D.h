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
#ifndef __ACTION_CCGRID3D_ACTION_H__
#define __ACTION_CCGRID3D_ACTION_H__

#include "2d/CCActionGrid.h"

namespace cocos2d {

// You can control the effect by these parameters:
// duration, grid size, waves count, amplitude.

class CC_DLL Waves3D : public Grid3DAction
{
public:
    Waves3D(float duration, Size gridSize, unsigned int waves_count, float amplitude);

    void setAmplitudeRate(float amplitudeRate) { _amplitudeRate = amplitudeRate; }

    virtual Waves3D* clone() const override;
    virtual void step(float time) override;
    
protected:
    unsigned int _waves;
    float _amplitude;
    float _amplitudeRate;
};

// This action is used for flipping the target node on the x axis.

class CC_DLL FlipX3D : public Grid3DAction
{
public:
    explicit FlipX3D(float duration);

    virtual FlipX3D* clone() const override;
    virtual void step(float time) override;
};

// This action is used for flipping the target node on the y axis.

class CC_DLL FlipY3D : public Grid3DAction
{
public:
    explicit FlipY3D(float duration);

    virtual FlipY3D* clone() const override;
    virtual void step(float time) override;
};

class CC_DLL Lens3D : public Grid3DAction
{
public:
    Lens3D(float duration, Size gridSize, Vec2 centre_position, float radius);

    float getLensEffect() const { return _lensEffect; }
    void setLensEffect(float lensEffect) { _lensEffect = lensEffect; }

    void setConcave(bool concave) { _concave = concave; }

    const Vec2& getPosition() const { return _position; }
    void setPosition(const Vec2& position);

    virtual Lens3D* clone() const override;
    virtual void step(float time) override;
    
protected:
    /* lens center position */
    Vec2 _position;
    float _radius;
    /** lens effect. Defaults to 0.7 - 0 means no effect, 1 is very strong effect */
    float _lensEffect;
    /** lens is concave. (true = concave, false = convex) default is convex i.e. false */
    bool _concave;

    bool _dirty;
};

class CC_DLL Ripple3D : public Grid3DAction
{
public:
    Ripple3D(float duration, Size gridSize, Vec2 centre_position, float radius, unsigned int waves_count, float amplitude);

    const Vec2& getPosition() const { return _position; }
    
    void setPosition(const Vec2& position);

    void setAmplitudeRate(float fAmplitudeRate) { _amplitudeRate = fAmplitudeRate; }

    virtual Ripple3D* clone() const override;
    virtual void step(float time) override;

protected:
    /* center position */
    Vec2 _position;
    float _radius;
    unsigned int _waves;
    float _amplitude;
    float _amplitudeRate;
};

class CC_DLL Shaky3D : public Grid3DAction
{
public:
    Shaky3D(float duration, Size gridSize, int range, bool shakeZ);

    virtual Shaky3D* clone() const override;
    virtual void step(float time) override;
    
protected:
    int _randrange;
    bool _shakeZ;
};

class CC_DLL Liquid : public Grid3DAction
{
public:
    Liquid(float duration, Size gridSize, unsigned int waves, float amplitude);

    void setAmplitudeRate(float amplitudeRate) { _amplitudeRate = amplitudeRate; }

    virtual Liquid* clone() const override;
    virtual void step(float time) override;
    
protected:
    unsigned int _waves;
    float _amplitude;
    float _amplitudeRate;
};

class CC_DLL Waves : public Grid3DAction
{
public:
    Waves(float duration, Size gridSize, unsigned int waves, float amplitude, bool horizontal, bool vertical);

    void setAmplitudeRate(float amplitudeRate) { _amplitudeRate = amplitudeRate; }

    virtual Waves* clone() const override;
    virtual void step(float time) override;
    
protected:
    unsigned int _waves;
    float _amplitude;
    float _amplitudeRate;
    bool _vertical;
    bool _horizontal;
};

class CC_DLL Twirl : public Grid3DAction
{
public:
    Twirl(float duration, Size gridSize, const Vec2& position, unsigned int twirls, float amplitude);

    const Vec2& getPosition() const { return _position; }
    void setPosition(const Vec2& position);

    void setAmplitudeRate(float amplitudeRate) { _amplitudeRate = amplitudeRate; }

    virtual Twirl* clone() const override;
    virtual void step(float time) override;
    
protected:
    /* twirl center */
    Vec2 _position;
    unsigned int _twirls;
    float _amplitude;
    float _amplitudeRate;
};

} // namespace cocos2d

#endif // __ACTION_CCGRID3D_ACTION_H__

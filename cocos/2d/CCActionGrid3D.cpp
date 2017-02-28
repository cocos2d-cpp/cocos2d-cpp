/****************************************************************************
Copyright (c) 2009      On-Core
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2016 Chukong Technologies Inc.
 
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

#include "2d/CCActionGrid3D.h"
#include "base/CCDirector.h"

namespace cocos2d {

// implementation of Waves3D

Waves3D::Waves3D(float duration, Size gridSize, unsigned int waves, float amplitude)
    : Grid3DAction(duration, std::move(gridSize))
    , _waves( waves )
    , _amplitude( amplitude )
    , _amplitudeRate( 1.0f )
{}

Waves3D* Waves3D::clone() const
{
    return new Waves3D(_duration, _gridSize, _waves, _amplitude);
}

void Waves3D::step(float time)
{
    int i, j;
    for (i = 0; i < _gridSize.width + 1; ++i)
    {
        for (j = 0; j < _gridSize.height + 1; ++j)
        {
            Vec3 v = getOriginalVertex(Vec2(i ,j));
            v.z += (sinf((float)M_PI * time * _waves * 2 + (v.y+v.x) * 0.01f) * _amplitude * _amplitudeRate);
            setVertex(Vec2(i, j), v);
        }
    }
}

// implementation of FlipX3D

FlipX3D::FlipX3D(float duration)
    : Grid3DAction(duration, Size(1,1))
{
}

FlipX3D* FlipX3D::clone() const
{
    return new FlipX3D(_duration);
}

void FlipX3D::step(float time)
{
    float angle = (float)M_PI * time; // 180 degrees
    float mz = sinf(angle);
    angle = angle / 2.0f; // x calculates degrees from 0 to 90
    float mx = cosf(angle);

    Vec3 v0, v1, v, diff;

    v0 = getOriginalVertex(Vec2(1.0f, 1.0f));
    v1 = getOriginalVertex(Vec2());

    float    x0 = v0.x;
    float    x1 = v1.x;
    float    x;
    Vec2    a, b, c, d;

    if ( x0 > x1 )
    {
        // Normal Grid
        a.setZero();
        b.set(0.0f, 1.0f);
        c.set(1.0f, 0.0f);
        d.set(1.0f, 1.0f);
        x = x0;
    }
    else
    {
        // Reversed Grid
        c.setZero();
        d.set(0.0f, 1.0f);
        a.set(1.0f, 0.0f);
        b.set(1.0f, 1.0f);
        x = x1;
    }
    
    diff.x = ( x - x * mx );
    diff.z = fabsf( floorf( (x * mz) / 4.0f ) );

    // bottom-left
    v = getOriginalVertex(a);
    v.x = diff.x;
    v.z += diff.z;
    setVertex(a, v);
    
    // upper-left
    v = getOriginalVertex(b);
    v.x = diff.x;
    v.z += diff.z;
    setVertex(b, v);
    
    // bottom-right
    v = getOriginalVertex(c);
    v.x -= diff.x;
    v.z -= diff.z;
    setVertex(c, v);
    
    // upper-right
    v = getOriginalVertex(d);
    v.x -= diff.x;
    v.z -= diff.z;
    setVertex(d, v);
}

// implementation of FlipY3D

FlipY3D::FlipY3D(float duration)
    : Grid3DAction(duration, Size(1,1))
{
}

FlipY3D* FlipY3D::clone() const
{
    return new FlipY3D(_duration);
}

void FlipY3D::step(float time)
{
    float angle = (float)M_PI * time; // 180 degrees
    float mz = sinf( angle );
    angle = angle / 2.0f;     // x calculates degrees from 0 to 90
    float my = cosf(angle);
    
    Vec3    v0, v1, v, diff;
    
    v0 = getOriginalVertex(Vec2(1.0f, 1.0f));
    v1 = getOriginalVertex(Vec2());
    
    float    y0 = v0.y;
    float    y1 = v1.y;
    float y;
    Vec2    a, b, c, d;
    
    if (y0 > y1)
    {
        // Normal Grid
        a.setZero();
        b.set(0.0f, 1.0f);
        c.set(1.0f, 0.0f);
        d.set(1.0f, 1.0f);
        y = y0;
    }
    else
    {
        // Reversed Grid
        b.setZero();
        a.set(0.0f, 1.0f);
        d.set(1.0f, 0.0f);
        c.set(1.0f, 1.0f);
        y = y1;
    }
    
    diff.y = y - y * my;
    diff.z = fabsf(floorf((y * mz) / 4.0f));
    
    // bottom-left
    v = getOriginalVertex(a);
    v.y = diff.y;
    v.z += diff.z;
    setVertex(a, v);
    
    // upper-left
    v = getOriginalVertex(b);
    v.y -= diff.y;
    v.z -= diff.z;
    setVertex(b, v);
    
    // bottom-right
    v = getOriginalVertex(c);
    v.y = diff.y;
    v.z += diff.z;
    setVertex(c, v);
    
    // upper-right
    v = getOriginalVertex(d);
    v.y -= diff.y;
    v.z -= diff.z;
    setVertex(d, v);
}


// implementation of Lens3D

Lens3D::Lens3D(float duration, Size gridSize, Vec2 position, float radius)
    : Grid3DAction(duration, std::move(gridSize))
    , _position(position)
    , _radius(radius)
    , _lensEffect(0.7f)
    , _concave( false )
    , _dirty(true)
{
}

Lens3D* Lens3D::clone() const
{
    // no copy constructor
    auto a = new Lens3D(_duration, _gridSize, _position, _radius);
    a->_lensEffect = _lensEffect;
    a->_concave = _concave;
    return a;
}

void Lens3D::setPosition(const Vec2& pos)
{
    _position = pos;
    _dirty = true;
}

void Lens3D::step(float /*time*/)
{
    if (_dirty)
    {
        int i, j;
        
        for (i = 0; i < _gridSize.width + 1; ++i)
        {
            for (j = 0; j < _gridSize.height + 1; ++j)
            {
                Vec3 v = getOriginalVertex(Vec2(i, j));
                Vec2 vect = _position - Vec2(v.x, v.y);
                float r = vect.getLength();
                
                if (r < _radius)
                {
                    r = _radius - r;
                    float pre_log = r / _radius;
                    if ( pre_log == 0 ) 
                    {
                        pre_log = 0.001f;
                    }

                    float l = logf(pre_log) * _lensEffect;
                    float new_r = expf( l ) * _radius;
                    
                    if (vect.getLength() > 0)
                    {
                        vect.normalize();
                        Vec2 new_vect = vect * new_r;
                        v.z += (_concave ? -1.0f : 1.0f) * new_vect.getLength() * _lensEffect;
                    }
                }
                
                setVertex(Vec2(i, j), v);
            }
        }
        
        _dirty = false;
    }
}

// implementation of Ripple3D

Ripple3D::Ripple3D(float duration, Size gridSize, Vec2 position, float radius, unsigned int waves, float amplitude)
    : Grid3DAction(duration, std::move(gridSize))
    , _position(position)
    , _radius(radius)
    , _waves(waves)
    , _amplitude(amplitude)
    , _amplitudeRate(1.0f)
{
}

void Ripple3D::setPosition(const Vec2& position)
{
    _position = position;
}

Ripple3D* Ripple3D::clone() const
{
    auto a = new Ripple3D(_duration, _gridSize, _position, _radius, _waves, _amplitude);
    a->_amplitudeRate = _amplitudeRate;
    return a;
}

void Ripple3D::step(float time)
{
    int i, j;

    for (i = 0; i < (_gridSize.width+1); ++i)
    {
        for (j = 0; j < (_gridSize.height+1); ++j)
        {
            Vec3 v = getOriginalVertex(Vec2(i, j));
            Vec2 vect = _position - Vec2(v.x,v.y);
            float r = vect.getLength();
            
            if (r < _radius)
            {
                r = _radius - r;
                float rate = powf(r / _radius, 2);
                v.z += (sinf( time*(float)M_PI * _waves * 2 + r * 0.1f) * _amplitude * _amplitudeRate * rate);
            }
            
            setVertex(Vec2(i, j), v);
        }
    }
}

// implementation of Shaky3D

Shaky3D::Shaky3D(float duration, Size gridSize, int range, bool shakeZ)
    : Grid3DAction(duration, std::move(gridSize))
    , _randrange( range )
    , _shakeZ( shakeZ )
{
}

Shaky3D* Shaky3D::clone() const
{
    return new Shaky3D(_duration, _gridSize, _randrange, _shakeZ);
}

void Shaky3D::step(float /*time*/)
{
    int i, j;

    for (i = 0; i < (_gridSize.width+1); ++i)
    {
        for (j = 0; j < (_gridSize.height+1); ++j)
        {
            Vec3 v = getOriginalVertex(Vec2(i ,j));
            v.x += (rand() % (_randrange*2)) - _randrange;
            v.y += (rand() % (_randrange*2)) - _randrange;
            if (_shakeZ)
            {
                v.z += (rand() % (_randrange*2)) - _randrange;
            }
            
            setVertex(Vec2(i, j), v);
        }
    }
}

// implementation of Liquid

Liquid::Liquid(float duration, Size gridSize, unsigned int waves, float amplitude)
    : Grid3DAction(duration, std::move(gridSize))
    , _waves( waves )
    , _amplitude( amplitude )
    , _amplitudeRate( 1.0f )
{
}

Liquid* Liquid::clone() const
{
    return new Liquid(_duration, _gridSize, _waves, _amplitude);
}

void Liquid::step(float time)
{
    int i, j;

    for (i = 1; i < _gridSize.width; ++i)
    {
        for (j = 1; j < _gridSize.height; ++j)
        {
            Vec3 v = getOriginalVertex(Vec2(i, j));
            v.x = (v.x + (sinf(time * (float)M_PI * _waves * 2 + v.x * .01f) * _amplitude * _amplitudeRate));
            v.y = (v.y + (sinf(time * (float)M_PI * _waves * 2 + v.y * .01f) * _amplitude * _amplitudeRate));
            setVertex(Vec2(i, j), v);
        }
    }
}

// implementation of Waves

Waves::Waves(float duration, Size gridSize, unsigned int waves, float amplitude, bool horizontal, bool vertical)
    : Grid3DAction(duration, std::move(gridSize))
    , _waves( waves )
    , _amplitude( amplitude )
    , _amplitudeRate( 1.0f )
    , _vertical( vertical )
    , _horizontal( horizontal )
{
}

Waves* Waves::clone() const
{
    return new Waves(_duration, _gridSize, _waves, _amplitude, _horizontal, _vertical);
}

void Waves::step(float time)
{
    int i, j;

    for (i = 0; i < _gridSize.width + 1; ++i)
    {
        for (j = 0; j < _gridSize.height + 1; ++j)
        {
            Vec3 v = getOriginalVertex(Vec2(i, j));

            if (_vertical)
            {
                v.x = (v.x + (sinf(time * (float)M_PI * _waves * 2 + v.y * .01f) * _amplitude * _amplitudeRate));
            }

            if (_horizontal)
            {
                v.y = (v.y + (sinf(time * (float)M_PI * _waves * 2 + v.x * .01f) * _amplitude * _amplitudeRate));
            }

            setVertex(Vec2(i, j), v);
        }
    }
}

// implementation of Twirl

Twirl::Twirl(float duration, Size gridSize, const Vec2& position, unsigned int twirls, float amplitude)
    : Grid3DAction(duration, std::move(gridSize))
    , _position( position )
    , _twirls( twirls )
    , _amplitude( amplitude )
    , _amplitudeRate( 1.0f )
{
}

void Twirl::setPosition(const Vec2& position)
{
    _position = position;
}

Twirl *Twirl::clone() const
{
    auto a = new Twirl(_duration, _gridSize, _position, _twirls, _amplitude);
    a->_amplitudeRate = _amplitudeRate;
    return a;
}

void Twirl::step(float time)
{
    int i, j;
    Vec2 c = _position;
    
    for (i = 0; i < (_gridSize.width+1); ++i)
    {
        for (j = 0; j < (_gridSize.height+1); ++j)
        {
            Vec3 v = getOriginalVertex(Vec2(i ,j));
            
            Vec2 avg(i-(_gridSize.width/2.0f), j-(_gridSize.height/2.0f));
            float r = avg.getLength();
            
            float amp = 0.1f * _amplitude * _amplitudeRate;
            float a = r * cosf( (float)M_PI/2.0f + time * (float)M_PI * _twirls * 2 ) * amp;
            
            Vec2 d(
                sinf(a) * (v.y-c.y) + cosf(a) * (v.x-c.x),
                cosf(a) * (v.y-c.y) - sinf(a) * (v.x-c.x));
            
            v.x = c.x + d.x;
            v.y = c.y + d.y;

            setVertex(Vec2(i ,j), v);
        }
    }
}

} // namespace cocos2d

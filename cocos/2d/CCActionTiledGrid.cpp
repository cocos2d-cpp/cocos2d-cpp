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
#include "2d/CCActionTiledGrid.h"
#include "2d/CCGrid.h"
#include "2d/CCNodeGrid.h"
#include "base/CCDirector.h"
#include "base/ccMacros.h"

namespace cocos2d {

// implementation of ShakyTiles3D

ShakyTiles3D* ShakyTiles3D::clone() const
{
    return new ShakyTiles3D(_duration, _gridSize, _randrange, _shakeZ);
}

void ShakyTiles3D::step(float /*time*/)
{
    int i, j;

    for (i = 0; i < _gridSize.width; ++i)
    {
        for (j = 0; j < _gridSize.height; ++j)
        {
            Quad3 coords = getOriginalTile(Vec2(i, j));

            // X
            coords.bl.x += ( rand() % (_randrange*2) ) - _randrange;
            coords.br.x += ( rand() % (_randrange*2) ) - _randrange;
            coords.tl.x += ( rand() % (_randrange*2) ) - _randrange;
            coords.tr.x += ( rand() % (_randrange*2) ) - _randrange;

            // Y
            coords.bl.y += ( rand() % (_randrange*2) ) - _randrange;
            coords.br.y += ( rand() % (_randrange*2) ) - _randrange;
            coords.tl.y += ( rand() % (_randrange*2) ) - _randrange;
            coords.tr.y += ( rand() % (_randrange*2) ) - _randrange;

            if (_shakeZ)
            {
                coords.bl.z += ( rand() % (_randrange*2) ) - _randrange;
                coords.br.z += ( rand() % (_randrange*2) ) - _randrange;
                coords.tl.z += ( rand() % (_randrange*2) ) - _randrange;
                coords.tr.z += ( rand() % (_randrange*2) ) - _randrange;
            }
                        
            setTile(Vec2(i, j), coords);
        }
    }
}

// implementation of ShatteredTiles3D

ShatteredTiles3D* ShatteredTiles3D::clone() const
{
    return new ShatteredTiles3D(_duration, _gridSize, _randrange, _shatterZ);
}

void ShatteredTiles3D::step(float /*time*/)
{
    int i, j;

    if (_once == false)
    {
        for (i = 0; i < _gridSize.width; ++i)
        {
            for (j = 0; j < _gridSize.height; ++j)
            {
                Quad3 coords = getOriginalTile(Vec2(i ,j));
                
                // X
                coords.bl.x += ( rand() % (_randrange*2) ) - _randrange;
                coords.br.x += ( rand() % (_randrange*2) ) - _randrange;
                coords.tl.x += ( rand() % (_randrange*2) ) - _randrange;
                coords.tr.x += ( rand() % (_randrange*2) ) - _randrange;
                
                // Y
                coords.bl.y += ( rand() % (_randrange*2) ) - _randrange;
                coords.br.y += ( rand() % (_randrange*2) ) - _randrange;
                coords.tl.y += ( rand() % (_randrange*2) ) - _randrange;
                coords.tr.y += ( rand() % (_randrange*2) ) - _randrange;

                if (_shatterZ) 
                {
                    coords.bl.z += ( rand() % (_randrange*2) ) - _randrange;
                    coords.br.z += ( rand() % (_randrange*2) ) - _randrange;                
                    coords.tl.z += ( rand() % (_randrange*2) ) - _randrange;
                    coords.tr.z += ( rand() % (_randrange*2) ) - _randrange;
                }
                
                setTile(Vec2(i, j), coords);
            }
        }
        
        _once = true;
    }
}

// implementation of ShuffleTiles

ShuffleTiles::ShuffleTiles(float duration, Size gridSize, unsigned int random_seed)
    : TiledGrid3DAction(duration, std::move(gridSize))
    , _seed(random_seed)
    , _tilesOrder(_gridSize.width * _gridSize.height, 0u)
    , _tiles( _tilesOrder.size() )
{
    for (size_t i = 0, size = _tilesOrder.size(); i < size; i++)
    {
        _tilesOrder[i] = i;
    }

    // FIXME use std::shuffle
    if (_seed != (unsigned int)-1)
    {
        std::srand(_seed);
    }
    for (int i = _tilesOrder.size() - 1; i >= 0; i--)
    {
        unsigned int j = rand() % (i+1);
        unsigned int v = _tilesOrder[i];
        _tilesOrder[i] = _tilesOrder[j];
        _tilesOrder[j] = v;
    }

    size_t k = 0;

    for (int i = 0; i < _gridSize.width; i++)
    {
        for ( int j = 0; j < _gridSize.height; j++)
        {
            _tiles[k].position.set((float)i, (float)j);
            _tiles[k].startPosition.set((float)i, (float)j);
            _tiles[k].delta = getDelta(Size(i, j));
            k++;
        }
    }
}

ShuffleTiles* ShuffleTiles::clone() const
{
    return new ShuffleTiles(_duration, _gridSize, _seed);
}

Size ShuffleTiles::getDelta(Size pos) const
{
    Vec2    pos2;

    unsigned int idx = pos.width * _gridSize.height + pos.height;

    pos2.x = (float)(_tilesOrder[idx] / (int)_gridSize.height);
    pos2.y = (float)(_tilesOrder[idx] % (int)_gridSize.height);

    return Size((int)(pos2.x - pos.width), (int)(pos2.y - pos.height));
}

void ShuffleTiles::step(float time)
{
    size_t k = 0;

    for (int i = 0; i < _gridSize.width; i++)
    {
        for (int j = 0; j < _gridSize.height; j++)
        {
            _tiles[k].position = Vec2((float)_tiles[k].delta.width, (float)_tiles[k].delta.height) * time;
            placeTile(Vec2(i, j), _tiles[k]);
            ++k;
        }
    }
}

void ShuffleTiles::placeTile(const Vec2& pos, Tile const& t)
{
    Quad3 coords = getOriginalTile(pos);

    Vec2 step = _gridNodeTarget->getGrid()->getStep();
    coords.bl.x += (int)(t.position.x * step.x);
    coords.bl.y += (int)(t.position.y * step.y);

    coords.br.x += (int)(t.position.x * step.x);
    coords.br.y += (int)(t.position.y * step.y);

    coords.tl.x += (int)(t.position.x * step.x);
    coords.tl.y += (int)(t.position.y * step.y);

    coords.tr.x += (int)(t.position.x * step.x);
    coords.tr.y += (int)(t.position.y * step.y);

    setTile(pos, coords);
}

// implementation of FadeOutTRTiles

FadeOutTRTiles* FadeOutTRTiles::clone() const
{
    return new FadeOutTRTiles(_duration, _gridSize);
}

float FadeOutTRTiles::testFunc(const Size& pos, float time)
{
    Vec2 n = Vec2((float)_gridSize.width, (float)_gridSize.height) * time;
    if ((n.x + n.y) == 0.0f)
    {
        return 1.0f;
    }

    return powf((pos.width + pos.height) / (n.x + n.y), 6);
}

void FadeOutTRTiles::turnOnTile(const Vec2& pos)
{
    setTile(pos, getOriginalTile(pos));
}

void FadeOutTRTiles::turnOffTile(const Vec2& pos)
{
    Quad3 coords;
    memset(&coords, 0, sizeof(Quad3));
    setTile(pos, coords);
}

void FadeOutTRTiles::transformTile(const Vec2& pos, float part)
{
    Quad3 coords = getOriginalTile(pos);
    Vec2 step = _gridNodeTarget->getGrid()->getStep();

    coords.bl.x += (step.x / 2) * (1.0f - part);
    coords.bl.y += (step.y / 2) * (1.0f - part);

    coords.br.x -= (step.x / 2) * (1.0f - part);
    coords.br.y += (step.y / 2) * (1.0f - part);

    coords.tl.x += (step.x / 2) * (1.0f - part);
    coords.tl.y -= (step.y / 2) * (1.0f - part);

    coords.tr.x -= (step.x / 2) * (1.0f - part);
    coords.tr.y -= (step.y / 2) * (1.0f - part);

    setTile(pos, coords);
}

void FadeOutTRTiles::step(float time)
{
    for (int i = 0; i < _gridSize.width; ++i)
    {
        for (int j = 0; j < _gridSize.height; ++j)
        {
            float part = testFunc(Size(i, j), time);
            if ( part == 0 )
            {
                turnOffTile(Vec2(i, j));
            } else 
            if (part < 1)
            {
                transformTile(Vec2(i, j), part);
            }
            else
            {
                turnOnTile(Vec2(i, j));
            }
        }
    }
}

// implementation of FadeOutBLTiles

FadeOutBLTiles* FadeOutBLTiles::clone() const
{
    return new FadeOutBLTiles(_duration, _gridSize);
}

float FadeOutBLTiles::testFunc(const Size& pos, float time)
{
    Vec2 n = Vec2((float)_gridSize.width, (float)_gridSize.height) * (1.0f - time);
    if ((pos.width + pos.height) == 0)
    {
        return 1.0f;
    }

    return powf((n.x + n.y) / (pos.width + pos.height), 6);
}

// implementation of FadeOutUpTiles

FadeOutUpTiles* FadeOutUpTiles::clone() const
{
    return new FadeOutUpTiles(_duration, _gridSize);
}

float FadeOutUpTiles::testFunc(const Size& pos, float time)
{
    Vec2 n = Vec2((float)_gridSize.width, (float)_gridSize.height) * time;
    if (n.y == 0.0f)
    {
        return 1.0f;
    }

    return powf(pos.height / n.y, 6);
}

void FadeOutUpTiles::transformTile(const Vec2& pos, float distance)
{
    Quad3 coords = getOriginalTile(pos);
    Vec2 step = _gridNodeTarget->getGrid()->getStep();

    coords.bl.y += (step.y / 2) * (1.0f - distance);
    coords.br.y += (step.y / 2) * (1.0f - distance);
    coords.tl.y -= (step.y / 2) * (1.0f - distance);
    coords.tr.y -= (step.y / 2) * (1.0f - distance);

    setTile(pos, coords);
}

// implementation of FadeOutDownTiles

FadeOutDownTiles* FadeOutDownTiles::clone() const
{
    return new FadeOutDownTiles(_duration, _gridSize);
}

float FadeOutDownTiles::testFunc(const Size& pos, float time)
{
    Vec2 n = Vec2((float)_gridSize.width, (float)_gridSize.height) * (1.0f - time);
    return powf(n.y / (pos.height > 0.0f ? pos.height : 0.1f), 6);
}

// implementation of TurnOffTiles

TurnOffTiles::TurnOffTiles(float duration, const Size& gridSize, unsigned int seed)
    : TiledGrid3DAction(duration, std::move(gridSize))
    , _seed( seed )
    , _tilesOrder(_gridSize.width * _gridSize.height, 0)
{
    // FIXME use std::shuffle
    if (_seed != (unsigned int)-1)
    {
        std::srand(_seed);
    }
    for (size_t i = 0, size = _tilesOrder.size(); i < size; i++)
    {
        _tilesOrder[i] = i;
    }
    for (int i = _tilesOrder.size() - 1; i >= 0; i--)
    {
        unsigned int j = rand() % (i+1);
        unsigned int v = _tilesOrder[i];
        _tilesOrder[i] = _tilesOrder[j];
        _tilesOrder[j] = v;
    }
}

TurnOffTiles* TurnOffTiles::clone() const
{
    return new TurnOffTiles(_duration, _gridSize, _seed);
}

void TurnOffTiles::turnOnTile(const Vec2& pos)
{
    setTile(pos, getOriginalTile(pos));
}

void TurnOffTiles::turnOffTile(const Vec2& pos)
{
    Quad3 coords;
    memset(&coords, 0, sizeof(Quad3));
    setTile(pos, coords);
}

void TurnOffTiles::step(float time)
{
    unsigned int l = (unsigned int)(time * (float)_tilesOrder.size());

    size_t t = 0;

    for (size_t i = 0, size = _tilesOrder.size(); i < size; i++ )
    {
        t = _tilesOrder[i];
        Vec2 tilePos((size_t)(t / _gridSize.height), t % (size_t)_gridSize.height);

        if ( i < l )
        {
            turnOffTile(tilePos);
        }
        else
        {
            turnOnTile(tilePos);
        }
    }
}

// implementation of WavesTiles3D

WavesTiles3D::WavesTiles3D(float duration, Size gridSize, unsigned int waves, float amplitude)
    : TiledGrid3DAction(duration, std::move(gridSize))
    , _waves( waves )
    , _amplitude( amplitude )
    , _amplitudeRate( 1.0f )

{
}

WavesTiles3D* WavesTiles3D::clone() const
{
    return new WavesTiles3D(_duration, _gridSize, _waves, _amplitude);
}

void WavesTiles3D::step(float time)
{
    for (int i = 0; i < _gridSize.width; i++ )
    {
        for (int j = 0; j < _gridSize.height; j++ )
        {
            Quad3 coords = getOriginalTile(Vec2(i, j));

            coords.bl.z = (sinf(time * (float)M_PI  *_waves * 2 + 
                (coords.bl.y+coords.bl.x) * .01f) * _amplitude * _amplitudeRate );
            coords.br.z    = coords.bl.z;
            coords.tl.z = coords.bl.z;
            coords.tr.z = coords.bl.z;

            setTile(Vec2(i, j), coords);
        }
    }
}

// implementation of JumpTiles3D

JumpTiles3D* JumpTiles3D::clone() const
{
    // no copy constructor
    return new JumpTiles3D(_duration, _gridSize, _jumps, _amplitude);
}

void JumpTiles3D::step(float time)
{
    float sinz =  (sinf((float)M_PI * time * _jumps * 2) * _amplitude * _amplitudeRate );
    float sinz2 = (sinf((float)M_PI * (time * _jumps * 2 + 1)) * _amplitude * _amplitudeRate );

    for (int i = 0; i < _gridSize.width; i++ )
    {
        for (int j = 0; j < _gridSize.height; j++ )
        {
            Quad3 coords = getOriginalTile(Vec2(i, j));

            if ( ((i+j) % 2) == 0 )
            {
                coords.bl.z += sinz;
                coords.br.z += sinz;
                coords.tl.z += sinz;
                coords.tr.z += sinz;
            }
            else
            {
                coords.bl.z += sinz2;
                coords.br.z += sinz2;
                coords.tl.z += sinz2;
                coords.tr.z += sinz2;
            }

            setTile(Vec2(i, j), coords);
        }
    }
}

// implementation of SplitRows

SplitRows* SplitRows::clone() const
{
    return new SplitRows(_duration, _rows);
}

void SplitRows::startWithTarget(Node *target)
{
    TiledGrid3DAction::startWithTarget(target);
    _winSize = Director::getInstance()->getWinSizeInPixels();
}

void SplitRows::step(float time)
{
    for (unsigned int j = 0; j < _gridSize.height; ++j)
    {
        Quad3 coords = getOriginalTile(Vec2(0, j));
        float    direction = 1;

        if ( (j % 2 ) == 0 )
        {
            direction = -1;
        }

        coords.bl.x += direction * _winSize.width * time;
        coords.br.x += direction * _winSize.width * time;
        coords.tl.x += direction * _winSize.width * time;
        coords.tr.x += direction * _winSize.width * time;

        setTile(Vec2(0, j), coords);
    }
}

// implementation of SplitCols

SplitCols* SplitCols::clone() const
{
    return new SplitCols(_duration, _cols);
}

void SplitCols::startWithTarget(Node *target)
{
    TiledGrid3DAction::startWithTarget(target);
    _winSize = Director::getInstance()->getWinSizeInPixels();
}

void SplitCols::step(float time)
{
    for (unsigned int i = 0; i < _gridSize.width; ++i)
    {
        Quad3 coords = getOriginalTile(Vec2(i, 0));
        float    direction = 1;

        if ( (i % 2 ) == 0 )
        {
            direction = -1;
        }

        coords.bl.y += direction * _winSize.height * time;
        coords.br.y += direction * _winSize.height * time;
        coords.tl.y += direction * _winSize.height * time;
        coords.tr.y += direction * _winSize.height * time;

        setTile(Vec2(i, 0), coords);
    }
}

} // namespace cocos2d

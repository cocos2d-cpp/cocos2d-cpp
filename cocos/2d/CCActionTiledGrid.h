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
#ifndef __ACTION_CCTILEDGRID_ACTION_H__
#define __ACTION_CCTILEDGRID_ACTION_H__

#include "2d/CCActionGrid.h"

#include <vector>

namespace cocos2d {

struct Tile
{
    Vec2    position;
    Vec2    startPosition;
    Size    delta;
};

// This action makes the target node shake with many tiles.

class CC_DLL ShakyTiles3D : public TiledGrid3DAction
{
public:
    ShakyTiles3D(float duration, Size gridSize, int range, bool shakeZ)
        : TiledGrid3DAction(duration, std::move(gridSize))
        , _randrange( range )
        , _shakeZ( shakeZ )
    {}

    virtual ShakyTiles3D* clone() const override;
    virtual void step(float time) override;
    
protected:
    int _randrange;
    bool _shakeZ;
};

// This action makes the target node shattered with many tiles.

class CC_DLL ShatteredTiles3D : public TiledGrid3DAction
{
public:
    ShatteredTiles3D(float duration, Size gridSize, int range, bool shatterZ)
        : TiledGrid3DAction(duration, std::move(gridSize))
        , _randrange(range)
        , _once( false )
        , _shatterZ( shatterZ )
    {}

    virtual ShatteredTiles3D* clone() const override;
    virtual void step(float time) override;
    
protected:
    int _randrange;
    bool _once;
    bool _shatterZ;
};

// This action makes the target node shuffle with many tiles in random order.

class CC_DLL ShuffleTiles : public TiledGrid3DAction
{
public:
    ShuffleTiles(float duration, Size gridSize, unsigned int random_seed);

    virtual ShuffleTiles* clone() const override;

    Size getDelta(Size pos) const;

    virtual void step(float time) override;

private:

    void placeTile(const Vec2& pos, Tile const& t);
    
protected:
    unsigned int        _seed;
    std::vector<size_t> _tilesOrder;
    std::vector<Tile>   _tiles;
};

// Fades out the target node with many tiles from Bottom-Left to Top-Right.

class CC_DLL FadeOutTRTiles : public TiledGrid3DAction
{
public:
    FadeOutTRTiles(float duration, Size gridSize)
        : TiledGrid3DAction(duration, std::move(gridSize))
    {}

    virtual FadeOutTRTiles* clone() const override;

    // Calculate the percentage a tile should be shown.
    virtual float testFunc(const Size& pos_of_tile, float time);

    // Show the tile at specified position.
    void turnOnTile(const Vec2& pos_of_tile);

    // Hide the tile at specified position.
    void turnOffTile(const Vec2& pos_of_tile);

    // Show part of the tile.
    virtual void transformTile(const Vec2& pos, float part);

    virtual void step(float time) override;
};

/**
@brief FadeOutBLTiles action.
@details Fades out the target node with many tiles from Top-Right to Bottom-Left.
 */
class CC_DLL FadeOutBLTiles : public FadeOutTRTiles
{
public:
    FadeOutBLTiles(float duration, const Size& gridSize)
        : FadeOutTRTiles(duration, std::move(gridSize))
    {}

    virtual float testFunc(const Size& pos, float time) override;
    virtual FadeOutBLTiles* clone() const override;
};

// Fades out the target node with many tiles from bottom to top.

class CC_DLL FadeOutUpTiles : public FadeOutTRTiles
{
public:
    FadeOutUpTiles(float duration, Size gridSize)
        : FadeOutTRTiles(duration, gridSize)
    {}

    virtual FadeOutUpTiles* clone() const override;

    virtual float testFunc(const Size& pos, float time) override;

    virtual void transformTile(const Vec2& pos, float distance) override;
};

// Fades out the target node with many tiles from top to bottom.

class CC_DLL FadeOutDownTiles : public FadeOutUpTiles
{
public:
    FadeOutDownTiles(float duration, Size gridSize)
        : FadeOutUpTiles(duration, std::move(gridSize))
    {}

    virtual FadeOutDownTiles* clone() const override;
    virtual float testFunc(const Size& pos, float time) override;
};

// Turn off the target node with many tiles in random order.

class CC_DLL TurnOffTiles : public TiledGrid3DAction
{
public:
    TurnOffTiles(float duration, const Size& gridSize, unsigned int seed);

    TurnOffTiles(float duration, Size gridSize)
        : TurnOffTiles(duration, std::move(gridSize), 0)
    {}

    virtual TurnOffTiles* clone() const override;

    // Show the tile at specified position.
    void turnOnTile(const Vec2& pos);

    // Hide the tile at specified position.
    void turnOffTile(const Vec2& pos);

    virtual void step(float time) override;
    
protected:
    unsigned int        _seed;
    std::vector<size_t> _tilesOrder;
};

// This action wave the target node with many tiles.

class CC_DLL WavesTiles3D : public TiledGrid3DAction
{
public:
    WavesTiles3D(float duration, Size gridSize, unsigned int waves, float amplitude);

    void setAmplitudeRate(float amplitudeRate) { _amplitudeRate = amplitudeRate; }

    // Override
    virtual WavesTiles3D* clone() const override;
    virtual void step(float time) override;
    
protected:
    unsigned int _waves;
    float _amplitude;
    float _amplitudeRate;
};

// Move the tiles of a target node across the Z axis.

class CC_DLL JumpTiles3D : public TiledGrid3DAction
{
public:
    JumpTiles3D(float duration, Size gridSize, unsigned int numberOfJumps, float amplitude)
        : TiledGrid3DAction(duration, std::move(gridSize))
        , _jumps( numberOfJumps )
        , _amplitude( amplitude )
        , _amplitudeRate(1.0f) 
    {}

    void setAmplitudeRate(float amplitudeRate) { _amplitudeRate = amplitudeRate; }

    virtual JumpTiles3D* clone() const override;
    virtual void step(float time) override;
    
protected:
    unsigned int _jumps;
    float _amplitude;
    float _amplitudeRate;
};

// Split the target node in many rows.
// Then move out some rows from left, move out the other rows from right.

class CC_DLL SplitRows : public TiledGrid3DAction
{
public :
    SplitRows(float duration, unsigned int nRows)
        : TiledGrid3DAction(duration, Size(1, nRows))
        , _rows( nRows )
    {}

    virtual SplitRows* clone() const override;
    virtual void step(float time) override;
    virtual void startWithTarget(Node *target) override;
    
protected:

    unsigned int _rows;
    Size _winSize;
};

// Split the target node in many columns.
// Then move out some columns from top, move out the other columns from bottom.

class CC_DLL SplitCols : public TiledGrid3DAction
{
public:
    
    SplitCols(float duration, unsigned int nCols)
        : TiledGrid3DAction(duration, Size(nCols, 1))
        , _cols(nCols)
    {}

    virtual SplitCols* clone() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void step(float time) override;
    
protected:
    unsigned int _cols;
    Size _winSize;
};

} // namespace cocos2d

#endif // __ACTION_CCTILEDGRID_ACTION_H__

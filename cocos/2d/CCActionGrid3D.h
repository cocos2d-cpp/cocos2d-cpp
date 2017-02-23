/****************************************************************************
Copyright (c) 2009      On-Core
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2016 Chukong Technologies Inc.

http://www.cocos2d-x.org

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

/**
 * @addtogroup actions
 * @{
 */

/** 
@brief Waves3D action.
@details This action is used for take effect on the target node as 3D waves.
        You can control the effect by these parameters:
        duration, grid size, waves count, amplitude.
*/
class CC_DLL Waves3D : public Grid3DAction
{
public:
    /**
    @brief Create an action with duration, grid size, waves and amplitude.
    @param duration Specify the duration of the Waves3D action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param waves Specify the waves count of the Waves3D action.
    @param amplitude Specify the amplitude of the Waves3D action.
    @return If the creation success, return a pointer of Waves3D action; otherwise, return nil.
    */
    static Waves3D* create(float duration, const Size& gridSize, unsigned int waves, float amplitude);

    /**
    @brief Get the amplitude of the effect.
    @return Return the amplitude of the effect.
    */
    float getAmplitude() const { return _amplitude; }
    /**
    @brief Set the amplitude to the effect.
    @param amplitude The value of amplitude will be set.
    */
    void setAmplitude(float amplitude) { _amplitude = amplitude; }

    /**
    @brief Get the amplitude rate of the effect.
    @return Return the amplitude rate of the effect.
    */
    float getAmplitudeRate() const { return _amplitudeRate; }
    /**
    @brief Set the amplitude rate of the effect.
    @param amplitudeRate The value of amplitude rate will be set.
    */
    void setAmplitudeRate(float amplitudeRate) { _amplitudeRate = amplitudeRate; }

    // Overrides
    virtual Waves3D* clone() const override;
    virtual void step(float time) override;
    
protected:
    Waves3D() {}
    virtual ~Waves3D() {}

    /** 
    @brief Initializes an action with duration, grid size, waves and amplitude.
    @param duration Specify the duration of the Waves3D action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param waves Specify the waves count of the Waves3D action.
    @param amplitude Specify the amplitude of the Waves3D action.
    @return If the initialization success, return true; otherwise, return false.
    */
    bool initWithDuration(float duration, const Size& gridSize, unsigned int waves, float amplitude);

protected:
    unsigned int _waves;
    float _amplitude;
    float _amplitudeRate;

private:
    Waves3D(const Waves3D &) = delete;
    const Waves3D & operator=(const Waves3D &) = delete;
};

/**
@brief FlipX3D action.
@details This action is used for flipping the target node on the x axis.
*/
class CC_DLL FlipX3D : public Grid3DAction
{
public:
    /**
    @brief Create the action with duration.
    @param duration Specify the duration of the FilpX3D action. It's a value in seconds.
    @return If the creation success, return a pointer of FilpX3D action; otherwise, return nil.
    */
    static FlipX3D* create(float duration);

    // Override
    virtual FlipX3D* clone() const override;
    virtual void step(float time) override;
    
protected:
    FlipX3D() {}
    virtual ~FlipX3D() {}

    /** 
    @brief Initializes an action with duration.
    @param duration Specify the duration of the FlipX3D action. It's a value in seconds.
    @return If the initialization success, return true; otherwise, return false.
    */
    bool initWithDuration(float duration);

    /** 
    @brief Initializes an action with duration and grid size.
    @param gridSize Specify the grid size of the FlipX3D action.
    @param duration Specify the duration of the FlipX3D action. It's a value in seconds.
    @return If the initialization success, return true; otherwise, return false.
    */
    virtual bool initWithSize(const Size& gridSize, float duration);

private:
    FlipX3D(const FlipX3D &) = delete;
    const FlipX3D & operator=(const FlipX3D &) = delete;
};

/**
@brief FlipY3D action.
@details This action is used for flipping the target node on the y axis.
*/
class CC_DLL FlipY3D : public FlipX3D
{
public:
    /**
    @brief Create the action with duration.
    @param duration Specify the duration of the FlipY3D action. It's a value in seconds.
    @return If the creation success, return a pointer of FlipY3D action; otherwise, return nil.
    */
    static FlipY3D* create(float duration);

    // Overrides
    virtual void step(float time) override;
    virtual FlipY3D* clone() const override;

protected:
    FlipY3D() {}
    virtual ~FlipY3D() {}
private:
    FlipY3D(const FlipY3D &) = delete;
    const FlipY3D & operator=(const FlipY3D &) = delete;
};

/**
@brief Lens3D action.
@details This action is used for take effect on the target node as lens.
        You can create the action by these parameters:
        duration, grid size, center position of lens, radius of lens.
        Also you can change the lens effect value & whether effect is concave by the setter methods.
*/
class CC_DLL Lens3D : public Grid3DAction
{
public:
    /**
    @brief Create the action with center position, radius, a grid size and duration.
    @param duration Specify the duration of the Lens3D action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param position Specify the center position of the lens.
    @param radius Specify the radius of the lens.
    @return If the creation success, return a pointer of Lens3D action; otherwise, return nil.
    */
    static Lens3D* create(float duration, const Size& gridSize, const Vec2& position, float radius);

    /**
    @brief Get the value of lens effect. Default value is 0.7.
    @return The value of lens effect.
    */
    float getLensEffect() const { return _lensEffect; }

    /**
    @brief Set the value of lens effect.
    @param lensEffect The value of lens effect will be set.
    */
    void setLensEffect(float lensEffect) { _lensEffect = lensEffect; }

    /**
    @brief Set whether lens is concave.
    @param concave Whether lens is concave.
    */
    void setConcave(bool concave) { _concave = concave; }

    /**
    @brief Get the center position of lens effect.
    @return The center position of lens effect.
    */
    const Vec2& getPosition() const { return _position; }

    /**
    @brief Set the center position of lens effect.
    @param position The center position will be set.
    */
    void setPosition(const Vec2& position);

    // Overrides
    virtual Lens3D* clone() const override;
    virtual void step(float time) override;
    
protected:
    Lens3D() {}
    virtual ~Lens3D() {}

    /**
    @brief Initializes the action with center position, radius, grid size and duration.
    @param duration Specify the duration of the Lens3D action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param position Specify the center position of the lens effect.
    @param radius Specify the radius of the lens effect.
    @return If the initialization success, return true; otherwise, return false.
    */
    bool initWithDuration(float duration, const Size& gridSize, const Vec2& position, float radius);

protected:
    /* lens center position */
    Vec2 _position;
    float _radius;
    /** lens effect. Defaults to 0.7 - 0 means no effect, 1 is very strong effect */
    float _lensEffect;
    /** lens is concave. (true = concave, false = convex) default is convex i.e. false */
    bool _concave;

    bool _dirty;

private:
    Lens3D(const Lens3D &) = delete;
    const Lens3D & operator=(const Lens3D &) = delete;
};

/**
@brief Ripple3D action.
@details This action is used for take effect on the target node as ripple.
        You can create the action by these parameters:
        duration, grid size, center position of ripple,
        radius of ripple, waves count, amplitude.
*/
class CC_DLL Ripple3D : public Grid3DAction
{
public:
    /**
    @brief Create the action with center position, radius, number of waves, amplitude, a grid size and duration.
    @param duration Specify the duration of the Ripple3D action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param position Specify the center position of the ripple effect.
    @param radius Specify the radius of the ripple effect.
    @param waves Specify the waves count of the ripple effect.
    @param amplitude Specify the amplitude of the ripple effect.
    @return If the creation success, return a pointer of Ripple3D action; otherwise, return nil.
    */
    static Ripple3D* create(float duration, const Size& gridSize, const Vec2& position, float radius, unsigned int waves, float amplitude);

    /**
    @brief Get the center position of ripple effect.
    @return The center position of ripple effect.
    */
    const Vec2& getPosition() const { return _position; }
    /**
    @brief Set the center position of ripple effect.
    @param position The center position of ripple effect will be set.
    */
    void setPosition(const Vec2& position);

    /**
    @brief Get the amplitude of ripple effect.
    @return The amplitude of ripple effect.
    */
    float getAmplitude() const { return _amplitude; }
    /**
    @brief Set the amplitude of ripple effect.
    @param fAmplitude The amplitude of ripple effect.
    */
    void setAmplitude(float fAmplitude) { _amplitude = fAmplitude; }

    /**
    @brief Get the amplitude rate of ripple effect.
    @return The amplitude rate of ripple effect.
    */
    float getAmplitudeRate() const { return _amplitudeRate; }
    /**
    @brief Set the amplitude rate of ripple effect.
    @param fAmplitudeRate The amplitude rate of ripple effect.
    */
    void setAmplitudeRate(float fAmplitudeRate) { _amplitudeRate = fAmplitudeRate; }

    // Override
    virtual Ripple3D* clone() const override;
    virtual void step(float time) override;

protected:
    Ripple3D() {}
    virtual ~Ripple3D() {}

    /**
    @brief Initializes the action with center position, radius, number of waves, amplitude, a grid size and duration.
    @param duration Specify the duration of the Ripple3D action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param position Specify the center position of the ripple effect.
    @param radius Specify the radius of the ripple effect.
    @param waves Specify the waves count of the ripple effect.
    @param amplitude Specify the amplitude of the ripple effect.
    @return If the initialization success, return true; otherwise, return false.
     */
    bool initWithDuration(float duration, const Size& gridSize, const Vec2& position, float radius, unsigned int waves, float amplitude);

protected:
    /* center position */
    Vec2 _position;
    float _radius;
    unsigned int _waves;
    float _amplitude;
    float _amplitudeRate;

private:
    Ripple3D(const Ripple3D &) = delete;
    const Ripple3D & operator=(const Ripple3D &) = delete;
};

/**
@brief Shaky3D action.
@details This action is used for take effect on the target node as shaky.
        You can create the action by these parameters:
        duration, grid size, range, whether shake on the z axis.
*/
class CC_DLL Shaky3D : public Grid3DAction
{
public:
    /** 
    @brief Create the action with a range, shake Z vertices, a grid and duration.
    @param initWithDuration Specify the duration of the Shaky3D action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param range Specify the range of the shaky effect.
    @param shakeZ Specify whether shake on the z axis.
    @return If the creation success, return a pointer of Shaky3D action; otherwise, return nil.
    */
    static Shaky3D* create(float initWithDuration, const Size& gridSize, int range, bool shakeZ);

    // Overrides
    virtual Shaky3D* clone() const override;
    virtual void step(float time) override;
    
protected:
    Shaky3D() {}
    virtual ~Shaky3D() {}
    
    /** 
    @brief Initializes the action with a range, shake Z vertices, grid size and duration.
    @param duration Specify the duration of the Shaky3D action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param range Specify the range of the shaky effect.
    @param shakeZ Specify whether shake on the z axis.
    @return If the Initialization success, return true; otherwise, return false.
    */
    bool initWithDuration(float duration, const Size& gridSize, int range, bool shakeZ);

protected:
    int _randrange;
    bool _shakeZ;

private:
    Shaky3D(const Shaky3D &) = delete;
    const Shaky3D & operator=(const Shaky3D &) = delete;
};

/**
@brief Liquid action.
@details This action is used for take effect on the target node as liquid.
        You can create the action by these parameters:
        duration, grid size, waves count, amplitude of the liquid effect.
*/
class CC_DLL Liquid : public Grid3DAction
{
public:
    /**
    @brief Create the action with amplitude, grid size, waves count and duration.
    @param duration Specify the duration of the Liquid action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param waves Specify the waves count of the Liquid action.
    @param amplitude Specify the amplitude of the Liquid action.
    @return If the creation success, return a pointer of Liquid action; otherwise, return nil.
    */
    static Liquid* create(float duration, const Size& gridSize, unsigned int waves, float amplitude);

    /**
    @brief Get the amplitude of the effect.
    @return Return the amplitude of the effect.
    */
    float getAmplitude() const { return _amplitude; }
    /**
    @brief Set the amplitude to the effect.
    @param amplitude The value of amplitude will be set.
    */
    void setAmplitude(float amplitude) { _amplitude = amplitude; }

    /**
    @brief Get the amplitude rate of the effect.
    @return Return the amplitude rate of the effect.
    */
    float getAmplitudeRate() const { return _amplitudeRate; }
    /**
    @brief Set the amplitude rate of the effect.
    @param amplitudeRate The value of amplitude rate will be set.
    */
    void setAmplitudeRate(float amplitudeRate) { _amplitudeRate = amplitudeRate; }

    // Overrides
    virtual Liquid* clone() const override;
    virtual void step(float time) override;
    
protected:
    Liquid() {}
    virtual ~Liquid() {}
    
    /**
    @brief Initializes the action with amplitude, grid size, waves count and duration.
    @param duration Specify the duration of the Liquid action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param waves Specify the waves count of the Liquid action.
    @param amplitude Specify the amplitude of the Liquid action.
    @return If the initialization success, return true; otherwise, return false.
    */
    bool initWithDuration(float duration, const Size& gridSize, unsigned int waves, float amplitude);

protected:
    unsigned int _waves;
    float _amplitude;
    float _amplitudeRate;

private:
    Liquid(const Liquid &) = delete;
    const Liquid & operator=(const Liquid &) = delete;
};

/**
@brief Waves action.
@details This action is used for take effect on the target node as waves.
        You can control the effect by these parameters:
        duration, grid size, waves count, amplitude,
        whether waves on horizontal and whether waves on vertical.
*/
class CC_DLL Waves : public Grid3DAction
{
public:
    /**
    @brief Create the action with amplitude, horizontal sin, vertical sin, grid size, waves count and duration.
    @param duration Specify the duration of the Waves action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param waves Specify the waves count of the Waves action.
    @param amplitude Specify the amplitude of the Waves action.
    @param horizontal Specify whether waves on horizontal.
    @param vertical Specify whether waves on vertical.
    @return If the creation success, return a pointer of Waves action; otherwise, return nil.
    */
    static Waves* create(float duration, const Size& gridSize, unsigned int waves, float amplitude, bool horizontal, bool vertical);

    /**
    @brief Get the amplitude of the effect.
    @return Return the amplitude of the effect.
    */
    float getAmplitude() const { return _amplitude; }
    /**
    @brief Set the amplitude to the effect.
    @param amplitude The value of amplitude will be set.
    */
    void setAmplitude(float amplitude) { _amplitude = amplitude; }

    /**
    @brief Get the amplitude rate of the effect.
    @return Return the amplitude rate of the effect.
    */
    float getAmplitudeRate() const { return _amplitudeRate; }
    /**
    @brief Set the amplitude rate of the effect.
    @param amplitudeRate The value of amplitude rate will be set.
    */
    void setAmplitudeRate(float amplitudeRate) { _amplitudeRate = amplitudeRate; }

    // Overrides
    virtual Waves* clone() const override;
    virtual void step(float time) override;
    
protected:
    Waves() {}
    virtual ~Waves() {}
    
    /**
    @brief Initializes the action with amplitude, horizontal sin, vertical sin, grid size, waves count and duration.
    @param duration Specify the duration of the Waves action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param waves Specify the waves count of the Waves action.
    @param amplitude Specify the amplitude of the Waves action.
    @param horizontal Specify whether waves on horizontal.
    @param vertical Specify whether waves on vertical.
    @return If the initialization success, return true; otherwise, return false.
    */
    bool initWithDuration(float duration, const Size& gridSize, unsigned int waves, float amplitude, bool horizontal, bool vertical);

protected:
    unsigned int _waves;
    float _amplitude;
    float _amplitudeRate;
    bool _vertical;
    bool _horizontal;

private:
    Waves(const Waves &) = delete;
    const Waves & operator=(const Waves &) = delete;
};

/**
@brief Twirl action.
@details This action is used for take effect on the target node as twirl.
        You can control the effect by these parameters:
        duration, grid size, center position, twirls count, amplitude.
*/
class CC_DLL Twirl : public Grid3DAction
{
public:
    /**
    @brief Create the action with center position, number of twirls, amplitude, a grid size and duration.
    @param duration Specify the duration of the Twirl action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param position Specify the center position of the twirl action.
    @param twirls Specify the twirls count of the Twirl action.
    @param amplitude Specify the amplitude of the Twirl action.
    @return If the creation success, return a pointer of Twirl action; otherwise, return nil.
    */
    static Twirl* create(float duration, const Size& gridSize, const Vec2& position, unsigned int twirls, float amplitude);

    /**
    @brief Get the center position of twirl action.
    @return The center position of twirl action.
    */
    const Vec2& getPosition() const { return _position; }
    /**
    @brief Set the center position of twirl action.
    @param position The center position of twirl action will be set.
    */
    void setPosition(const Vec2& position);

    /**
    @brief Get the amplitude of the effect.
    @return Return the amplitude of the effect.
    */
    float getAmplitude() const { return _amplitude; }
    /**
    @brief Set the amplitude to the effect.
    @param amplitude The value of amplitude will be set.
    */
    void setAmplitude(float amplitude) { _amplitude = amplitude; }

    /**
    @brief Get the amplitude rate of the effect.
    @return Return the amplitude rate of the effect.
    */
    float getAmplitudeRate() const { return _amplitudeRate; }
    /**
    @brief Set the amplitude rate of the effect.
    @param amplitudeRate The value of amplitude rate will be set.
    */
    void setAmplitudeRate(float amplitudeRate) { _amplitudeRate = amplitudeRate; }


    // Overrides
    virtual Twirl* clone() const override;
    virtual void step(float time) override;
    
protected:
    Twirl() {}
    virtual ~Twirl() {}

    /**
    @brief Initializes the action with center position, number of twirls, amplitude, a grid size and duration.
    @param duration Specify the duration of the Twirl action. It's a value in seconds.
    @param gridSize Specify the size of the grid.
    @param position Specify the center position of the twirl action.
    @param twirls Specify the twirls count of the Twirl action.
    @param amplitude Specify the amplitude of the Twirl action.
    @return If the initialization success, return true; otherwise, return false.
    */
    bool initWithDuration(float duration, const Size& gridSize, const Vec2& position, unsigned int twirls, float amplitude);

protected:
    /* twirl center */
    Vec2 _position;
    unsigned int _twirls;
    float _amplitude;
    float _amplitudeRate;

private:
    Twirl(const Twirl &) = delete;
    const Twirl & operator=(const Twirl &) = delete;
};

// end of actions group
/// @}

} // namespace cocos2d

#endif // __ACTION_CCGRID3D_ACTION_H__

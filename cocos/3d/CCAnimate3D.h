/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.
 Copyright (c) 2017 Iakov Sergeev <yahont@github>

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

#ifndef __CCANIMATE3D_H__
#define __CCANIMATE3D_H__

#include <limits>
#include <map>
#include <unordered_map>

#include "3d/CCAnimation3D.h"
#include "base/ccMacros.h"
#include "base/CCRef.h"
#include "2d/CCActionInterval.h"

namespace cocos2d {

class Bone3D;
class Sprite3D;
class EventCustom;

enum class Animate3DQuality
{
    QUALITY_NONE = 0, // it'll be ignore the curve-evaluating(the animation looks like stop), just accumulate transition time.
    QUALITY_LOW,      // low animation quality, it'll be more efficient.
    QUALITY_HIGH,     // high animation quality.
};

// Animates a Sprite3D given with an Animation3D

class CC_DLL Animate3D : public ActionInterval
{
public:
    struct FrameInfo {

        FrameInfo(int s, int e, float r = 30.0f)
            : startFrame(s)
            , endFrame(e)
            , frameRate(r)
            {}
        
        int startFrame;
        int endFrame;
        float frameRate;
    };
    
    Animate3D(Animation3D* animation, float fromTime = 0.0f, float duration = std::numeric_limits<float>::max());
    
    Animate3D(Animation3D* animation, FrameInfo const& info);

    virtual ~Animate3D();
    
    virtual Animate3D* clone() const override;
    virtual Animate3D* reverse() const override;

    virtual void startWithTarget(Node *target) override;
    
    virtual void step(float t) override;
    
    /**get & set speed, negative speed means playing reverse */
    float getSpeed() const;
    void setSpeed(float speed);
    
    /**get & set blend weight, weight must positive*/
    float getWeight() const { return _weight; }
    void setWeight(float weight);
    
    /**get & set origin interval*/
    void setOriginInterval(float interval);
    float getOriginInterval() const {return _originInterval; }
    
    /** get animate transition time between 3d animations */
    static float getTransitionTime() { return _transTime; }
    
    /** set animate transition time between 3d animations */
    static void setTransitionTime(float transTime) { if (transTime >= 0.f) _transTime = transTime; }
    
    /**set animate quality*/
    void setQuality(Animate3DQuality quality);
    
    /**get animate quality*/
    Animate3DQuality getQuality() const;


    struct Animate3DDisplayedEventInfo
    {
        int frame;
        Node* target;
        const ValueMap* userInfo;
    };
    void setKeyFrameUserInfo(int keyFrame, const ValueMap &userInfo);
    const ValueMap* getKeyFrameUserInfo(int keyFrame) const;
    ValueMap* getKeyFrameUserInfo(int keyFrame);
    

    
protected:
    
    void removeFromMap();
    
    virtual void at_stop() override;

protected:
    
    enum class Animate3DState
    {
        FadeIn,
        FadeOut,
        Running,
    };
    Animate3DState _state; //animation state
    Animation3D* _animation; //animation data

    float      _absSpeed; //playing speed
    float      _weight; //blend weight
    float      _start; //start time 0 - 1, used to generate sub Animate3D
    float      _last; //last time 0 - 1, used to generate sub Animate3D
    bool       _playReverse; // is playing reverse
    static float      _transTime; //transition time from one animate3d to another
    float      _accTransTime; // accumulate transition time
    float      _lastTime;     // last t (0 - 1)
    float      _originInterval;// save origin interval time
    float      _frameRate;
    
    // animation quality
    EvaluateType _translateEvaluate;
    EvaluateType _roteEvaluate;
    EvaluateType _scaleEvaluate;
    Animate3DQuality _quality;
    
    std::unordered_map<Bone3D*, Animation3D::Curve*> _boneCurves; //weak ref
    std::unordered_map<Node*, Animation3D::Curve*> _nodeCurves;
    
    std::unordered_map<int, ValueMap> _keyFrameUserInfos;
    std::unordered_map<int, EventCustom*> _keyFrameEvent;
    std::unordered_map<int, Animate3DDisplayedEventInfo> _displayedEventInfo;

    //sprite animates
    static std::unordered_map<Node*, Animate3D*> s_fadeInAnimates;
    static std::unordered_map<Node*, Animate3D*> s_fadeOutAnimates;
    static std::unordered_map<Node*, Animate3D*> s_runningAnimates;
};

// end of 3d group
/// @}

} // namespace cocos2d

#endif // __CCANIMATE3D_H__

/****************************************************************************
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

#ifndef __cocos2d_libs__CCTouchEventListener__
#define __cocos2d_libs__CCTouchEventListener__

#include "base/CCEventListener.h"
#include <vector>

/**
 * @addtogroup base
 * @{
 */

namespace cocos2d {

class Touch;

/** @class EventListenerTouchOneByOne
 * @brief Single touch event listener.
 */
class CC_DLL EventListenerTouchOneByOne : public EventListener
{
public:
    static const std::string LISTENER_ID;
    
    /** Create a one by one touch event listener.
     */
    static EventListenerTouchOneByOne* create();
    
    virtual ~EventListenerTouchOneByOne();
    
    /** Whether or not to swall touches.
     *
     * @param needSwallow True if needs to swall touches.
     */
    void setSwallowTouches(bool needSwallow);
    /** Is swall touches or not.
     *
     * @return True if needs to swall touches.
     */
    bool isSwallowTouches();
    
    /// Overrides
    EventListenerTouchOneByOne* clone() const;
    virtual bool checkAvailable() override;
    //

public:

    typedef std::function<bool(Touch*, Event*)> ccTouchBeganCallback;
    typedef std::function<void(Touch*, Event*)> ccTouchCallback;

    ccTouchBeganCallback onTouchBegan;
    ccTouchCallback onTouchMoved;
    ccTouchCallback onTouchEnded;
    ccTouchCallback onTouchCancelled;
    
protected:
    EventListenerTouchOneByOne();
    bool init();
    
private:
    std::vector<Touch*> _claimedTouches;
    bool _needSwallow;
    
    friend class EventDispatcher;
};

/** @class EventListenerTouchAllAtOnce
 * @brief Multiple touches event listener.
 */
class CC_DLL EventListenerTouchAllAtOnce : public EventListener
{
public:
    static const std::string LISTENER_ID;
    
    /** Create a all at once event listener.
     *
     * @return An autoreleased EventListenerTouchAllAtOnce object.
     */
    static EventListenerTouchAllAtOnce* create();

    virtual ~EventListenerTouchAllAtOnce();
    
    /// Overrides
    EventListenerTouchAllAtOnce* clone() const;
    virtual bool checkAvailable() override;
    //
public:

    typedef std::function<void(const std::vector<Touch*>&, Event*)> ccTouchesCallback;

    ccTouchesCallback onTouchesBegan;
    ccTouchesCallback onTouchesMoved;
    ccTouchesCallback onTouchesEnded;
    ccTouchesCallback onTouchesCancelled;
    
protected:
    EventListenerTouchAllAtOnce();
    bool init();
private:
    
    friend class EventDispatcher;
};

} // namespace cocos2d

// end of base group
/// @}

#endif /* defined(__cocos2d_libs__CCTouchEventListener__) */

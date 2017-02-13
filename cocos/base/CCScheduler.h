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

#ifndef __CCSCHEDULER_H__
#define __CCSCHEDULER_H__

#include "platform/CCPlatformDefine.h" // CC_DLL
#include "platform/CCPlatformMacros.h" // CC_DEPRECATED_ATTRIBUTE

#include <functional>
#include <limits>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace cocos2d {

extern const uint32_t CC_REPEAT_FOREVER;

class Scheduler;

class CC_DLL TimedJob {
public:
    TimedJob(void* target, std::function<void(float)> callback, size_t id)
        : _target(target)
        , _id(id)
        , _interval(0.0f)
        , _paused(false)
        , _repeat(CC_REPEAT_FOREVER)
        , _leftover(-std::numeric_limits<float>::epsilon())
        , _callback(callback)
        {
            CC_ASSERT(_target);
            CC_ASSERT(_callback);
        }

    template<typename T>
    TimedJob(T* target, void (T::*func)(float), size_t id)
        : TimedJob(target,
                   [target,func](float dt){ (target->*func)(dt); },
                   id)
        {}

    TimedJob(TimedJob const&) = default;
    TimedJob& operator=(TimedJob const&) = default;
    TimedJob(TimedJob &&) = default;
    TimedJob& operator=(TimedJob &&) = default;

    void* target() const { return _target; }
    size_t id() const { return _id; }
    bool paused() const { return _paused; }

    TimedJob & interval(float v)
    {
        _interval = v;
        return *this;
    }
    TimedJob & repeat(uint32_t v)
    {
        _repeat = v;
        return *this;
    }
    TimedJob & delay(float v)
    {
        if (v == 0.0f)
            _leftover = -std::numeric_limits<float>::epsilon();
        else
            _leftover = -v;
        return *this;
    }
    TimedJob & paused(bool v)
    {
        _paused = v;
        return *this;
    }

    void update(Scheduler &, float dt);

private:
    void trigger(float dt);
    void cancel(Scheduler &);

private:
    void* _target;
    size_t _id;

    float    _interval;
    uint32_t _paused:1;
    uint32_t _repeat:31;
    float    _leftover;

    std::function<void(float)> _callback;
};

// deprecated
class Ref;
typedef void (Ref::*SEL_SCHEDULE)(float);

struct _listEntry;
struct _hashSelectorEntry;

/** @brief Scheduler is responsible for triggering the scheduled callbacks.
You should not use system timer for your game logic. Instead, use this class.

There are 2 different types of callbacks (selectors):

- update selector: the 'update' selector will be called every frame. You can customize the priority.
- custom selector: A custom selector will be called every frame, or with a custom interval of time

The 'custom selectors' should be avoided when possible. It is faster, and consumes less memory to use the 'update selector'.

*/
class CC_DLL Scheduler final
{
public:
    Scheduler() = default;
    ~Scheduler();

    Scheduler(Scheduler const&) = delete;
    Scheduler & operator=(Scheduler const&) = delete;

    float getSpeedup() const;
    void setSpeedup(float speedup);

    void schedule(TimedJob timer);
    void unschedule(void* target, size_t id);

    /* updates per frame
     * The lower the priority, the earlier it is called.
     */
    template<typename T>
    void scheduleUpdate(T *target, int priority, bool paused)
    {
        this->schedulePerFrame([target](float dt){
            target->update(dt);
        }, target, priority, paused);
    }
    void unscheduleUpdate(void *target);

    // for both timed jobs and updates per frame
    void unscheduleAllForTarget(void *target);
    void unscheduleAll();

    void pauseTarget(void *target);
    void resumeTarget(void *target);
    
    bool isTargetPaused(void *target) const;

    void pauseAllTargets();
    void resumeAllTargets();

    /** Calls a function on the cocos2d thread. Useful when you need to call a cocos2d function from another thread.
     This function is thread safe.
     */
    void performFunctionInCocosThread(const std::function<void()> &function);
    
    /** 'update' the scheduler.
     * You should NEVER call this method, unless you know what you are doing.
     */
    void update(float dt);

private:
    // data types
    struct ListEntry
    {
        std::function<void(float)> callback;
        void* target;
        int   priority;
        bool  paused;
        bool  markedForDeletion; // selector will no longer be called and entry will be removed at end of the next tick
    };

    using updates_list_t = std::list<ListEntry>;
    using updates_hash_t = std::unordered_map<void*, updates_list_t::iterator>;

    // Hash Element used for "selectors with interval"
    struct HashTimerEntry {
        std::vector<std::unique_ptr<TimedJob>> timedJobs;
        int         timerIndex = -1;
        const void* currentJob = nullptr;
        bool        currentJobSalvaged = false;
        bool        paused;
    };

    using timedjobs_hash_t = std::unordered_map<void*,std::unique_ptr<HashTimerEntry>>;

private:
    // member helpers
    void updatePausedState(bool paused);
    void updatePausedState(void* target, bool paused);
    void unscheduleUpdate(updates_hash_t::iterator);

    /** Schedules the 'callback' function for a given target with a given priority.
     The 'callback' selector will be called every frame.
     The lower the priority, the earlier it is called.
     @note This method is only for internal use.
     */
    void schedulePerFrame(std::function<void(float)>, void *target, int priority, bool paused);
    
private:
    float _speedup = 1.0f;

    // TODO make a separate class for PriorityList
    // "updates with priority" stuff
    updates_list_t _updatesList; // list sorted by priority
    updates_hash_t _hashForUpdates; // hash used to fetch quickly the list entries for pause,delete,etc

    // TODO store all TimedJobs in a single vector (might be based on PriorityList)
    // Used for "selectors with interval"
    timedjobs_hash_t _hashForTimers;

    // TODO get rid of them
    void* _currentTarget = nullptr;
    bool _currentTargetSalvaged = false;
    bool _updateHashLocked = false;
    
    // Used for "perform Function"
    std::vector<std::function<void()>> _functionsToPerform;
    std::mutex _performMutex;

public: // deprecated
#define CC_SCHEDULE_SELECTOR(_SELECTOR) static_cast<cocos2d::SEL_SCHEDULE>(&_SELECTOR)
    CC_DEPRECATED_ATTRIBUTE void schedule(std::function<void(float)>, void *target, float interval, unsigned int repeat, float delay, bool paused, const std::string& id);
    CC_DEPRECATED_ATTRIBUTE void schedule(std::function<void(float)>, void *target, float interval, bool paused, const std::string& id);
    CC_DEPRECATED_ATTRIBUTE void schedule(SEL_SCHEDULE selector, Ref *target, float interval, unsigned int repeat, float delay, bool paused);
    CC_DEPRECATED_ATTRIBUTE void schedule(SEL_SCHEDULE selector, Ref *target, float interval, bool paused);
    CC_DEPRECATED_ATTRIBUTE void unschedule(const std::string& id, void *target);
    CC_DEPRECATED_ATTRIBUTE void unschedule(SEL_SCHEDULE selector, Ref *target);
};

// end of base group
/** @} */

} // namespace cocos2d

#endif // __CCSCHEDULER_H__

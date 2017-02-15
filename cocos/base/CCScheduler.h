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

#include <cstdint>
#include <functional>
#include <limits>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace cocos2d {

class Scheduler;

class CC_DLL Job {
public:

    Job(Job const&) = default;
    Job& operator=(Job const&) = default;

    bool unscheduled() const { return (_properties & TO_DELETE_BIT); }
    uint32_t properties() const { return _properties; }
    void* target() const { return _target; }
    bool paused() const { return (_repeat & PAUSED_BIT); }

    void update(Scheduler &, float dt);

    void interval(float v)
    {
        _interval = v;
    }
    void repeat(uint32_t v)
    {
        CC_ASSERT(v <= REPEAT_FOREVER);
        _repeat = v;
    }
    void delay(float v)
    {
        if (v <= 0.0f)
            _leftover = -std::numeric_limits<float>::epsilon();
        else
            _leftover = -v;
    }
    void paused(bool v)
    {
        _repeat = ((_repeat & REPEAT_FOREVER) | (uint32_t(v) << 31));
    }

    void unschedule()
    {
        _properties |= TO_DELETE_BIT;
    }

    bool operator<(Job const& j) const
    {
        return _properties < j._properties
            || (_properties == j._properties && _target < j._target);
    }

protected:
    // encoded in properties

    // for properties
    static constexpr uint32_t TO_DELETE_BIT = (uint32_t(1) << 31);

    enum Type : uint32_t
    {
        ACTION = (uint32_t(0) << 29),
        UPDATE = (uint32_t(1) << 29),
        TIMED  = (uint32_t(2) << 29),
        TYPE_BITMASK = (uint32_t(3) << 29)
    };

    static_assert(TO_DELETE_BIT > TYPE_BITMASK);

    // for repeat
    static constexpr uint32_t PAUSED_BIT     = (uint32_t(1) << 31);
public:
    static constexpr uint32_t REPEAT_FOREVER = ~PAUSED_BIT;

    static_assert(PAUSED_BIT > REPEAT_FOREVER);

protected:

    Job(uint32_t properties, void* target, std::function<void(float)> callback)
        : _properties(properties)
        , _target(target)
        , _interval(0.0f)
        , _repeat(REPEAT_FOREVER)
        , _leftover(-std::numeric_limits<float>::epsilon())
        , _callback(callback)
        {
            CC_ASSERT(_target);
            CC_ASSERT(_callback);
            CC_ASSERT(!(properties & TO_DELETE_BIT));
        }

private:
    void trigger(float dt);
    void cancel(Scheduler &);

protected:
    uint32_t _properties;

private:
    void*    _target;

    float    _interval;
    uint32_t _repeat;
    float    _leftover;

    std::function<void(float)> _callback;
};

constexpr uint32_t CC_REPEAT_FOREVER = Job::REPEAT_FOREVER;

class UpdateJob : public Job {
public:

    static constexpr uint32_t PRIORITY_BITMASK = 0x00FFFFFF;
    static constexpr uint32_t MAX_PRIORITY = PRIORITY_BITMASK;
    using priority_t = uint32_t;

    static_assert(TYPE_BITMASK > PRIORITY_BITMASK);

public:
    template<typename T>
    UpdateJob(priority_t priority, T* target)
        : UpdateJob(make_properties(priority),
                    target,
                    [target](float dt){ target->update(dt); })
        {}

    UpdateJob(UpdateJob const&) = default;
    UpdateJob& operator=(UpdateJob const&) = default;
    UpdateJob(UpdateJob &&) = default;
    UpdateJob& operator=(UpdateJob &&) = default;

    priority_t priority() const
    {
        return  (PRIORITY_BITMASK & properties());
    }

    static uint32_t make_properties(priority_t priority)
    {
        CC_ASSERT(priority <= MAX_PRIORITY);
        return (Type::UPDATE | priority);
    }

    UpdateJob & paused(bool v)
    {
        Job::paused(v);
        return *this;
    }
};

class TimedJob : public Job {
public:
    using id_t = uint16_t;
    static constexpr uint32_t ID_BITMASK = uint32_t(id_t(-1));

public:
    TimedJob(id_t id, void* target, std::function<void(float)> callback)
        : Job(make_properties(id), target, callback)
        {}

    template<typename T>
    TimedJob(id_t id, T* target, void (T::*func)(float))
        : TimedJob(id, target, [target,func](float dt){ (target->*func)(dt); })
        {}

    TimedJob(TimedJob const&) = default;
    TimedJob& operator=(TimedJob const&) = default;
    TimedJob(TimedJob &&) = default;
    TimedJob& operator=(TimedJob &&) = default;

    id_t id() const { return  (ID_BITMASK & properties()); }

    static uint32_t make_properties(id_t id)
    {
        return (Type::TIMED | uint32_t(id));
    }

    TimedJob & interval(float v)
    {
        Job::interval(v);
        return *this;
    }
    TimedJob & repeat(uint32_t v)
    {
        Job::repeat(v);
        return *this;
    }
    TimedJob & delay(float v)
    {
        Job::delay(v);
        return *this;
    }
    TimedJob & paused(bool v)
    {
        Job::paused(v);
        return *this;
    }
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
    void unscheduleTimedJob(TimedJob::id_t id, void* target);

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
    
    void pauseAllTargets();
    void resumeAllTargets();

    /** Calls a function on the cocos2d thread. Useful when you need to call a cocos2d function from another thread.
     This function is thread safe.
     */
    void performFunctionInCocosThread(std::function<void()> function);
    
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

private:
    // member helpers
    void updatePausedStateForTarget(void* target, bool paused);
    void updatePausedStateForAll(bool paused);
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

    // Used for "selectors with interval"
    std::vector<Job> _jobs;
    std::vector<Job> _jobsToAdd;

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

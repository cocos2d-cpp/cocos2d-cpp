/****************************************************************************
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

#ifndef BASE_CCSCHEDULER_H
#define BASE_CCSCHEDULER_H

#include "platform/CCPlatformDefine.h" // CC_DLL
#include "platform/CCPlatformMacros.h" // CC_DEPRECATED_ATTRIBUTE

#include <algorithm> // upper_bound
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

/* Updates per frame
 * The lower the priority, the earlier it is called.
 * Unique per target
 */
class CC_DLL UpdateJobId {
protected:
    using priority_type = int32_t;

    void*         _target;
    priority_type _priority;

public:

    UpdateJobId(void* target, priority_type priority)
        : _target(target)
        , _priority(priority)
        {}
    bool operator<(UpdateJobId const& a) const
    {
        return _priority < a._priority
            || (_priority == a._priority
                && _target < a._target);
    }
};

class UpdateJob : public UpdateJobId {
public:
    using priority_type = UpdateJobId::priority_type;

public:
    template<typename T>
        UpdateJob(T* target, priority_type priority = 0)
            : UpdateJobId{target, priority}
            , _callback([target](float dt){ target->update(dt); })
        {
            CC_ASSERT(target);
        }

    UpdateJob(UpdateJob const&) = default;
    UpdateJob& operator=(UpdateJob const&) = default;
    UpdateJob(UpdateJob &&) = default;
    UpdateJob& operator=(UpdateJob &&) = default;

    UpdateJob & paused(bool v)
    {
        _paused = v;
        return *this;
    }
    void unschedule()
    {
        _unscheduled = true;
    }
    void update(float dt)
    {
        _callback(dt);
    }

public:

    void* target() const
    {
        return _target;
    }
    priority_type priority() const
    {
        return _priority;
    }
    bool unscheduled() const
    {
        return _unscheduled;
    }
    bool paused() const
    {
        return _paused;
    }

private:

    bool _unscheduled = false;
    bool _paused = false;
    std::function<void(float)> _callback;
};

/*
 * Executed by INTERVAL after DELAY for REPEAT times, but within a
 * frame always after UpdateJob
 * Unique per (target + id) unless id is negative, e.g. DEFAULT_ID
 * If a new TimedJob is scheduled with a non-negative id, any TimedJob scheduled
 * for the target with the same non-negative id is unscheduled
 * There can be many TimedJob's for a target with equal negative id's
 */
class CC_DLL TimedJobId {
public:
    using id_type = int32_t;

protected:
    void*   _target;
    id_type _id;

public:
    TimedJobId(void* target, id_type id)
        : _target(target)
        , _id(id)
        {}
    bool operator<(TimedJobId const& a) const
    {
        return _target < a._target
            || (_target == a._target
                && _id < a._id);
    }
};

class CC_DLL TimedJob : public TimedJobId {
public:
    using id_type = TimedJobId::id_type;
    static constexpr id_type DEFAULT_ID = -1;

public:
    // for _repeat
    static constexpr uint32_t TO_DELETE_BIT  = (uint32_t(1) << 31);
    static constexpr uint32_t PAUSED_OFFSET  = 30;
    static constexpr uint32_t PAUSED_BIT     = (uint32_t(1) << PAUSED_OFFSET);
    static constexpr uint32_t FOREVER_BIT    = (uint32_t(1) << 29);
    static constexpr uint32_t REPEAT_BITMASK = (FOREVER_BIT | (FOREVER_BIT - 1));

    static_assert(TO_DELETE_BIT > PAUSED_BIT);
    static_assert(PAUSED_BIT    > FOREVER_BIT);

public:

    TimedJob(void* target, std::function<void(float)> callback, id_type id = DEFAULT_ID)
        : TimedJobId{target, id}
        , _callback(callback)
        {
            CC_ASSERT(_target);
            CC_ASSERT(_callback);
        }

    template<typename T>
    TimedJob(T* target, void (T::*func)(float), id_type id = DEFAULT_ID)
        : TimedJob(target, [target,func](float dt){ (target->*func)(dt); }, id)
        {}


    TimedJob(TimedJob const&) = default;
    TimedJob& operator=(TimedJob const&) = default;
    TimedJob(TimedJob &&) = default;
    TimedJob& operator=(TimedJob &&) = default;

public:

    TimedJob & delay(float v)
    {
        CC_ASSERT(v >= 0.0f);

        if (v <= 0.0f)
            _leftover = -std::numeric_limits<float>::epsilon();
        else
            _leftover = -v;

        return *this;
    }
    TimedJob & interval(float v)
    {
        _interval = v;
        return *this;
    }
    TimedJob & repeat(uint32_t v)
    {
        CC_ASSERT(v <= FOREVER_BIT);
        _repeat = ((_repeat & ~REPEAT_BITMASK) | v);
        return *this;
    }
    TimedJob & paused(bool v)
    {
        _repeat = ((_repeat & ~PAUSED_BIT) | (uint32_t(v) << PAUSED_OFFSET));
        return *this;
    }
    void unschedule()
    {
        _repeat |= (TO_DELETE_BIT | PAUSED_BIT);
    }

public:

    void* target() const
    {
        return _target;
    }
    id_type id() const
    {
        return _id;
    }
    bool unscheduled() const
    {
        return (_repeat & TO_DELETE_BIT);
    }
    bool paused() const
    {
        return (_repeat & PAUSED_BIT);
    }
    bool forever() const
    {
        return (_repeat & FOREVER_BIT);
    }

    void update(float dt);

private:

    float    _interval = 0.0f;
    uint32_t _repeat   = FOREVER_BIT;
    float    _leftover = -std::numeric_limits<float>::epsilon();

    std::function<void(float)> _callback;
};

constexpr uint32_t CC_REPEAT_FOREVER = TimedJob::FOREVER_BIT;

// deprecated
class Ref;
typedef void (Ref::*SEL_SCHEDULE)(float);

/** @brief Scheduler is responsible for triggering the scheduled callbacks.
You should not use system timer for your game logic. Instead, use this class.

There are 2 different types of callbacks (selectors):

- UpdateJob: the 'update' selector will be called every frame. You can customize the priority.
- TimedJob: A custom selector will be called every frame, or with a custom interval of time

*/
class CC_DLL Scheduler final
{
public:
    Scheduler() = default;
    Scheduler(Scheduler const&) = delete;
    Scheduler & operator=(Scheduler const&) = delete;

    float getSpeedup() const
    {
        return _speedup;
    }
    void setSpeedup(float speedup)
    {
        CC_ASSERT(speedup > std::numeric_limits<float>::epsilon());
        _speedup = speedup;
    }

private:

    template<typename Vector, typename Job>
        void schedule(Vector & v, Job job)
        {
            v.insert( std::upper_bound( v.begin(), v.end(), job),
                      job);
        }

public:

    void schedule(UpdateJob job) { schedule( _updateJobsToAdd, job); }
    void schedule(TimedJob job)  { schedule( _timedJobsToAdd,  job); }

    void unscheduleUpdateJob(void * target);
    void unscheduleTimedJob(void * target, TimedJob::id_type id);

    void unscheduleAllForTarget(void *target);
    void unscheduleAllJobs();

    void pauseJobsForTarget(void *target);
    void resumeJobsForTarget(void *target);
    
    void pauseAllJobs();
    void resumeAllJobs();

    /** Calls a function on the cocos2d thread. Useful when you need to call a cocos2d function from another thread.
     This function is thread safe.
     */
    void performFunctionInCocosThread(std::function<void()> function);
    
    /** 'update' the scheduler.
     * You should NEVER call this method, unless you know what you are doing.
     */
    void update(float dt);

private:

    void updatePausedStateForTarget(void* target, bool paused);

private:

    float _speedup = 1.0f;

    std::vector<UpdateJob> _updateJobs;
    std::vector<UpdateJob> _updateJobsToAdd;

    std::vector<TimedJob> _timedJobs;
    std::vector<TimedJob> _timedJobsToAdd;

    std::unordered_map<void*,UpdateJob::priority_type> _update_target_to_priority;

    // Used for "perform Function"
    std::vector<std::function<void()>> _functionsToPerform;
    std::mutex _performMutex;

public: // deprecated

#define CC_SCHEDULE_SELECTOR(_SELECTOR) static_cast<cocos2d::SEL_SCHEDULE>(&_SELECTOR)
    CC_DEPRECATED_ATTRIBUTE void schedule(std::function<void(float)> callback, void *target, float interval, unsigned int repeat, float delay, bool paused, const std::string& id);
    CC_DEPRECATED_ATTRIBUTE void schedule(std::function<void(float)> callback, void *target, float interval, bool paused, const std::string& id);
    CC_DEPRECATED_ATTRIBUTE void schedule(SEL_SCHEDULE selector, Ref *target, float interval, unsigned int repeat, float delay, bool paused);
    CC_DEPRECATED_ATTRIBUTE void schedule(SEL_SCHEDULE selector, Ref *target, float interval, bool paused);
    CC_DEPRECATED_ATTRIBUTE void unschedule(const std::string& id, void *target);
    CC_DEPRECATED_ATTRIBUTE void unschedule(SEL_SCHEDULE selector, Ref *target);
};

} // namespace cocos2d

#endif // BASE_CCSCHEDULER_H

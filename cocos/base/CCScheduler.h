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

class JobId {
protected:
    uint32_t _properties;
    void*    _target;

public:

    JobId(uint32_t properties, void* target)
        : _properties(properties)
        , _target(target)
        {}

    bool operator<(JobId const& a) const
    {
        return _properties < a._properties
            || (_properties == a._properties
                && _target < a._target);
    }
};

class CC_DLL Job : public JobId {
public:

    // for _properties
    enum Type : uint32_t
    {
        ACTION = (uint32_t(0) << 30),
        UPDATE = (uint32_t(1) << 30),
        TIMED  = (uint32_t(2) << 30),
        TYPE_BITMASK = (uint32_t(3) << 30)
    };

    // for _repeat
    static constexpr uint32_t TO_DELETE_BIT  = (uint32_t(1) << 31);
    static constexpr uint32_t PAUSED_OFFSET  = 30;
    static constexpr uint32_t PAUSED_BIT     = (uint32_t(1) << PAUSED_OFFSET);
    static constexpr uint32_t FOREVER_BIT    = (uint32_t(1) << 29);
    static constexpr uint32_t REPEAT_BITMASK = (FOREVER_BIT | (FOREVER_BIT - 1));

    static_assert(TO_DELETE_BIT  > PAUSED_BIT);
    static_assert(PAUSED_BIT     > REPEAT_BITMASK);
    static_assert(REPEAT_BITMASK > FOREVER_BIT);

public:
    Job(Job const&) = default;
    Job& operator=(Job const&) = default;
    Job(Job &&) = default;
    Job& operator=(Job &&) = default;

    uint32_t properties() const
    {
        return _properties;
    }
    Type type() const
    {
        return static_cast<Type>(properties() & TYPE_BITMASK);
    }
    void* target() const
    {
        return _target;
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

    void update(Scheduler &, float dt);

    void delay(float v)
    {
        if (v <= 0.0f)
            _leftover = -std::numeric_limits<float>::epsilon();
        else
            _leftover = -v;
    }
    void interval(float v)
    {
        _interval = v;
    }
    void repeat(uint32_t v)
    {
        CC_ASSERT(v <= FOREVER_BIT);
        _repeat = ((_repeat & ~REPEAT_BITMASK) | v);
    }
    void paused(bool v)
    {
        _repeat = ((_repeat & ~PAUSED_BIT) | (uint32_t(v) << PAUSED_OFFSET));
    }

    void unschedule()
    {
        _repeat |= (TO_DELETE_BIT | PAUSED_BIT);
    }

protected:

    Job(uint32_t properties, void* target, std::function<void(float)> callback)
        : JobId{properties, target}
        , _interval(0.0f)
        , _repeat(FOREVER_BIT)
        , _leftover(-std::numeric_limits<float>::epsilon())
        , _callback(callback)
        {
            CC_ASSERT(_target);
            CC_ASSERT(_callback);
        }

private:
    void trigger(float dt);
    void cancel(Scheduler &);

private:
    float    _interval;
    uint32_t _repeat;
    float    _leftover;

    std::function<void(float)> _callback;
};

constexpr uint32_t CC_REPEAT_FOREVER = Job::FOREVER_BIT;

/* Updates per frame
 * The lower the priority, the earlier it is called.
 * Unique per target
 */
class UpdateJob : public Job {
public:

    using priority_t = uint32_t;
    static constexpr priority_t PRIORITY_BITMASK = 0x1FFFFFFF;
    static constexpr priority_t MAX_PRIORITY = PRIORITY_BITMASK;

public:
    template<typename T>
    UpdateJob(priority_t priority, T* target)
        : Job(make_properties(priority),
              target,
              [target](float dt){ target->update(dt); })
        {
            CC_ASSERT(target);
            CC_ASSERT(priority <= MAX_PRIORITY);
        }

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
        return (Type::UPDATE | (PRIORITY_BITMASK & priority));
    }

    UpdateJob & paused(bool v)
    {
        Job::paused(v);
        return *this;
    }
};

/*
 * Executed by INTERVAL after DELAY for REPEAT times, but within a
 * frame always after ActionJob and UpdateJob
 * Unique per (target + id)
 */
class TimedJob : public Job {
public:
    // can be enlarged to 0x1FFFFFFF, see Job::_properties
    using id_t = uint32_t;
    static constexpr id_t ID_BITMASK = 0x1FFFFFFF;
    static constexpr id_t MAX_ID = 0x1FFFFFFF;

public:
    TimedJob(id_t id, void* target, std::function<void(float)> callback)
        : Job(make_properties(id), target, callback)
        {}

    template<typename T>
    TimedJob(id_t id, T* target, void (T::*func)(float))
        : TimedJob(id, target, [target,func](float dt){ (target->*func)(dt); })
        {
            CC_ASSERT(target);
            CC_ASSERT(id <= MAX_ID);
        }

    TimedJob(TimedJob const&) = default;
    TimedJob& operator=(TimedJob const&) = default;
    TimedJob(TimedJob &&) = default;
    TimedJob& operator=(TimedJob &&) = default;

    id_t id() const { return  (ID_BITMASK & properties()); }

    static uint32_t make_properties(id_t id)
    {
        return (Type::TIMED | (ID_BITMASK & id));
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
    ~Scheduler();

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

    template<typename JobType>
        void schedule(JobType job)
        {
            static_assert(std::is_base_of<Job,JobType>::value);
            CC_ASSERT(!job.unscheduled());

            _jobsToAdd.insert
            (
                std::upper_bound
                (
                    _jobsToAdd.begin(),
                    _jobsToAdd.end(),
                    job
                ),
                job
            );
        }

    void unscheduleUpdate(void *target);
    void unscheduleTimedJob(TimedJob::id_t id, void* target);

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

    // member helpers
    void updatePausedStateForTarget(void* target, bool paused);
    void updatePausedStateForAll(bool paused);

private:

    float _speedup = 1.0f;

    std::vector<Job> _jobs;
    std::vector<Job> _jobsToAdd;

    // Used for "perform Function"
    std::vector<std::function<void()>> _functionsToPerform;
    std::mutex _performMutex;

public: // deprecated

    template<typename T>
    CC_DEPRECATED_ATTRIBUTE
    void scheduleUpdate(T *target, uint32_t priority, bool paused)
    {
        schedule(
            UpdateJob(priority, target)
                .paused(paused)
        );
    }
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

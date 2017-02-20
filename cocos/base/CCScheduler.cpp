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

#include "base/CCScheduler.h"

#include "base/ccMacros.h"

namespace cocos2d {

void TimedJob::update(float dt)
{
    CC_ASSERT(!paused());
    CC_ASSERT(!unscheduled());
    CC_ASSERT(0.0f <= dt);

    if (dt < std::numeric_limits<float>::epsilon())
    {
        return;
    }

    if (_leftover < 0.0f)
    {
        if (_leftover + dt < 0.0f)
        {
            _leftover += dt;
            return;
        }
        
        // first run after delay
        _callback(0.0f);

        if (!_repeat)
        {
            unschedule();
            return;
        }

        if (_interval == 0.0f)
        {
            _leftover = 0.0f;
            return;
        }

        if (_leftover == -std::numeric_limits<float>::epsilon())
        {
            _leftover = 0.0;
        }

        dt += _leftover;
        _leftover = 0.0f;
    }

    // if _interval == 0, should trigger once every frame
    float interval = (0.0f < _interval) ? _interval : dt;

    for (_leftover += dt; interval <= _leftover; _leftover -= interval)
    {
        _callback(interval);

        if (!forever() && !--_repeat)
        {
            unschedule();
            return;
        }
    }
}

// implementation of Scheduler

using update_map       = std::unordered_map<void*,int32_t>;
using update_vector    = std::vector<UpdateJob>;
using update_iterators = std::pair<update_map::iterator,update_vector::iterator>;

inline
static update_iterators findUpdateJob(update_map & m, update_vector & v, void* target)
{
    update_iterators iterators{ m.find(target), v.end() };

    if (iterators.first != m.end())
    {
        const UpdateJobId jobId{ target, iterators.first->second };

        iterators.second = std::lower_bound(v.begin(), v.end(), jobId);
        
        // UpdateJobs are unique per target
        CC_ASSERT(iterators.second != v.end());
        CC_ASSERT(iterators.second->target() == target);
    }

    return iterators;
}

using timed_vector   = std::vector<TimedJob>;
using timed_iterator = timed_vector::iterator;

inline
static timed_iterator findFirstTimedJobForTarget(timed_vector & v, void * target,
                                                 int32_t id = std::numeric_limits<TimedJob::id_type>::min())
{
    return std::lower_bound( v.begin(), v.end(), TimedJobId{ target, id});
}

void Scheduler::unscheduleUpdateJob(void *target)
{
    auto iterators = findUpdateJob(_update_target_to_priority, _updateJobs, target);

    if (iterators.second != _updateJobs.end())
    {
        _update_target_to_priority.erase( iterators.first);
        iterators.second->unschedule();
    }
}

void Scheduler::unscheduleTimedJob(void *target, int32_t id)
{
    auto lower_bound = findFirstTimedJobForTarget(_timedJobs, target, id);
    
    if ( lower_bound != _timedJobs.end()
         && lower_bound->target() == target
         && lower_bound->id()     == id )
    {
        lower_bound->unschedule();
    }
}

template<typename V>
static void unscheduleAllHelper(V & v)
{
    for (auto & job : v)
        job.unschedule();
}

void Scheduler::unscheduleAll()
{
    unscheduleAllHelper( _updateJobs);
    _update_target_to_priority.clear();
    unscheduleAllHelper( _updateJobsToAdd);
    unscheduleAllHelper( _timedJobs);
    unscheduleAllHelper( _timedJobsToAdd);
}

template<typename V>
static void unscheduleAllForTargetHelper(V & v, void const* const target)
{
   for (auto & job : v)
        if (job.target() == target)
            job.unschedule();
}

void Scheduler::unscheduleAllForTarget(void *target)
{
    unscheduleUpdateJob(target);

    unscheduleAllForTargetHelper( _updateJobsToAdd, target);

    auto it = findFirstTimedJobForTarget( _timedJobs, target);
    for (; it != _timedJobs.end() && it->target() == target; it++)
        it->unschedule();

    unscheduleAllForTargetHelper( _timedJobsToAdd,  target);
}

template<typename V>
static void updatePausedStateForAll(V & v, bool paused)
{
    for (auto & job : v)
        job.paused(paused);
}

void Scheduler::pauseAllTargets()
{
    updatePausedStateForAll( _updateJobs,      true);
    updatePausedStateForAll( _updateJobsToAdd, true);
    updatePausedStateForAll( _timedJobs,       true);
    updatePausedStateForAll( _timedJobsToAdd,  true);
}

void Scheduler::resumeAllTargets()
{
    updatePausedStateForAll( _updateJobs,      false);
    updatePausedStateForAll( _updateJobsToAdd, false);
    updatePausedStateForAll( _timedJobs,       false);
    updatePausedStateForAll( _timedJobsToAdd,  false);
}

template<typename V>
static void updatePausedStateForTargetHelper(V & v, void const* const target, bool paused)
{
    for (auto & job : v)
        if (job.target() == target)
            job.paused(paused);
}

void Scheduler::updatePausedStateForTarget(void *target, bool paused)
{
    auto iterators = findUpdateJob(_update_target_to_priority, _updateJobs, target);
    if (iterators.second != _updateJobs.end())
        iterators.second->paused(paused);

    updatePausedStateForTargetHelper(_updateJobsToAdd, target, paused);

    auto it = findFirstTimedJobForTarget( _timedJobs, target);
    for (; it != _timedJobs.end() && it->target() == target; it++)
        it->paused(paused);

    updatePausedStateForTargetHelper(_timedJobsToAdd,  target, paused);
}

void Scheduler::pauseTarget(void *target)
{
    updatePausedStateForTarget(target, true);
}

void Scheduler::resumeTarget(void *target)
{
    updatePausedStateForTarget(target, false);
}

void Scheduler::performFunctionInCocosThread(std::function<void()> function)
{
    std::lock_guard<std::mutex> locker(_performMutex);
    _functionsToPerform.push_back(function);
}

template<typename V>
static void run_and_erase_unscheduled(V & v, float dt)
{
    size_t move_to_idx = 0;

    const size_t size  = v.size();

    for (size_t i = 0; i < size; i++)
    {
        if (! v[i].unscheduled())
        {
            if (! v[i].paused())
                v[i].update(dt);

            if (move_to_idx != i)
                v[move_to_idx] = std::move( v[i]);

            move_to_idx++;
        }
    }

    v.erase(v.begin() + move_to_idx, v.end());
}

// main loop
void Scheduler::update(float dt)
{
    dt *= _speedup;

    run_and_erase_unscheduled( _updateJobs, dt);
    run_and_erase_unscheduled( _timedJobs, dt);

    _updateJobs.reserve(_updateJobs.size() + _updateJobsToAdd.size());

    for (auto & job : _updateJobsToAdd)
    {
        if (job.unscheduled())
            continue;

        auto iterators = findUpdateJob(_update_target_to_priority, _updateJobs, job.target());

        if (iterators.second != _updateJobs.end())
        {
            iterators.first->second = job.priority();
            _updateJobs.erase( iterators.second);
        }
        else
        {
            _update_target_to_priority[ job.target() ] = job.priority();
        }

        _updateJobs.insert(
            std::upper_bound(_updateJobs.begin(), _updateJobs.end(), job),
            std::move(job)
        );
    }

    _updateJobsToAdd.clear();

    // Timed

    _timedJobs.reserve(_timedJobs.size() + _timedJobsToAdd.size());

    auto begin = _timedJobs.begin();

    for (auto & job : _timedJobsToAdd)
    {
        if (job.unscheduled())
            continue;

        const auto end = _timedJobs.end();

        begin = std::lower_bound(begin, end, job);

        if (begin == end || job < *begin)
            begin = _timedJobs.insert(begin, job);
        else
            *begin = job;
    }

    _timedJobsToAdd.clear();

    // Functions allocated from another thread

    if( !_functionsToPerform.empty() )
    {
        std::vector<std::function<void()>> tmp;
        {
            std::lock_guard<std::mutex> locker(_performMutex);
            // if new functions are added in callback,
            // it will cause thread deadlock.
            tmp = std::move(_functionsToPerform);
        }
        for(auto function : tmp)
        {
            function();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//************************ DEPRECATED ****************************************//
////////////////////////////////////////////////////////////////////////////////

static int32_t to_id(SEL_SCHEDULE selector)
{
    return static_cast<int32_t>(std::hash<void*>()(reinterpret_cast<void*>(selector)));
}
static int32_t to_id(std::string const& id)
{
    return static_cast<int32_t>(std::hash<std::string>()(id));
}

void Scheduler::schedule(std::function<void(float)> callback, void *target, float interval, unsigned int repeat, float delay, bool paused, const std::string& id)
{
    schedule(
        TimedJob(target, to_id(id), callback)
            .interval(interval)
            .repeat(repeat)
            .delay(delay)
            .paused(paused)
    );
}
void Scheduler::schedule(std::function<void(float)> callback, void *target, float interval, bool paused, const std::string& id)
{
    schedule(
        TimedJob(target, to_id(id), callback)
            .interval(interval)
            .paused(paused)
    );
}
void Scheduler::schedule(SEL_SCHEDULE selector, Ref *target, float interval, unsigned int repeat, float delay, bool paused)
{
    schedule(
        TimedJob(target, to_id(selector), selector)
            .interval(interval)
            .repeat(repeat)
            .delay(delay)
            .paused(paused)
    );
}
void Scheduler::schedule(SEL_SCHEDULE selector, Ref *target, float interval, bool paused)
{
    schedule(
        TimedJob(target, to_id(selector), selector)
            .interval(interval)
            .paused(paused)
    );
}
void Scheduler::unschedule(const std::string& id, void *target)
{
    unscheduleTimedJob(target, to_id(id));
}
void Scheduler::unschedule(SEL_SCHEDULE selector, Ref *target)
{
    unscheduleTimedJob(target, to_id(selector));
}

} // namespace cocos2d

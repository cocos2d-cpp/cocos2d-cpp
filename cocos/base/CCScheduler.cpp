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

#include <limits>

namespace cocos2d {

const UpdateJob::priority_type UpdateJob::SYSTEM_PRIORITY = std::numeric_limits<UpdateJob::priority_type>::min();

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

using update_map       = std::unordered_map<void*,UpdateJob::priority_type>;
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
static timed_iterator findTimedJob(timed_vector & vec, void * target, TimedJob::id_type id)
{
    return std::lower_bound( vec.begin(), vec.end(), TimedJobId{ target, id});
}

inline
static timed_iterator findFirstTimedJobForTarget(timed_vector & vec, void * target)
{
    return findTimedJob(vec, target, std::numeric_limits<TimedJob::id_type>::min());
}

void Scheduler::unscheduleUpdateJob(void *target)
{
    auto iterators = findUpdateJob(_update_target_to_priority, _updateJobs, target);

    if (iterators.second != _updateJobs.end())
    {
        _update_target_to_priority.erase( iterators.first);
        iterators.second->unschedule();
    }

    for (auto & j : _updateJobsToAdd)
    {
        if (j.target() == target)
        {
            j.unschedule();
        }
    }
}

void Scheduler::unscheduleTimedJob(void *target, TimedJob::id_type id)
{
    auto unschedule_in = [target, id] (auto & vec) {

        auto equal = [=](auto & vec, auto lower_bound) {
            return lower_bound != vec.end()
                && lower_bound->target() == target
                && lower_bound->id() == id;
        };

        auto lower_bound = findTimedJob(vec, target, id);

        for ( ; equal(vec, lower_bound); lower_bound++)
            lower_bound->unschedule();
    };

    unschedule_in(_timedJobs);
    unschedule_in(_timedJobsToAdd);
}

template<typename F1, typename F2>
static void for_each(std::vector<UpdateJob> & v, F1 for_system, F2 for_users)
{
    size_t i = 0;
    const size_t size = v.size();

    for ( ; i < size && v[i].priority() == UpdateJob::SYSTEM_PRIORITY; i++)
        for_system(v[i]);

    for ( ; i < size; i++)
        for_users(v[i]);
}

void Scheduler::unscheduleAll()
{
    _update_target_to_priority.clear();

    for_each(
        _updateJobs,
        [=](auto & j) {
            if (! j.unscheduled())
                _update_target_to_priority[ j.target() ] = j.priority();
        },
        [](auto & j) {
            j.unschedule();
        }
    );
    for_each(
        _updateJobsToAdd,
        [](auto &) {},
        [](auto & j) {
            j.unschedule();
        }
    );

    for (auto & job : _timedJobs)
        job.unschedule();
    for (auto & job : _timedJobsToAdd)
        job.unschedule();
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

void Scheduler::pauseAll()
{
    for_each(
        _updateJobs,
        [](auto &) {},
        [](auto & j) {
            j.paused(true);
        }
    );
    for_each(
        _updateJobsToAdd,
        [](auto &) {},
        [](auto & j) {
            j.paused(true);
        }
    );

    updatePausedStateForAll( _timedJobs,       true);
    updatePausedStateForAll( _timedJobsToAdd,  true);
}

void Scheduler::resumeAll()
{
    for_each(
        _updateJobs,
        [](auto &) {},
        [](auto & j) {
            j.paused(false);
        }
    );
    for_each(
        _updateJobsToAdd,
        [](auto &) {},
        [](auto & j) {
            j.paused(false);
        }
    );

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

void Scheduler::pauseAllForTarget(void *target)
{
    updatePausedStateForTarget(target, true);
}

void Scheduler::resumeAllForTarget(void *target)
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
    size_t n_to_erase = 0;
    const size_t size  = v.size();

    for (size_t i = 0; i < size; i++)
    {
        if (! v[i].unscheduled())
        {
            if (! v[i].paused())
            {
                v[i].update(dt);
            }
        }
        else
        {
            n_to_erase++;
        }
    }

    if (n_to_erase)
    {
        v.erase(
            std::remove_if(v.begin(), v.end(),
                           [](auto const& j) { return j.unscheduled(); }),
            v.end()
        );
    }
}

// main loop
void Scheduler::update(float dt)
{
    dt *= _speedup;

    run_and_erase_unscheduled( _updateJobs, dt);
    run_and_erase_unscheduled( _timedJobs, dt);

    // Functions scheeduled from another thread

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

        if (job.id() < 0)
        {
            begin = std::upper_bound(begin, end, job);
            begin = ++_timedJobs.insert(begin, job);
        }
        else
        {
            begin = std::lower_bound(begin, end, job);

            if (begin == end || job < *begin)
                begin = _timedJobs.insert(begin, job);
            else
                *begin = job;
        }
    }

    _timedJobsToAdd.clear();
}

} // namespace cocos2d

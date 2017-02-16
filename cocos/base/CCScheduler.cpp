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

#include "base/CCScheduler.h"

#include "base/ccMacros.h"

namespace cocos2d {

static_assert(sizeof(Job) == sizeof(TimedJob));

void Job::update(Scheduler & scheduler, float dt)
{
    CC_ASSERT(!paused());
    CC_ASSERT(!unscheduled());
    CC_ASSERT(0.0f <= dt);

    if (dt < std::numeric_limits<float>::epsilon())
    {
        return;
    }

    // if _interval == 0, should trigger once every frame
    float interval = (_interval > 0.0f) ? _interval : dt;

    if (_leftover < 0.0f)
    {
        if (_leftover + dt < 0.0f)
        {
            _leftover += dt;
            return;
        }
        
        // first run after delay
        trigger(0.0f);

        if (!_repeat)
        {
            cancel(scheduler);
            return;
        }

        dt += _leftover;
        _leftover = 0.0f;
    }

    for (_leftover += dt; interval <= _leftover; _leftover -= interval)
    {
        trigger(interval);

        if (!forever() && !--_repeat)
        {
            cancel(scheduler);
            return;
        }
    }
}

void Job::trigger(float dt)
{
    if (_callback)
        _callback(dt);
}

void Job::cancel(Scheduler & scheduler)
{
    // Only TimedJobs can be cancelled here
    scheduler.unscheduleTimedJob(static_cast<TimedJob*>(this)->id(), _target);
}
  
// implementation of Scheduler

void Scheduler::unscheduleTimedJob(TimedJob::id_t id, void *target)
{
    JobId jobId(TimedJob::make_properties(id), target);

    auto lb = std::lower_bound(_jobs.begin(), _jobs.end(), jobId);
    
    if (!(*lb < jobId))
    {
        lb->unschedule();
    }
}

void Scheduler::unscheduleUpdate(void *target)
{
    auto low = [](Job const& j, Job::Type type) {
        return j.type() < type;
    };
    auto up = [](Job::Type type, Job const& j) {
        return type < j.type();
    };
    // property is unknown :-(
    auto lb = std::lower_bound(_jobs.begin(), _jobs.end(), Job::UPDATE, low);
    auto ub = std::upper_bound(lb, _jobs.end(), Job::UPDATE, up);
    auto it = std::find_if(lb, ub, [target](Job const& j) { return target == j.target(); });
    if (it != ub) it->unschedule();
}

void Scheduler::unscheduleAll()
{
    for (auto & job : _jobs)
        job.unschedule();

    for (auto & job : _jobsToAdd)
        job.unschedule();
}

void Scheduler::unscheduleAllForTarget(void *target)
{
    for (auto & job : _jobs)
        if (job.target() == target)
            job.unschedule();

    for (auto & job : _jobsToAdd)
        if (job.target() == target)
            job.unschedule();
}

void Scheduler::updatePausedStateForTarget(void *target, bool paused)
{
    CCASSERT(target != nullptr, "target can't be nullptr!");

    for (auto & job : _jobs)
        if (job.target() == target)
            job.paused(paused);

    for (auto & job : _jobsToAdd)
        if (job.target() == target)
            job.paused(paused);
}

void Scheduler::pauseTarget(void *target)
{
    updatePausedStateForTarget(target, true);
}

void Scheduler::resumeTarget(void *target)
{
    updatePausedStateForTarget(target, false);
}

void Scheduler::updatePausedStateForAll(bool paused)
{
    for (auto & job : _jobs)
        job.paused(paused);

    for (auto & job : _jobsToAdd)
        job.paused(paused);
}

void Scheduler::pauseAllTargets()
{
    updatePausedStateForAll(true);
}

void Scheduler::resumeAllTargets()
{
    updatePausedStateForAll(false);
}

void Scheduler::performFunctionInCocosThread(std::function<void()> function)
{
    std::lock_guard<std::mutex> locker(_performMutex);
    _functionsToPerform.push_back(function);
}

// main loop
void Scheduler::update(float dt)
{
    dt *= _speedup;

    size_t n_unscheduled = 0;

    for (auto & job : _jobs)
    {
        // TODO remove_if can be implemented here
        if (job.unscheduled())
        {
            n_unscheduled++;
        }
        else if (!job.paused())
        {
            job.update(*this, dt);
        }
    }

    if (n_unscheduled)
    {
        _jobs.erase
            (
                std::remove_if(_jobs.begin(), _jobs.end(),
                               [](const Job & j){ return j.unscheduled(); }),
                _jobs.end()
            );
    }

    _jobs.reserve(_jobs.size() + _jobsToAdd.size());

    auto begin = _jobs.begin();

    for (auto & job : _jobsToAdd)
    {
        begin = std::lower_bound(begin, _jobs.end(), job);

        if (begin == _jobs.end() || job < *begin)
            begin = _jobs.insert(begin, job);
        else
            *begin = job;
    }

    _jobsToAdd.clear();

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

static TimedJob::id_t to_id(SEL_SCHEDULE selector)
{
    return
        TimedJob::ID_BITMASK &
            static_cast<TimedJob::id_t>(
                reinterpret_cast<size_t>(
                    reinterpret_cast<void*>(selector)
                ));
}
static TimedJob::id_t to_id(std::string const& id)
{
    return TimedJob::ID_BITMASK & static_cast<TimedJob::id_t>(std::hash<std::string>()(id));
}

void Scheduler::schedule(std::function<void(float)> callback, void *target, float interval, unsigned int repeat, float delay, bool paused, const std::string& id)
{
    schedule(
        TimedJob(to_id(id), target, callback)
            .interval(interval)
            .repeat(repeat)
            .delay(delay)
            .paused(paused)
    );
}
void Scheduler::schedule(std::function<void(float)> callback, void *target, float interval, bool paused, const std::string& id)
{
    schedule(
        TimedJob(to_id(id), target, callback)
            .interval(interval)
            .paused(paused)
    );
}
void Scheduler::schedule(SEL_SCHEDULE selector, Ref *target, float interval, unsigned int repeat, float delay, bool paused)
{
    schedule(
        TimedJob(to_id(selector), target, selector)
            .interval(interval)
            .repeat(repeat)
            .delay(delay)
            .paused(paused)
    );
}
void Scheduler::schedule(SEL_SCHEDULE selector, Ref *target, float interval, bool paused)
{
    schedule(
        TimedJob(to_id(selector), target, selector)
            .interval(interval)
            .paused(paused)
    );
}
void Scheduler::unschedule(const std::string& id, void *target)
{
    unscheduleTimedJob(to_id(id), target);
}
void Scheduler::unschedule(SEL_SCHEDULE selector, Ref *target)
{
    unscheduleTimedJob(to_id(selector), target);
}

} // namespace cocos2d

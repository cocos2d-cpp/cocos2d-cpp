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

        if (_interval == 0.0f)
        {
            _leftover = 0.0f;
            return;
        }

        if (_leftover == std::numeric_limits<float>::epsilon())
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

    auto begin = _jobs.begin() + first_timed_idx;
    auto end   = _jobs.end();
    auto lb = std::lower_bound(begin, end, jobId);
    
    if (lb != end && !(jobId < *lb))
    {
        lb->unschedule();
    }
}

void Scheduler::unscheduleUpdate(void *target)
{
    auto begin = _jobs.begin() + first_update_idx;
    auto end   = _jobs.begin() + first_timed_idx;
    auto it = std::find_if(begin, end,
                           [target](Job const& j) {
                               return target == j.target();
                           });
    if (it != end)
    {
        it->unschedule();
    }
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

    auto move_to = _jobs.begin();
    auto end     = _jobs.end();

    for (auto curr = move_to; curr != end; curr++)
    {
        if (!curr->unscheduled())
        {
            if (!curr->paused())
                curr->update(*this, dt);

            if (move_to != curr)
                *move_to = std::move(*curr);

            move_to++;
        }
        else
        {
            first_update_idx -= (curr->type() == Job::ACTION);
            first_timed_idx  -= (curr->type() == Job::ACTION
                                 || curr->type() == Job::UPDATE);
        }
    }

    _jobs.erase(move_to, end);

    _jobs.reserve(_jobs.size() + _jobsToAdd.size());

    auto begin = _jobs.begin();

    auto add_it = _jobsToAdd.begin();
    auto add_end = _jobsToAdd.end();

    for (; add_it != add_end && Job::ACTION == add_it->type(); add_it++)
    {
        CC_ASSERT(false); // not implemented yet

        if (add_it->unscheduled())
            continue;

        //first_update_idx--++;
        //first_timed_idx--++;
    }

    for (; add_it != add_end && Job::UPDATE == add_it->type(); add_it++)
    {
        if (add_it->unscheduled())
            continue;

        auto target = add_it->target();
        begin = _jobs.begin() + first_update_idx;
        end   = _jobs.begin() + first_timed_idx;

        auto it = std::find_if(begin, end,
                               [target](Job const j) {
                                   return target == j.target();
                               });

        if (it != end)
        {
            _jobs.erase(it);
            first_timed_idx--;
            begin = _jobs.begin() + first_update_idx;
            end   = _jobs.begin() + first_timed_idx;
        }

        _jobs.insert(std::upper_bound(begin, end, *add_it),
                     std::move(*add_it));
        first_timed_idx++;
    }

    begin = _jobs.begin() + first_timed_idx;

    for (; add_it != add_end; add_it++)
    {
        if (add_it->unscheduled())
            continue;

        end = _jobs.end();

        begin = std::lower_bound(begin, end, *add_it);
        if (begin == end || *add_it < *begin)
            begin = _jobs.insert(begin, *add_it);
        else
            *begin = *add_it;
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

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

#include <algorithm>

namespace cocos2d {

namespace {
    uint16_t to_id(SEL_SCHEDULE selector)
    {
        return reinterpret_cast<size_t>(reinterpret_cast<void*>(selector));
    }
    uint16_t to_id(std::string const& id)
    {
        return std::hash<std::string>()(id);
    }
}

static_assert(sizeof(Job) == sizeof(TimedJob));

void Job::update(Scheduler & scheduler, float dt)
{
    CC_ASSERT(!paused());
    CC_ASSERT(_repeat <= CC_REPEAT_FOREVER);

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

        if (_interval <= 0.0f)
        {
            _leftover = 0.0f;
        }
    }

    // if _interval == 0, should trigger once every frame
    float interval = (_interval > 0.0f) ? _interval : dt;

    for (_leftover += dt; interval <= _leftover; _leftover -= interval)
    {
        trigger(interval);
        if (!--_repeat)
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
    // TODO Fix
    scheduler.unschedule(_target, static_cast<TimedJob*>(this)->id());
}
  
// implementation of Scheduler

Scheduler::~Scheduler(void)
{
    unscheduleAll();
}

void Scheduler::schedule(TimedJob job)
{
    auto & element = _hashForTimers[job.target()];

    if (!element)
    {
        element.reset(new HashTimerEntry);
        element->paused = static_cast<Job&>(job).paused();
    }
    else
    {
        CCASSERT(element->paused == static_cast<Job&>(job).paused(),
                 "element's paused should be paused!");
    }

    for (auto & j : element->timedJobs)
    {
        if (static_cast<TimedJob*>(j.get())->id() == job.id())
        {
            CCLOG("CCScheduler#schedule. TimedJob already scheduled. Updating");
            *j = std::move(static_cast<Job&&>(job));
            return;
        }        
    }

    element->timedJobs.push_back(
        std::unique_ptr<TimedJob>(
            new TimedJob(job)
        ));
}

void Scheduler::schedule(std::function<void(float)> callback, void *target, float interval, bool paused, const std::string& id)
{
    schedule(
        TimedJob(to_id(id), target, callback)
            .interval(interval)
            .paused(paused)
    );
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

void Scheduler::schedule(SEL_SCHEDULE selector, Ref *target, float interval, unsigned int repeat, float delay, bool paused)
{
    CCASSERT(target, "Argument target must be non-nullptr");
    
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
    CCASSERT(target, "Argument target must be non-nullptr");
    
    schedule(
        TimedJob(to_id(selector), target, selector)
            .interval(interval)
            .paused(paused)
    );
}

void Scheduler::unschedule(void *target, size_t id)
{
    auto element_it = _hashForTimers.find(target);

    if (_hashForTimers.end() == element_it)
    {
        return;
    }

    auto & element = *element_it->second;
    auto & timedJobs = element.timedJobs;

    auto job_it = std::find_if(
        timedJobs.begin(), timedJobs.end(),
        [id](const std::unique_ptr<Job> & p) {
            return id == static_cast<const TimedJob*>(p.get())->id();
        });

    if (job_it == timedJobs.end())
    {
        return;
    }

    if ((! element.currentJobSalvaged)
        && job_it->get() == element.currentJob)
    {
        element.currentJobSalvaged = true;
    }

    // update timerIndex in case we are in tick:, looping over the actions
    if (element.timerIndex >= std::distance(timedJobs.begin(), job_it))
    {
        element.timerIndex--;
    }

    timedJobs.erase(job_it);

    if (timedJobs.empty())
    {
        if (_currentTarget == target)
        {
            _currentTargetSalvaged = true;
        }
        else
        {
            CC_ASSERT(element_it == _hashForTimers.find(target));
            _hashForTimers.erase(element_it);
        }
    }
}

void Scheduler::unschedule(const std::string &id, void *target)
{
    unschedule(target, to_id(id));
}

void Scheduler::unschedule(SEL_SCHEDULE selector, Ref *target)
{
    unschedule(target, to_id(selector));
}

void Scheduler::schedulePerFrame(std::function<void(float)> callback, void *target, int priority, bool paused)
{
    auto hash_it = _hashForUpdates.find(target);

    if (hash_it != _hashForUpdates.end())
    {
        // check if priority has changed
        if (hash_it->second->priority != priority)
        {
            if (_updateHashLocked)
            {
                CC_ASSERT(false);
                CCLOG("warning: you CANNOT change update priority in scheduled function");
                hash_it->second->markedForDeletion = false;
                hash_it->second->paused = paused;
                return;
            }
            else
            {
            	// will be added again outside if (hashElement).
                unscheduleUpdate(hash_it);
            }
        }
        else
        {
            hash_it->second->markedForDeletion = false;
            hash_it->second->paused = paused;
            return;
        }
    }

    auto list_it = std::find_if(_updatesList.begin(), _updatesList.end(),
                                [priority](const ListEntry& e) {
                                    return priority < e.priority;
                                });

    _hashForUpdates[target] = _updatesList.insert(list_it, {callback, target, priority, paused, false});
}

void Scheduler::unscheduleUpdate(void *target)
{
    auto hash_it = _hashForUpdates.find(target);

    if (hash_it != _hashForUpdates.end())
    {
        unscheduleUpdate(hash_it);
    }
}

void Scheduler::unscheduleUpdate(updates_hash_t::iterator hash_it)
{
    if (_updateHashLocked)
    {
        hash_it->second->markedForDeletion = true;
    }
    else
    {
        _updatesList.erase( hash_it->second );
        _hashForUpdates.erase(hash_it);
    }
}

void Scheduler::unscheduleAll()
{
    // Custom Selectors

    std::unique_ptr<HashTimerEntry> currentTargetValue;

    auto it = _hashForTimers.find(_currentTarget);

    if (it != _hashForTimers.end())
    {
        currentTargetValue = std::move(it->second);
        _hashForTimers.erase(it);
    }

    while (_hashForTimers.size())
        unscheduleAllForTarget(_hashForTimers.begin()->first);

    if (currentTargetValue)
    {
        _hashForTimers[_currentTarget] = std::move(currentTargetValue);
        unscheduleAllForTarget(_currentTarget);
    }

    // Updates selectors
    if (_updateHashLocked)
    {
        for (auto & l : _updatesList)
            l.markedForDeletion = true;
    }
    else
    {
        _hashForUpdates.clear();
        _updatesList.clear();
    }
}

void Scheduler::unscheduleAllForTarget(void *target)
{
    // explicit nullptr handling
    if (target == nullptr)
    {
        return;
    }

    auto it = _hashForTimers.find(target);

    if (_hashForTimers.end() != it)
    {
        // Custom Selectors
        if (_currentTarget != target)
        {
            _hashForTimers.erase(it);
        }
        else
        {
            auto & timedJobs = it->second->timedJobs;

            if (! it->second->currentJobSalvaged)
            {
                auto currentJob_it = std::find_if(
                    timedJobs.begin(), timedJobs.end(),
                    [it](const std::unique_ptr<Job> & p) {
                        return p.get() == it->second->currentJob;
                    });
                if (currentJob_it !=  timedJobs.end())
                {
                    it->second->currentJobSalvaged = true;
                }
            }

            timedJobs.clear();

            _currentTargetSalvaged = true;
        }
    }

    // update selector
    unscheduleUpdate(target);
}

void Scheduler::updatePausedState(void *target, bool paused)
{
    CCASSERT(target != nullptr, "target can't be nullptr!");

    // custom selectors
    auto timers_hash_it = _hashForTimers.find(target);
    if (_hashForTimers.end() != timers_hash_it)
    {
        timers_hash_it->second->paused = paused;
        std::for_each(timers_hash_it->second->timedJobs.begin(),
                      timers_hash_it->second->timedJobs.end(),
                      [paused](std::unique_ptr<Job> & p){
                          p->paused(paused);
                      });
    }

    auto updates_hash_it = _hashForUpdates.find(target);
    if (_hashForUpdates.end() != updates_hash_it)
    {
        updates_hash_it->second->paused = paused;
    }
}

void Scheduler::pauseTarget(void *target)
{
    updatePausedState(target, true);
}

void Scheduler::resumeTarget(void *target)
{
    updatePausedState(target, false);
}

bool Scheduler::isTargetPaused(void *target) const
{
    CCASSERT( target != nullptr, "target must be non nil" );

    // Custom selectors
    auto timers_hash_it = _hashForTimers.find(target);
    if (_hashForTimers.end() != timers_hash_it)
    {
        return timers_hash_it->second->paused;
    }
    
    // We should check update selectors if target does not have custom selectors
    auto updates_hash_it = _hashForUpdates.find(target);
    if (updates_hash_it != _hashForUpdates.end())
    {
        return updates_hash_it->second->paused;
    }
    
    CC_ASSERT(false);
    return false;
}

void Scheduler::updatePausedState(bool paused)
{
    // Custom Selectors
    for (auto & pair : _hashForTimers)
    {
        pair.second->paused = paused;
        std::for_each(pair.second->timedJobs.begin(),
                      pair.second->timedJobs.end(),
                      [paused](std::unique_ptr<Job> & p){
                          p->paused(paused);
                      });
    }

    // Updates selectors
    for (auto & entry : _updatesList) 
    {
        entry.paused = paused;
    }
}

void Scheduler::pauseAllTargets()
{
    updatePausedState(true);
}

void Scheduler::resumeAllTargets()
{
    updatePausedState(false);
}

void Scheduler::performFunctionInCocosThread(const std::function<void ()> &function)
{
    std::lock_guard<std::mutex> locker(_performMutex);
    _functionsToPerform.push_back(function);
}

float Scheduler::getSpeedup() const
{
    return _speedup;
}

void Scheduler::setSpeedup(float speedup)
{
    CCASSERT(speedup > std::numeric_limits<float>::epsilon(),
             "speedup must be positive");
    _speedup = speedup;
}

// main loop
void Scheduler::update(float dt)
{
    _updateHashLocked = true;

    dt *= _speedup;

    //
    // Selector callbacks
    //

    for (auto & entry : _updatesList)
    {
        if (!entry.paused && !entry.markedForDeletion)
        {
            entry.callback(dt);
        }
    }

    // Iterate over all the custom selectors
    for (auto it = _hashForTimers.begin(), end = _hashForTimers.end(); it != end; )
    {
        auto target = it->first;
        _currentTarget = it->first;
        _currentTargetSalvaged = false;

        auto elt = it->second.get();

        if (! elt->paused)
        {
            // The 'timedJobs' array may change while inside this loop
            for (elt->timerIndex = 0; elt->timerIndex < static_cast<int>(elt->timedJobs.size()); elt->timerIndex++)
            {
                auto & currentJob = elt->timedJobs[elt->timerIndex];
                elt->currentJob = currentJob.get();
                elt->currentJobSalvaged = false;
                currentJob->update(*this, dt);
            }
        }

        it = _hashForTimers.find(target);

        CC_ASSERT(it != _hashForTimers.end());

        // only delete currentTarget if no actions were scheduled during the cycle (issue #481)
        if (_currentTargetSalvaged && elt->timedJobs.empty())
        {
            CC_ASSERT(it == _hashForTimers.find(target));
            it = _hashForTimers.erase(it);
        }
        else
        {
            it++;
        }
    }

    for (auto it = _updatesList.begin(), end = _updatesList.end(); it != end; )
    {
        if (it->markedForDeletion)
        {
            auto hash_it = _hashForUpdates.find(it->target);
            if (hash_it != _hashForUpdates.end())
            {
                // hash can contain a new element, see schedulePerFrame
                if (hash_it->second->markedForDeletion)
                {
                    _hashForUpdates.erase(hash_it);
                }
            }
            it = _updatesList.erase(it);
        }
        else
        {
            it++;
        }
    }

    _updateHashLocked = false;
    _currentTarget = nullptr;

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

} // namespace cocos2d

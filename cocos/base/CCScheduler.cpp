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

#include "base/ccCArray.h"
#include "base/CCDirector.h"
#include "base/utlist.h"
#include "base/ccMacros.h"

namespace cocos2d {

// data structures

class CC_DLL TimerInt
{
protected:
    TimerInt(Scheduler* scheduler, float interval_in_seconds, unsigned int repeat, float delay)
        : _scheduler(scheduler)
        , _elapsed(-1)
        , _runForever(repeat == CC_REPEAT_FOREVER)
        , _useDelay(delay > 0.0f)
        , _timesExecuted(0)
        , _repeat(repeat)
        , _delay(delay)
        , _interval(interval_in_seconds)
        {}
public:
    /** get interval in seconds */
    float getInterval() const { return _interval; }
    /** set interval in seconds */
    void setInterval(float interval) { _interval = interval; }
    
    virtual void trigger(float dt) = 0;
    virtual void cancel() = 0;
    
    /** triggers the timer */
    void update(float dt)
    {
        if (_elapsed == -1)
        {
            _elapsed = 0;
            _timesExecuted = 0;
            return;
        }

        // accumulate elapsed time
        _elapsed += dt;

        // deal with delay
        if (_useDelay)
        {
            if (_elapsed < _delay)
            {
                return;
            }
            trigger(_delay);
            _elapsed = _elapsed - _delay;
            _timesExecuted++;
            _useDelay = false;
            // after delay, the rest time should compare with interval
            if (!_runForever && _timesExecuted > _repeat)
            {    //unschedule timer
                cancel();
                return;
            }
        }

        // if _interval == 0, should trigger once every frame
        float interval = (_interval > 0) ? _interval : _elapsed;
        while (_elapsed >= interval)
        {
            trigger(interval);
            _elapsed -= interval;
            _timesExecuted++;

            if (!_runForever && _timesExecuted > _repeat)
            {
                cancel();
                break;
            }

            if (_elapsed <= 0.f)
            {
                break;
            }
        }
    }

protected:
    
    Scheduler* _scheduler; // weak ref
    float _elapsed;
    bool _runForever;
    bool _useDelay;
    unsigned int _timesExecuted;
    unsigned int _repeat; //0 = once, 1 is 2 x executed
    float _delay;
    float _interval;
};


class CC_DLL TimerTargetSelector : public TimerInt
{
public:
    TimerTargetSelector(Ref* target, float interval, Scheduler* scheduler, SEL_SCHEDULE selector, unsigned int repeat, float delay)
        : TimerInt(scheduler, interval, repeat, delay)
        , _target(target)
        , _selector(selector)
        {}
    
    SEL_SCHEDULE getSelector() const { return _selector; }
    
    virtual void trigger(float dt) override
    {
        if (_target && _selector)
            (_target->*_selector)(dt);
    }
    virtual void cancel() override
    {
        _scheduler->unschedule(_selector, _target);
    }
    
protected:
    Ref* _target;
    SEL_SCHEDULE _selector;
};

class CC_DLL TimerTargetCallback : public TimerInt
{
public:
    TimerTargetCallback(void *target, float interval, Scheduler* scheduler, std::function<void(float)> callback, const std::string& key, unsigned int repeat, float delay)
        : TimerInt(scheduler, interval, repeat, delay)
        , _target(target)
        , _callback(callback)
        , _key(key)
        {}
    
    const std::function<void(float)>& getCallback() const { return _callback; }
    const std::string& getKey() const { return _key; }
    
    virtual void trigger(float dt) override
    {
        if (_callback)
            _callback(dt);
    }

    virtual void cancel() override
    {
        _scheduler->unschedule(_key, _target);
    }
  
protected:
    void* _target;
    std::function<void(float)> _callback;
    std::string _key;
};

// implementation of Scheduler

// Priority level reserved for system services.
const int Scheduler::PRIORITY_SYSTEM = INT_MIN;

// Minimum priority level for user scheduling.
const int Scheduler::PRIORITY_NON_SYSTEM_MIN = PRIORITY_SYSTEM + 1;

Scheduler::Scheduler(void)
: _timeScale(1.0f)
, _updatesNegList(nullptr)
, _updates0List(nullptr)
, _updatesPosList(nullptr)
, _hashForUpdates(nullptr)
, _hashForTimers()
, _currentTarget(nullptr)
, _currentTargetSalvaged(false)
, _updateHashLocked(false)
{
    // I don't expect to have more than 30 functions to all per frame
    _functionsToPerform.reserve(30);
}

Scheduler::~Scheduler(void)
{
    unscheduleAll();
}

template<typename TimerT, typename F, typename Target, typename ...Args>
void Scheduler::schedule_impl(F match, Target target, bool paused, float interval, Args... args)
{
    auto & element = _hashForTimers[target];

    if (!element)
    {
        element.reset(new tHashTimerEntry);
        element->paused = paused;
    }
    else
    {
        CCASSERT(element->paused == paused, "element's paused should be paused!");
    }

    for (auto & t : element->timers)
    {
        TimerT *timer = dynamic_cast<TimerT*>(t.get());

        if ( match(timer) )
        {
            CCLOG("CCScheduler#scheduleSelector. Selector already scheduled. Updating interval from: %.4f to %.4f", timer->getInterval(), interval);
            timer->setInterval(interval);
            return;
        }        
    }

    element->timers.push_back(
        std::unique_ptr<TimerT>(
            new TimerT(
                target, interval, std::forward<Args>(args)...
            )));
}

void Scheduler::schedule(std::function<void(float)> callback, void *target, float interval, bool paused, const std::string& key)
{
    this->schedule(callback, target, interval, CC_REPEAT_FOREVER, 0.0f, paused, key);
}

void Scheduler::schedule(std::function<void(float)> callback, void *target, float interval, unsigned int repeat, float delay, bool paused, const std::string& key)
{
    CCASSERT(target, "Argument target must be non-nullptr");

    schedule_impl<TimerTargetCallback>(
        [&key] (const TimerTargetCallback *timer) {
            return timer && key == timer->getKey();
        },
        target, paused, interval, this, callback, key, repeat, delay);
}

void Scheduler::schedule(SEL_SCHEDULE selector, Ref *target, float interval, unsigned int repeat, float delay, bool paused)
{
    CCASSERT(target, "Argument target must be non-nullptr");
    
    schedule_impl<TimerTargetSelector>(
        [&selector] (const TimerTargetSelector *timer) {
            return timer && selector == timer->getSelector();
        },
        target, paused, interval, this, selector, repeat, delay);
}

void Scheduler::schedule(SEL_SCHEDULE selector, Ref *target, float interval, bool paused)
{
    this->schedule(selector, target, interval, CC_REPEAT_FOREVER, 0.0f, paused);
}

template<typename F>
void Scheduler::unschedule(void *target, F compareTimers)
{
    auto element_it = _hashForTimers.find(target);

    if (_hashForTimers.end() == element_it)
    {
        return;
    }

    auto & element = *element_it->second;
    auto & timers = element.timers;

    auto timer_it = std::find_if( timers.begin(), timers.end(), compareTimers );

    if (timer_it == timers.end())
    {
        return;
    }

    if ((! element.currentTimerSalvaged)
        && timer_it->get() == element.currentTimer)
    {
        element.currentTimerSalvaged = true;
    }

    // update timerIndex in case we are in tick:, looping over the actions
    if (element.timerIndex >= std::distance(timers.begin(), timer_it))
    {
        element.timerIndex--;
    }

    timers.erase(timer_it);

    if (timers.empty())
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

void Scheduler::unschedule(const std::string &key, void *target)
{
    // explicit handle nil arguments when removing an object
    if (target == nullptr)
    {
        return;
    }
    unschedule(target,
               [&key](const std::unique_ptr<TimerInt> & p) {
                   auto timer = dynamic_cast<const TimerTargetCallback*>(p.get());
                   return timer && key == timer->getKey();
               });
}

void Scheduler::unschedule(SEL_SCHEDULE selector, Ref *target)
{
    if (target == nullptr || selector == nullptr)
    {
        return;
    }

    unschedule(target,
               [&selector](const std::unique_ptr<TimerInt> & p) {
                   auto timer = dynamic_cast<const TimerTargetSelector*>(p.get());
                   return timer && selector == timer->getSelector();
               });
}

void Scheduler::priorityIn(tListEntry **list, std::function<void(float)> callback, void *target, int priority, bool paused)
{
    tListEntry *listElement = new (std::nothrow) tListEntry();

    listElement->callback = callback;
    listElement->target = target;
    listElement->priority = priority;
    listElement->paused = paused;
    listElement->next = listElement->prev = nullptr;
    listElement->markedForDeletion = false;

    // empty list ?
    if (! *list)
    {
        DL_APPEND(*list, listElement);
    }
    else
    {
        bool added = false;

        for (tListEntry *element = *list; element; element = element->next)
        {
            if (priority < element->priority)
            {
                if (element == *list)
                {
                    DL_PREPEND(*list, listElement);
                }
                else
                {
                    listElement->next = element;
                    listElement->prev = element->prev;

                    element->prev->next = listElement;
                    element->prev = listElement;
                }

                added = true;
                break;
            }
        }

        // Not added? priority has the higher value. Append it.
        if (! added)
        {
            DL_APPEND(*list, listElement);
        }
    }

    // update hash entry for quick access
    tHashUpdateEntry *hashElement = (tHashUpdateEntry *)calloc(sizeof(*hashElement), 1);
    hashElement->target = target;
    hashElement->list = list;
    hashElement->entry = listElement;
    HASH_ADD_PTR(_hashForUpdates, target, hashElement);
}

void Scheduler::appendIn(tListEntry **list, std::function<void(float)> callback, void *target, bool paused)
{
    tListEntry *listElement = new (std::nothrow) tListEntry();

    listElement->callback = callback;
    listElement->target = target;
    listElement->paused = paused;
    listElement->priority = 0;
    listElement->markedForDeletion = false;

    DL_APPEND(*list, listElement);

    // update hash entry for quicker access
    tHashUpdateEntry *hashElement = (tHashUpdateEntry *)calloc(sizeof(*hashElement), 1);
    hashElement->target = target;
    hashElement->list = list;
    hashElement->entry = listElement;
    HASH_ADD_PTR(_hashForUpdates, target, hashElement);
}

void Scheduler::schedulePerFrame(std::function<void(float)> callback, void *target, int priority, bool paused)
{
    tHashUpdateEntry *hashElement = nullptr;
    HASH_FIND_PTR(_hashForUpdates, &target, hashElement);
    if (hashElement)
    {
        // check if priority has changed
        if ((*hashElement->list)->priority != priority)
        {
            if (_updateHashLocked)
            {
                CCLOG("warning: you CANNOT change update priority in scheduled function");
                hashElement->entry->markedForDeletion = false;
                hashElement->entry->paused = paused;
                return;
            }
            else
            {
            	// will be added again outside if (hashElement).
                unscheduleUpdate(target);
            }
        }
        else
        {
            hashElement->entry->markedForDeletion = false;
            hashElement->entry->paused = paused;
            return;
        }
    }

    // most of the updates are going to be 0, that's way there
    // is an special list for updates with priority 0
    if (priority == 0)
    {
        appendIn(&_updates0List, callback, target, paused);
    }
    else if (priority < 0)
    {
        priorityIn(&_updatesNegList, callback, target, priority, paused);
    }
    else
    {
        // priority > 0
        priorityIn(&_updatesPosList, callback, target, priority, paused);
    }
}

bool Scheduler::isScheduled(const std::string& key, void *target)
{
    CCASSERT(target, "Argument target must be non-nullptr");
    
    auto element_it = _hashForTimers.find(target);

    if (_hashForTimers.end() != element_it)
    {
        auto & timers = element_it->second->timers;

        for (auto it = timers.begin(), end = timers.end(); it != end; it++)
        {
            TimerTargetCallback *timer = dynamic_cast<TimerTargetCallback*>(it->get());

            if (timer && key == timer->getKey())
            {
                return true;
            }
        }
    }

    return false;
}

void Scheduler::removeUpdateFromHash(tListEntry *entry)
{
    tHashUpdateEntry *element = nullptr;

    HASH_FIND_PTR(_hashForUpdates, &entry->target, element);
    if (element)
    {
        // list entry
        DL_DELETE(*element->list, element->entry);
        CC_SAFE_DELETE(element->entry);

        // hash entry
        HASH_DEL(_hashForUpdates, element);
        free(element);
    }
}

void Scheduler::unscheduleUpdate(void *target)
{
    if (target == nullptr)
    {
        return;
    }

    tHashUpdateEntry *element = nullptr;
    HASH_FIND_PTR(_hashForUpdates, &target, element);
    if (element)
    {
        if (_updateHashLocked)
        {
            element->entry->markedForDeletion = true;
        }
        else
        {
            this->removeUpdateFromHash(element->entry);
        }
    }
}

void Scheduler::unscheduleAll(void)
{
    unscheduleAllWithMinPriority(PRIORITY_SYSTEM);
}

void Scheduler::unscheduleAllWithMinPriority(int minPriority)
{
    // Custom Selectors

    std::unique_ptr<tHashTimerEntry> currentTargetValue;

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
    tListEntry *entry, *tmp;
    if(minPriority < 0)
    {
        DL_FOREACH_SAFE(_updatesNegList, entry, tmp)
        {
            if(entry->priority >= minPriority)
            {
                unscheduleUpdate(entry->target);
            }
        }
    }

    if(minPriority <= 0)
    {
        DL_FOREACH_SAFE(_updates0List, entry, tmp)
        {
            unscheduleUpdate(entry->target);
        }
    }

    DL_FOREACH_SAFE(_updatesPosList, entry, tmp)
    {
        if(entry->priority >= minPriority)
        {
            unscheduleUpdate(entry->target);
        }
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
            auto & timers = it->second->timers;

            if (! it->second->currentTimerSalvaged)
            {
                auto currentTimer_it = std::find_if(timers.begin(), timers.end(),
                                                    [it](const std::unique_ptr<TimerInt> & p) {
                                                        return p.get() == it->second->currentTimer;
                                                    });
                if (currentTimer_it !=  timers.end())
                {
                    it->second->currentTimerSalvaged = true;
                }
            }

            timers.clear();

            _currentTargetSalvaged = true;
        }
    }

    // update selector
    unscheduleUpdate(target);
}

void Scheduler::resumeTarget(void *target)
{
    CCASSERT(target != nullptr, "target can't be nullptr!");

    // custom selectors
    auto it = _hashForTimers.find(target);
    if (_hashForTimers.end() != it)
    {
        it->second->paused = false;
    }

    // update selector
    tHashUpdateEntry *elementUpdate = nullptr;
    HASH_FIND_PTR(_hashForUpdates, &target, elementUpdate);
    if (elementUpdate)
    {
        CCASSERT(elementUpdate->entry != nullptr, "elementUpdate's entry can't be nullptr!");
        elementUpdate->entry->paused = false;
    }
}

void Scheduler::pauseTarget(void *target)
{
    CCASSERT(target != nullptr, "target can't be nullptr!");

    // custom selectors
    auto it = _hashForTimers.find(target);
    if (_hashForTimers.end() != it)
    {
        it->second->paused = true;
    }

    // update selector
    tHashUpdateEntry *elementUpdate = nullptr;
    HASH_FIND_PTR(_hashForUpdates, &target, elementUpdate);
    if (elementUpdate)
    {
        CCASSERT(elementUpdate->entry != nullptr, "elementUpdate's entry can't be nullptr!");
        elementUpdate->entry->paused = true;
    }
}

bool Scheduler::isTargetPaused(void *target)
{
    CCASSERT( target != nullptr, "target must be non nil" );

    // Custom selectors
    auto it = _hashForTimers.find(target);
    if (_hashForTimers.end() != it)
    {
        return it->second->paused;
    }
    
    // We should check update selectors if target does not have custom selectors
	tHashUpdateEntry *elementUpdate = nullptr;
	HASH_FIND_PTR(_hashForUpdates, &target, elementUpdate);
	if ( elementUpdate )
    {
		return elementUpdate->entry->paused;
    }
    
    return false;  // should never get here
}

std::set<void*> Scheduler::pauseAllTargets()
{
    return pauseAllTargetsWithMinPriority(PRIORITY_SYSTEM);
}

std::set<void*> Scheduler::pauseAllTargetsWithMinPriority(int minPriority)
{
    std::set<void*> idsWithSelectors;

    // Custom Selectors
    for (auto & pair : _hashForTimers)
    {
        pair.second->paused = true;
        idsWithSelectors.insert(pair.first);
    }

    // Updates selectors
    tListEntry *entry, *tmp;
    if(minPriority < 0)
    {
        DL_FOREACH_SAFE( _updatesNegList, entry, tmp ) 
        {
            if(entry->priority >= minPriority)
            {
                entry->paused = true;
                idsWithSelectors.insert(entry->target);
            }
        }
    }

    if(minPriority <= 0)
    {
        DL_FOREACH_SAFE( _updates0List, entry, tmp )
        {
            entry->paused = true;
            idsWithSelectors.insert(entry->target);
        }
    }

    DL_FOREACH_SAFE( _updatesPosList, entry, tmp ) 
    {
        if(entry->priority >= minPriority) 
        {
            entry->paused = true;
            idsWithSelectors.insert(entry->target);
        }
    }

    return idsWithSelectors;
}

void Scheduler::resumeTargets(const std::set<void*>& targetsToResume)
{
    for(const auto &obj : targetsToResume) {
        this->resumeTarget(obj);
    }
}

void Scheduler::performFunctionInCocosThread(const std::function<void ()> &function)
{
    _performMutex.lock();

    _functionsToPerform.push_back(function);

    _performMutex.unlock();
}

// main loop
void Scheduler::update(float dt)
{
    _updateHashLocked = true;

    if (_timeScale != 1.0f)
    {
        dt *= _timeScale;
    }

    //
    // Selector callbacks
    //

    // Iterate over all the Updates' selectors
    tListEntry *entry, *tmp;

    // updates with priority < 0
    DL_FOREACH_SAFE(_updatesNegList, entry, tmp)
    {
        if ((! entry->paused) && (! entry->markedForDeletion))
        {
            entry->callback(dt);
        }
    }

    // updates with priority == 0
    DL_FOREACH_SAFE(_updates0List, entry, tmp)
    {
        if ((! entry->paused) && (! entry->markedForDeletion))
        {
            entry->callback(dt);
        }
    }

    // updates with priority > 0
    DL_FOREACH_SAFE(_updatesPosList, entry, tmp)
    {
        if ((! entry->paused) && (! entry->markedForDeletion))
        {
            entry->callback(dt);
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
            // The 'timers' array may change while inside this loop
            for (elt->timerIndex = 0; elt->timerIndex < static_cast<int>(elt->timers.size()); elt->timerIndex++)
            {
                auto & currentTimer = elt->timers[elt->timerIndex];
                elt->currentTimer = currentTimer.get();
                elt->currentTimerSalvaged = false;
                currentTimer->update(dt);
            }
        }

        it = _hashForTimers.find(target);

        CC_ASSERT(it != _hashForTimers.end());

        // only delete currentTarget if no actions were scheduled during the cycle (issue #481)
        if (_currentTargetSalvaged && elt->timers.empty())
        {
            CC_ASSERT(it == _hashForTimers.find(target));
            it = _hashForTimers.erase(it);
        }
        else
        {
            it++;
        }
    }

    // delete all updates that are marked for deletion
    // updates with priority < 0
    DL_FOREACH_SAFE(_updatesNegList, entry, tmp)
    {
        if (entry->markedForDeletion)
        {
            this->removeUpdateFromHash(entry);
        }
    }

    // updates with priority == 0
    DL_FOREACH_SAFE(_updates0List, entry, tmp)
    {
        if (entry->markedForDeletion)
        {
            this->removeUpdateFromHash(entry);
        }
    }

    // updates with priority > 0
    DL_FOREACH_SAFE(_updatesPosList, entry, tmp)
    {
        if (entry->markedForDeletion)
        {
            this->removeUpdateFromHash(entry);
        }
    }

    _updateHashLocked = false;
    _currentTarget = nullptr;

    //
    // Functions allocated from another thread
    //

    // Testing size is faster than locking / unlocking.
    // And almost never there will be functions scheduled to be called.
    if( !_functionsToPerform.empty() ) {
        _performMutex.lock();
        // fixed #4123: Save the callback functions, they must be invoked after '_performMutex.unlock()', otherwise if new functions are added in callback, it will cause thread deadlock.
        auto temp = _functionsToPerform;
        _functionsToPerform.clear();
        _performMutex.unlock();
        for( const auto &function : temp ) {
            function();
        }
        
    }
}

bool Scheduler::isScheduled(SEL_SCHEDULE selector, Ref *target)
{
    CCASSERT(selector, "Argument selector must be non-nullptr");
    CCASSERT(target, "Argument target must be non-nullptr");
    
    auto element_it = _hashForTimers.find(target);

    if (_hashForTimers.end() != element_it)
    {
        auto & timers = element_it->second->timers;

        for (auto it = timers.begin(), end = timers.end(); it != end; it++)
        {
            TimerTargetSelector *timer = dynamic_cast<TimerTargetSelector*>(it->get());

            if (timer && selector == timer->getSelector())
            {
                return true;
            }
        }
    }

    return false;
}

} // namespace cocos2d

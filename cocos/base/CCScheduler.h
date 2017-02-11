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
#include "base/ccMacros.h" // CC_REPEAT_FOREVER
#include "base/uthash.h"

#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <unordered_map>
#include <vector>

#define CC_SCHEDULE_SELECTOR(_SELECTOR) static_cast<cocos2d::SEL_SCHEDULE>(&_SELECTOR)

namespace cocos2d {

class Scheduler;

class TimedJob {
public:
    explicit TimedJob(std::function<void(float)> callback)
        : _callback(callback)
        , _target(nullptr)
        , _interval(0.0f)
        , _repeat(CC_REPEAT_FOREVER)
        , _delay(0.0f)
        , _paused(false)
        , _key()
        , _scheduler(nullptr)
        , _elapsed(-1)
        , _timesExecuted(0)
        , _runForever(true)
        , _useDelay(false)
        {}

    template<typename T>
    TimedJob(T* t, void (T::*f)(float))
        : TimedJob( [t,f](float dt){ (t->*f)(dt); } )
        {}

    TimedJob(TimedJob const&) = default;
    TimedJob& operator=(TimedJob const&) = default;
    TimedJob(TimedJob &&) = default;
    TimedJob& operator=(TimedJob &&) = default;

    TimedJob & callback(std::function<void(float)> v)
    {
        _callback = v;
        return *this;
    }
    TimedJob & target(void* v)
    {
        _target = v;
        return *this;
    }
    TimedJob & interval(float v)
    {
        _interval = v;
        return *this;
    }
    TimedJob & repeat(unsigned int v)
    {
        _repeat = v;
        _runForever = (_repeat == CC_REPEAT_FOREVER);
        return *this;
    }
    TimedJob & delay(float v)
    {
        _delay = v;
        _useDelay = (0.0f < _delay);
        return *this;
    }
    TimedJob & paused(bool v)
    {
        _paused = v;
        return *this;
    }
    TimedJob & key(size_t v)
    {
        _key = v;
        return *this;
    }
    void scheduler(Scheduler* v)
    {
        _scheduler = v;
    }

    void update(float dt);

private:
    void trigger(float dt);
    void cancel();

public:
    std::function<void(float)> _callback;
    void* _target;
    float _interval;
    unsigned int _repeat;
    float _delay;
    bool _paused;
    size_t _key;

private:
    Scheduler* _scheduler;
    float _elapsed;
    unsigned int _timesExecuted;
    bool _runForever;
    bool _useDelay;
};

class Ref;
typedef void (Ref::*SEL_SCHEDULE)(float);

class TimerInt;

struct _listEntry;
struct _hashSelectorEntry;

/** @brief Scheduler is responsible for triggering the scheduled callbacks.
You should not use system timer for your game logic. Instead, use this class.

There are 2 different types of callbacks (selectors):

- update selector: the 'update' selector will be called every frame. You can customize the priority.
- custom selector: A custom selector will be called every frame, or with a custom interval of time

The 'custom selectors' should be avoided when possible. It is faster, and consumes less memory to use the 'update selector'.

*/
class CC_DLL Scheduler
{
public:
    /** Priority level reserved for system services. 
     * @lua NA
     * @js NA
     */
    static const int PRIORITY_SYSTEM;
    
    /** Minimum priority level for user scheduling. 
     * Priority level of user scheduling should bigger then this value.
     *
     * @lua NA
     * @js NA
     */
    static const int PRIORITY_NON_SYSTEM_MIN;
    
    /**
     * Constructor
     *
     * @js ctor
     */
    Scheduler();
    
    /**
     * Destructor
     *
     * @js NA
     * @lua NA
     */
    virtual ~Scheduler();

    /**
     * Gets the time scale of schedule callbacks.
     * @see Scheduler::setTimeScale()
     */
    float getTimeScale() { return _timeScale; }
    /** Modifies the time of all scheduled callbacks.
    You can use this property to create a 'slow motion' or 'fast forward' effect.
    Default is 1.0. To create a 'slow motion' effect, use values below 1.0.
    To create a 'fast forward' effect, use values higher than 1.0.
    @since v0.8
    @warning It will affect EVERY scheduled selector / action.
    */
    void setTimeScale(float timeScale) { _timeScale = timeScale; }

    /** 'update' the scheduler.
     * You should NEVER call this method, unless you know what you are doing.
     * @lua NA
     */
    void update(float dt);

    void schedule(TimedJob timer);
    void unschedule(void* target, size_t key);
    bool isScheduled(void* target, size_t key) const;

    /////////////////////////////////////
    
    // schedule
    
    /** The scheduled method will be called every 'interval' seconds.
     If paused is true, then it won't be called until it is resumed.
     If 'interval' is 0, it will be called every frame, but if so, it's recommended to use 'scheduleUpdate' instead.
     If the 'callback' is already scheduled, then only the interval parameter will be updated without re-scheduling it again.
     repeat let the action be repeated repeat + 1 times, use CC_REPEAT_FOREVER to let the action run continuously
     delay is the amount of time the action will wait before it'll start.
     @param callback The callback function.
     @param target The target of the callback function.
     @param interval The interval to schedule the callback. If the value is 0, then the callback will be scheduled every frame.
     @param repeat repeat+1 times to schedule the callback.
     @param delay Schedule call back after `delay` seconds. If the value is not 0, the first schedule will happen after `delay` seconds.
            But it will only affect first schedule. After first schedule, the delay time is determined by `interval`.
     @param paused Whether or not to pause the schedule.
     @param key The key to identify the callback function, because there is not way to identify a std::function<>.
     @since v3.0
     */
    CC_DEPRECATED_ATTRIBUTE void schedule(std::function<void(float)>, void *target, float interval, unsigned int repeat, float delay, bool paused, const std::string& key);

    /** The scheduled method will be called every 'interval' seconds for ever.
     @param callback The callback function.
     @param target The target of the callback function.
     @param interval The interval to schedule the callback. If the value is 0, then the callback will be scheduled every frame.
     @param paused Whether or not to pause the schedule.
     @param key The key to identify the callback function, because there is not way to identify a std::function<>.
     @since v3.0
     */
    CC_DEPRECATED_ATTRIBUTE void schedule(std::function<void(float)>, void *target, float interval, bool paused, const std::string& key);
    
    
    /** The scheduled method will be called every `interval` seconds.
     If paused is true, then it won't be called until it is resumed.
     If 'interval' is 0, it will be called every frame, but if so, it's recommended to use 'scheduleUpdate' instead.
     If the selector is already scheduled, then only the interval parameter will be updated without re-scheduling it again.
     repeat let the action be repeated repeat + 1 times, use CC_REPEAT_FOREVER to let the action run continuously
     delay is the amount of time the action will wait before it'll start
     
     @param selector The callback function.
     @param target The target of the callback function.
     @param interval The interval to schedule the callback. If the value is 0, then the callback will be scheduled every frame.
     @param repeat repeat+1 times to schedule the callback.
     @param delay Schedule call back after `delay` seconds. If the value is not 0, the first schedule will happen after `delay` seconds.
     But it will only affect first schedule. After first schedule, the delay time is determined by `interval`.
     @param paused Whether or not to pause the schedule.
     @since v3.0
     */
    CC_DEPRECATED_ATTRIBUTE void schedule(SEL_SCHEDULE selector, Ref *target, float interval, unsigned int repeat, float delay, bool paused);
    
    /** The scheduled method will be called every `interval` seconds for ever.
     @param selector The callback function.
     @param target The target of the callback function.
     @param interval The interval to schedule the callback. If the value is 0, then the callback will be scheduled every frame.
     @param paused Whether or not to pause the schedule.
     */
    CC_DEPRECATED_ATTRIBUTE void schedule(SEL_SCHEDULE selector, Ref *target, float interval, bool paused);
    
    /** Schedules the 'update' selector for a given target with a given priority.
     The 'update' selector will be called every frame.
     The lower the priority, the earlier it is called.
     @since v3.0
     @lua NA
     */
    template <class T>
    void scheduleUpdate(T *target, int priority, bool paused)
    {
        this->schedulePerFrame([target](float dt){
            target->update(dt);
        }, target, priority, paused);
    }

    /////////////////////////////////////
    
    // unschedule

    /** Unschedules a callback for a key and a given target.
     If you want to unschedule the 'callbackPerFrame', use unscheduleUpdate.
     @param key The key to identify the callback function, because there is not way to identify a std::function<>.
     @param target The target to be unscheduled.
     @since v3.0
     */
    CC_DEPRECATED_ATTRIBUTE void unschedule(const std::string& key, void *target);

    /** Unschedules a selector for a given target.
     If you want to unschedule the "update", use `unscheudleUpdate()`.
     @param selector The selector that is unscheduled.
     @param target The target of the unscheduled selector.
     @since v3.0
     */
    CC_DEPRECATED_ATTRIBUTE void unschedule(SEL_SCHEDULE selector, Ref *target);
    
    /** Unschedules the update selector for a given target
     @param target The target to be unscheduled.
     @since v0.99.3
     */
    void unscheduleUpdate(void *target);
    
    /** Unschedules all selectors for a given target.
     This also includes the "update" selector.
     @param target The target to be unscheduled.
     @since v0.99.3
     @lua NA
     */
    void unscheduleAllForTarget(void *target);
    
    /** Unschedules all selectors from all targets.
     You should NEVER call this method, unless you know what you are doing.
     @since v0.99.3
     */
    void unscheduleAll();
    
    /** Unschedules all selectors from all targets with a minimum priority.
     You should only call this with `PRIORITY_NON_SYSTEM_MIN` or higher.
     @param minPriority The minimum priority of selector to be unscheduled. Which means, all selectors which
            priority is higher than minPriority will be unscheduled.
     @since v2.0.0
     */
    void unscheduleAllWithMinPriority(int minPriority);
    
    /////////////////////////////////////
    
    // isScheduled
    
    /** Checks whether a callback associated with 'key' and 'target' is scheduled.
     @param key The key to identify the callback function, because there is not way to identify a std::function<>.
     @param target The target of the callback.
     @return True if the specified callback is invoked, false if not.
     @since v3.0.0
     */
    CC_DEPRECATED_ATTRIBUTE bool isScheduled(const std::string& key, void *target);
    
    /** Checks whether a selector for a given target is scheduled.
     @param selector The selector to be checked.
     @param target The target of the callback.
     @return True if the specified selector is invoked, false if not.
     @since v3.0
     */
    CC_DEPRECATED_ATTRIBUTE bool isScheduled(SEL_SCHEDULE selector, Ref *target);
    
    /////////////////////////////////////
    
    /** Pauses the target.
     All scheduled selectors/update for a given target won't be 'ticked' until the target is resumed.
     If the target is not present, nothing happens.
     @param target The target to be paused.
     @since v0.99.3
     */
    void pauseTarget(void *target);

    /** Resumes the target.
     The 'target' will be unpaused, so all schedule selectors/update will be 'ticked' again.
     If the target is not present, nothing happens.
     @param target The target to be resumed.
     @since v0.99.3
     */
    void resumeTarget(void *target);

    /** Returns whether or not the target is paused.
     * @param target The target to be checked.
     * @return True if the target is paused, false if not.
     * @since v1.0.0
     * @lua NA
     */
    bool isTargetPaused(void *target);

    /** Pause all selectors from all targets.
      You should NEVER call this method, unless you know what you are doing.
     @since v2.0.0
      */
    std::set<void*> pauseAllTargets();

    /** Pause all selectors from all targets with a minimum priority.
      You should only call this with PRIORITY_NON_SYSTEM_MIN or higher.
      @param minPriority The minimum priority of selector to be paused. Which means, all selectors which
            priority is higher than minPriority will be paused.
      @since v2.0.0
      */
    std::set<void*> pauseAllTargetsWithMinPriority(int minPriority);

    /** Resume selectors on a set of targets.
     This can be useful for undoing a call to pauseAllSelectors.
     @param targetsToResume The set of targets to be resumed.
     @since v2.0.0
      */
    void resumeTargets(const std::set<void*>& targetsToResume);

    /** Calls a function on the cocos2d thread. Useful when you need to call a cocos2d function from another thread.
     This function is thread safe.
     @param function The function to be run in cocos2d thread.
     @since v3.0
     @js NA
     */
    void performFunctionInCocosThread( const std::function<void()> &function);
    
protected:
    
    typedef struct _listEntry
    {
        struct _listEntry   *prev, *next;
        std::function<void(float)> callback;
        void* target;
        int   priority;
        bool  paused;
        bool  markedForDeletion; // selector will no longer be called and entry will be removed at end of the next tick
    } tListEntry;

    /** Schedules the 'callback' function for a given target with a given priority.
     The 'callback' selector will be called every frame.
     The lower the priority, the earlier it is called.
     @note This method is only for internal use.
     @since v3.0
     @js _schedulePerFrame
     */
    void schedulePerFrame(std::function<void(float)>, void *target, int priority, bool paused);
    
    void removeUpdateFromHash(tListEntry *entry);

    // update specific

    void priorityIn(tListEntry **list, std::function<void(float)>, void *target, int priority, bool paused);
    void appendIn(tListEntry **list, std::function<void(float)>, void *target, bool paused);


    float _timeScale;

    //
    // "updates with priority" stuff
    //
    tListEntry *_updatesNegList;        // list of priority < 0
    tListEntry *_updates0List;            // list priority == 0
    tListEntry *_updatesPosList;        // list priority > 0

    // A list double-linked list used for "updates with priority"
    typedef struct _hashUpdateEntry
    {
        tListEntry          **list; // Which list does it belong to ?
        tListEntry          *entry; // entry in the list
        void                *target;
        std::function<void(float)> callback;
        UT_hash_handle      hh;
    } tHashUpdateEntry;
    struct _hashUpdateEntry *_hashForUpdates; // hash used to fetch quickly the list entries for pause,delete,etc

    // Hash Element used for "selectors with interval"
    typedef struct _hashSelectorEntry
    {
        std::vector<std::unique_ptr<TimedJob>> timedJobs;
        int         timerIndex;
        const void* currentJob;
        bool        currentJobSalvaged;
        bool        paused;
    } tHashTimerEntry;
    // Used for "selectors with interval"
    std::unordered_map<void*,std::unique_ptr<tHashTimerEntry>> _hashForTimers;

    void* _currentTarget;
    bool _currentTargetSalvaged;
    // If true unschedule will not remove anything from a hash. Elements will only be marked for deletion.
    bool _updateHashLocked;
    
    // Used for "perform Function"
    std::vector<std::function<void()>> _functionsToPerform;
    std::mutex _performMutex;
};

// end of base group
/** @} */

} // namespace cocos2d

#endif // __CCSCHEDULER_H__

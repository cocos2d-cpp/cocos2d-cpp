/****************************************************************************
Copyright (c) 2010      Stuart Carnie
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2016 Chukong Technologies Inc.

http://www.cocos2d-x.org

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

#ifndef __SUPPORT_CCPROFILING_H__
#define __SUPPORT_CCPROFILING_H__
/// @cond DO_NOT_SHOW

#include "base/ccConfig.h"
#include "base/CCRef.h"

#include <chrono>
#include <map>
#include <memory>
#include <string>

namespace cocos2d {

/**
 * @addtogroup global
 * @{
 */

class ProfilingTimer;

/** Profiler
 cocos2d builtin profiler.

 To use it, enable set the CC_ENABLE_PROFILERS=1 in the ccConfig.h file
 */

class CC_DLL Profiler : public Ref
{
public:
    ~Profiler(void);
    void displayTimers(void);
    bool init(void);

public:
    static Profiler* getInstance(void);

    ProfilingTimer* createAndAddTimerWithName(const char* timerName);

    void releaseTimer(const char* timerName);

    void releaseAllTimers();

    std::map<std::string, std::unique_ptr<ProfilingTimer>> _activeTimers;
};

class ProfilingTimer
{
public:
    ProfilingTimer();
    
    ~ProfilingTimer(void);
    
    bool initWithName(const char* timerName);

    virtual std::string getDescription() const;

    const std::chrono::high_resolution_clock::time_point& getStartTime() { return _startTime; }

    /** resets the timer properties
     */
    void reset();

    std::string _nameStr;
    std::chrono::high_resolution_clock::time_point _startTime;
    long _averageTime1;
    long _averageTime2;
    long minTime;
    long maxTime;
    long totalTime;
    long numberOfCalls;
};

extern void CC_DLL ProfilingBeginTimingBlock(const char *timerName);
extern void CC_DLL ProfilingEndTimingBlock(const char *timerName);
extern void CC_DLL ProfilingResetTimingBlock(const char *timerName);

/*
 * cocos2d profiling categories
 * used to enable / disable profilers with granularity
 */

extern bool kProfilerCategorySprite;
extern bool kProfilerCategoryBatchSprite;
extern bool kProfilerCategoryParticles;

// end of global group
/// @}

} // namespace cocos2d

/// @endcond
#endif // __SUPPORT_CCPROFILING_H__

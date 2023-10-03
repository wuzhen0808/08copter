#pragma once
#include "a8/util/Rate.h"
#include "a8/util/Callback.h"
#include "a8/util/Runnable.h"
#include "a8/util/thread/defines.h"
#include "a8/util/thread/Thread.h"
#include "a8/util/thread/Timer.h"

namespace a8::util::thread {

class Scheduler {
public:
    Scheduler(){}
    ~Scheduler(){}
    
    virtual void startSchedule() = 0;
    virtual void endSchedule() = 0;
    virtual Thread *schedule(FuncType::run run, void * context) = 0;
    virtual Timer *scheduleTimer(FuncType::run run, void * context, const Rate& ticks) = 0;

    virtual Thread *getCurrentThread() = 0;
};
} // namespace a8::util

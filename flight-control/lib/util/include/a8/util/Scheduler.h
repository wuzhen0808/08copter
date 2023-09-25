#pragma once
#include "a8/util/Rate.h"
#include "a8/util/Callback.h"
#include "a8/util/Runnable.h"
#include "a8/util/Thread.h"
#include "a8/util/Timer.h"

namespace a8::util {

class Scheduler {
public:
    Scheduler();
    ~Scheduler();
    virtual Timer *tmpTimer() = 0;
    virtual void startSchedule() = 0;
    virtual void endSchedule() = 0;
    virtual Thread *schedule(Runnable *runnable) = 0;

    virtual Timer *scheduleTimer(Callback *callback, const Rate& ticks) = 0;

    virtual Thread *getCurrentThread() = 0;
};
} // namespace a8::util

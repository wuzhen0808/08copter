#pragma once
#include "a8/util/Callback.h"
#include "a8/util/Rate.h"
#include "a8/util/Runnable.h"
#include "a8/util/schedule/Thread.h"
#include "a8/util/schedule/Timer.h"
#include "a8/util/schedule/defines.h"

namespace a8::util::schedule {
using run = void (*)(void *);

class Scheduler {
public:
    Scheduler() {}
    ~Scheduler() {}

    virtual void startSchedule() = 0;
    virtual void endSchedule() = 0;
    virtual Thread *schedule(schedule::run run, void *context) = 0;
    virtual Thread *schedule(void *context, schedule::run run) = 0;
    virtual Timer *scheduleTimer(schedule::run run, void *context, const Rate &ticks) = 0;
    virtual Timer *scheduleTimer(const Rate &ticks, void *context, schedule::run run) = 0;
};
} // namespace a8::util::schedule

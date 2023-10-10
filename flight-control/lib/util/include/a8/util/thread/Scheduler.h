#pragma once
#include "a8/util/Callback.h"
#include "a8/util/Rate.h"
#include "a8/util/Runnable.h"
#include "a8/util/thread/Thread.h"
#include "a8/util/thread/Timer.h"
#include "a8/util/thread/defines.h"

namespace a8::util::thread {
using run = void (*)(void *);

class Scheduler {
public:
    Scheduler() {}
    ~Scheduler() {}

    virtual void startSchedule() = 0;
    virtual void endSchedule() = 0;
    virtual Thread *schedule(thread::run run, void *context) = 0;
    virtual Thread *schedule(void *context, thread::run run) = 0;
    virtual Timer *scheduleTimer(thread::run run, void *context, const Rate &ticks) = 0;
    virtual Timer *scheduleTimer(const Rate &ticks, void *context, thread::run run) = 0;
};
} // namespace a8::util::thread

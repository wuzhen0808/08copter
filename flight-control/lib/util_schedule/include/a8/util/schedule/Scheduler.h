#pragma once
#include "a8/util/Callback.h"
#include "a8/util/Rate.h"
#include "a8/util/Runnable.h"
#include "a8/util/schedule/Semaphore.h"
#include "a8/util/schedule/Thread.h"
#include "a8/util/schedule/Timer.h"
#include "a8/util/schedule/Lock.h"
#include "a8/util/schedule/SyncQueue.h"

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

    Thread *schedule(void *context, schedule::run run) {
        return schedule(run, context);
    }

    virtual Timer *scheduleTimer(schedule::run run, void *context, const Rate &rate) = 0;
    Timer *scheduleTimer(const Rate &rate, void *context, schedule::run run) {
        return scheduleTimer(run, context, rate);
    }

    virtual SyncQueue * createSyncQueue(int cap) = 0;

    virtual Semaphore *createSemaphore(int cap, int initial) = 0;

    Lock *createLock() {
        Semaphore *sem = this->createSemaphore(1, 1);
        return new Lock(sem);
    }


};
} // namespace a8::util::schedule

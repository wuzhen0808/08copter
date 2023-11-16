#pragma once
#include "a8/hal/freertos/FreeRtosTimer.h"
#include "a8/util/sched.h"

namespace a8::hal::freertos {
using namespace a8::util;
using namespace a8::util::sched;

class FreeRtosScheduler : public Scheduler {
protected:
    SyncQueue<void *> *doCreateSyncQueue(int cap, int itemSize) override;

public:
    FreeRtosScheduler() : Scheduler() {
    }

    ~FreeRtosScheduler() {
    }

    void startSchedule() override {
        vTaskStartScheduler();
    }

    void endSchedule() override {
        vTaskEndScheduler();
    }

    Thread *schedule(void *context, sched::run run) override;

    Timer *scheduleTimer(sched::run run, void *context, const Rate &rate) override;

    Semaphore *createSemaphore(int cap, int initial) override;
};

} // namespace a8::hal::freertos
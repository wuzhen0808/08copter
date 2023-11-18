#pragma once
#include "a8/hal/esp/FreeRtosTimer.h"
#include "a8/util/sched.h"

namespace a8::hal::esp {
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

    // ESP32 already has a freeRtos running.
    void startSchedule() override {
        // vTaskStartScheduler();
    }
    // ESP32 already has a freeRtos running.
    void endSchedule() override {
        // vTaskEndScheduler();
    }

    Thread *createTask(const String name, void *context, sched::run run) override;

    Timer *createTimer(const String name, const Rate &rate, void *context, sched::run run) override;

    Semaphore *createSemaphore(int cap, int initial) override;
};

} // namespace a8::hal::esp
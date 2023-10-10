#pragma once

#include "a8/hal/freertos/FreeRtosScheduler.h"
#include "a8/hal/freertos/FreeRtosThread.h"
#include "a8/hal/freertos/FreeRtosTimer.h"
#include "a8/util/thread.h"

using namespace a8::util;
using namespace a8::util::thread;

namespace a8::hal::freertos {

class FreeRtosScheduler : public Scheduler {
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

    Thread *schedule(void *context, thread::run run) override {
        return FreeRtosThread::start(run, context);
    }

    Thread *schedule(thread::run run, void *context) override {
        return FreeRtosThread::start(run, context);
    }
    Timer *scheduleTimer(const Rate &rate, void *context, thread::run run) override {
        return FreeRtosTimer::start(run, context, rate);
    }
    Timer *scheduleTimer(thread::run run, void *context, const Rate &rate) override {
        return FreeRtosTimer::start(run, context, rate);
    }
};

} // namespace a8::hal::freertos
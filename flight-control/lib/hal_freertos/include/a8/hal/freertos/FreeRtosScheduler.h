#pragma once

#include "a8/hal/freertos/FreeRtosScheduler.h"
#include "a8/hal/freertos/FreeRtosThread.h"
#include "a8/hal/freertos/FreeRtosTimer.h"
#include "a8/util/schedule.h"

namespace a8::hal::freertos {
using namespace a8::util;
using namespace a8::util::schedule;


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

    Thread *schedule(void *context, schedule::run run) override {
        return FreeRtosThread::start(run, context);
    }

    Thread *schedule(schedule::run run, void *context) override {
        return FreeRtosThread::start(run, context);
    }
    Timer *scheduleTimer(const Rate &rate, void *context, schedule::run run) override {
        return FreeRtosTimer::start(run, context, rate);
    }
    Timer *scheduleTimer(schedule::run run, void *context, const Rate &rate) override {
        return FreeRtosTimer::start(run, context, rate);
    }
};

} // namespace a8::hal::freertos
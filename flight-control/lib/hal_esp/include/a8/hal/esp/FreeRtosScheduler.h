#pragma once

#include "a8/hal/esp/FreeRtosScheduler.h"
#include "a8/hal/esp/FreeRtosThread.h"
#include "a8/hal/esp/FreeRtosTimer.h"
#include "a8/util/schedule.h"

namespace a8::hal::esp {
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
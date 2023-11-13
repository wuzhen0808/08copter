#pragma once
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

    Thread *schedule(schedule::run run, void *context) override;

    Timer *scheduleTimer(schedule::run run, void *context, const Rate &rate) override;
};

} // namespace a8::hal::freertos
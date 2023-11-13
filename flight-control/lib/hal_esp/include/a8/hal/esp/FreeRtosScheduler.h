#pragma once
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

    // ESP32 already has a freeRtos running.
    void startSchedule() override {
        // vTaskStartScheduler();
    }
    // ESP32 already has a freeRtos running.
    void endSchedule() override {
        // vTaskEndScheduler();
    }

    Thread *schedule(schedule::run run, void *context) override;

    Timer *scheduleTimer(schedule::run run, void *context, const Rate &rate) override ;
};

} // namespace a8::hal::esp
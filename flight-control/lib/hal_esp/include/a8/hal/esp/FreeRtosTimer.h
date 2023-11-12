#pragma once
#include "a8/util.h"
#include "a8/util/schedule.h"
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

namespace a8::hal::esp {
using namespace a8::util;
using namespace a8::util::schedule;

class FreeRtosTimer : public Timer {
private:
    Rate rate;
    schedule::run run_;
    void *context;
    TimerHandle_t handle;

public:
    FreeRtosTimer(schedule::run run, void *context, const Rate rate) {
        this->rate = rate;
        this->context = context;
        this->run_ = run;
    }

    ~FreeRtosTimer() {
    }
    /**
     * create thread
     */
    void run() {
        this->run_(this->context);
    }
    void setHandle(TimerHandle_t handle) {
        this->handle = handle;
    }
};

} // namespace a8::hal::esp

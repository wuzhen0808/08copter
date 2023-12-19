#pragma once
#include "a8/util.h"
#include "a8/util/sched.h"
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>

namespace a8::hal::esp {
using namespace a8::util;
using namespace a8::util::sched;

class FreeRtosTimer : public Timer {
private:
    Rate rate;
    sched::run run_;
    void *context;
    TimerHandle_t handle;

public:
    FreeRtosTimer(sched::run run, void *context, const Rate rate) {
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
    int close() override {
        long timeoutSec = 1;
        long ticks = configTICK_RATE_HZ * timeoutSec;

        BaseType_t ok = xTimerDelete(this->handle, ticks);
        if (ok == pdFALSE) {
            return -1;
        }
        return 1;
    }
};

} // namespace a8::hal::esp

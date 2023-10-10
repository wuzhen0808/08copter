#pragma once
#include "a8/util.h"
#include "a8/util/thread.h"
#include <FreeRTOS.h>
#include <timers.h>
// see the FreeRTOSConfig.h
#define TICK_RATE_ (50)

using namespace a8::util;
using namespace a8::util::thread;

namespace a8::hal::freertos {
class FreeRtosTimer : public Timer {
private:
    static void timerCallbackFunction(TimerHandle_t handle) {
        void *timerId = pvTimerGetTimerID(handle);
        FreeRtosTimer *timer = static_cast<FreeRtosTimer *>(timerId);
        timer->run_(timer->context);
    }

    Rate rate;
    run run_;
    void *context;

    TimerHandle_t handle;

public:
    static Timer *start(run run, void *context, const Rate &rate) {
        Timer *timer = new FreeRtosTimer(run, context, rate);
        timer->start();
        return timer;
    }

    FreeRtosTimer(run run, void *context, const Rate rate) {
        this->rate = rate;
        this->context = context;
        this->run_ = run;
        long ticks = TICK_RATE_ / rate.Hz();
        handle = xTimerCreate(
            static_cast<const char *>("My Timer"), // name
            ticks,                                 /*  */
            true,                                  /*  */
            this,                                  // timer id
            timerCallbackFunction);
    }

    ~FreeRtosTimer() {
    }
    /**
     * create thread
     */
    Timer *start() {

        BaseType_t isOk = xTimerStart(handle, 0);
        if (isOk != pdPASS) {
            return 0;
        }
        return this;
    }
};

} // namespace a8::hal::freertos

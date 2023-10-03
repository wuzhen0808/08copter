#pragma once
#include "a8/util.h"
#include "a8/util/thread.h"
#include <FreeRTOS.h>
#include <timers.h>

using namespace a8::util;
using namespace a8::util::thread;

namespace a8::hal::freertos {

class FreeRtosTimer : public Timer {
private:
    static void timerCallbackFunction(TimerHandle_t handle) {
        void *timerId = pvTimerGetTimerID(handle);
        FreeRtosTimer *timer = static_cast<FreeRtosTimer *>(timerId);
        timer->run(timer->context);
    }

    Rate rate;
    thread::FuncType::run run;
    void *context;

    TimerHandle_t handle;

public:
    static Timer *start(thread::FuncType::run run, void *context, const Rate &rate) {
        Timer *timer = new FreeRtosTimer(run, context, rate);
        timer->start();
        return timer;
    }

    FreeRtosTimer(thread::FuncType::run run, void *context, const Rate rate) {
        this->rate = rate;
        this->context = context;
        this->run = run;
        long ticks = rate.ms();
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

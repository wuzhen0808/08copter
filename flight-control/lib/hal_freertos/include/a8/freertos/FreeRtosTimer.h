#pragma once
#include "a8/util/Timer.h"
#include <FreeRTOS.h>
#include <timers.h>

using namespace a8::util;

namespace a8::freertos {

class FreeRtosTimer : public Timer {
public:
    FreeRtosTimer(Callback *callback, long ticks);
    ~FreeRtosTimer();
    virtual Timer *start();
    static Timer *start(Callback *callback, long ticks);

private:
    Callback *callback;
    long ticks;
    TimerHandle_t handle;
    static void timerCallbackFunction(TimerHandle_t handle);
};

} // namespace a8::freertos

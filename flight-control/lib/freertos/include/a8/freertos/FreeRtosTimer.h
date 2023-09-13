#pragma once
#include "a8/core/Timer.h"
#include <FreeRTOS.h>
#include <timers.h>
namespace a8::freertos {
using namespace a8::core;

class FreeRtosTimer : public Timer {
public:
    FreeRtosTimer(Callback *callback, ulong ticks);
    ~FreeRtosTimer();
    virtual Timer *start();
    static Timer *start(Callback *callback, ulong ticks);

private:
    Callback *callback;
    ulong ticks;
    TimerHandle_t handle;
    static void timerCallbackFunction(TimerHandle_t handle);
};

} // namespace a8::freertos

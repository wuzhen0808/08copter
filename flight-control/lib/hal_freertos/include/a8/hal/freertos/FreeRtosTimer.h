#pragma once
#include "a8/util/Timer.h"
#include "a8/util/Rate.h"
#include <FreeRTOS.h>
#include <timers.h>

using namespace a8::util;

namespace a8::hal::freertos {

class FreeRtosTimer : public Timer {
public:
    FreeRtosTimer(Callback *callback, const Rate & rate);
    ~FreeRtosTimer();
    virtual Timer *start();
    static Timer *start(Callback *callback, const Rate & rate);

private:
    Callback *callback;
    Rate rate;
    TimerHandle_t handle;
    static void timerCallbackFunction(TimerHandle_t handle);
};

} // namespace a8::hal::freertos

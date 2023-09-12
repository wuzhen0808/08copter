#pragma once
#include "a8/core/Timer.hpp"
#include <Arduino_FreeRTOS.h>
namespace a8 {
namespace arduino {
using namespace a8::core;

class ArduinoTimer : public Timer {
public:
    ArduinoTimer(Callback *callback, ulong ticks);
    ~ArduinoTimer();
    virtual Timer *start();
    static Timer *start(Callback *callback, ulong ticks);

private:
    Callback *callback;
    ulong ticks;
    TimerHandle_t handle;
    static void timerCallbackFunction(TimerHandle_t handle);
};
} // namespace ardui
} // namespace a8

#ifndef A8_TIMER__
#define A8_TIMER__
#include "a8/rtos/RtOs.hpp"
#include "a8/rtos/Callback.hpp"
#include <Arduino_FreeRTOS.h>

namespace a8 {
namespace rtos {
class Timer {
public:
    Timer(Callback *callback, ulong  ticks);
    ~Timer();
    Timer *start();
// static
    static Timer *start(Callback *callback, ulong ticks) {
        Timer *timer = new Timer(callback, ticks);
        return timer->start();
    };
private:
    Callback *callback;
    ulong  ticks;    
    TimerHandle_t handle;
    static void timerCallbackFunction(TimerHandle_t handle);    
};
} // namespace rtos
} // namespace a8

#endif
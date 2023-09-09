#include "a8/rtos/Timer.hpp"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#define LOCAL_THIS 0
#define LOCAL_LAST_WAKE 1
namespace a8 {
namespace rtos {
Timer::Timer(Callback *pvRunnable, ulong ticks) : callback(pvRunnable),
                                                  ticks(ticks) {
    handle = xTimerCreate(
        static_cast<const char *>("My Timer"), // name
        ticks,                                 /*  */
        true,                                  /*  */
        this,                                  // timer id
        timerCallbackFunction);

}

Timer::~Timer() {
}
/**
 * create thread
 */
Timer *Timer::start() {

    BaseType_t isOk = xTimerStart(handle, 0);
    if (isOk != pdPASS) {
        return 0;
    }
    return this;
}

void Timer::timerCallbackFunction(TimerHandle_t handle) {
    void *timerId = pvTimerGetTimerID(handle);
    Timer *timer = static_cast<Timer *>(timerId);
    timer->callback->call();
}

} // namespace rtos
} // namespace a8
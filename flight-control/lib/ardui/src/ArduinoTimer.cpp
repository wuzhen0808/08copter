
#include "a8/ardui/ArduinoTimer.hpp"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#define LOCAL_THIS 0
#define LOCAL_LAST_WAKE 1
namespace a8 {
namespace ardui {
ArduinoTimer::ArduinoTimer(Callback *pvRunnable, ulong ticks) : callback(pvRunnable),
                                                                ticks(ticks) {
    handle = xTimerCreate(
        static_cast<const char *>("My Timer"), // name
        ticks,                                 /*  */
        true,                                  /*  */
        this,                                  // timer id
        timerCallbackFunction);
}

ArduinoTimer::~ArduinoTimer() {
}
Timer *ArduinoTimer::start(Callback *callback, ulong ticks) {
    Timer* timer = new ArduinoTimer(callback, ticks);
    timer->start();
    return timer;
}
/**
 * create thread
 */
Timer *ArduinoTimer::start() {

    BaseType_t isOk = xTimerStart(handle, 0);
    if (isOk != pdPASS) {
        return 0;
    }
    return this;
}

void ArduinoTimer::timerCallbackFunction(TimerHandle_t handle) {
    void *timerId = pvTimerGetTimerID(handle);
    ArduinoTimer *timer = static_cast<ArduinoTimer *>(timerId);
    timer->callback->call();
}

} // namespace ardui
} // namespace a8
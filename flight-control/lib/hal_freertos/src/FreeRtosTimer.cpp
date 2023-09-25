
#include "a8/freertos/FreeRtosTimer.h"

#define LOCAL_THIS 0
#define LOCAL_LAST_WAKE 1

namespace a8::freertos {
FreeRtosTimer::FreeRtosTimer(Callback *pvRunnable, const Rate &rate) : callback(pvRunnable),
                                                                       rate(rate) {

    long ticks = rate.ms();
    handle = xTimerCreate(
        static_cast<const char *>("My Timer"), // name
        ticks,                                 /*  */
        true,                                  /*  */
        this,                                  // timer id
        timerCallbackFunction);
}

FreeRtosTimer::~FreeRtosTimer() {
}
Timer *FreeRtosTimer::start(Callback *callback, const Rate &rate) {
    Timer *timer = new FreeRtosTimer(callback, rate);
    timer->start();
    return timer;
}
/**
 * create thread
 */
Timer *FreeRtosTimer::start() {

    BaseType_t isOk = xTimerStart(handle, 0);
    if (isOk != pdPASS) {
        return 0;
    }
    return this;
}

void FreeRtosTimer::timerCallbackFunction(TimerHandle_t handle) {
    void *timerId = pvTimerGetTimerID(handle);
    FreeRtosTimer *timer = static_cast<FreeRtosTimer *>(timerId);
    timer->callback->call();
}

} // namespace a8::freertos

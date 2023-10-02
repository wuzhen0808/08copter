#include "a8/hal/freertos/FreeRtosScheduler.h"
#include "a8/hal/freertos/FreeRtosThread.h"
#include "a8/hal/freertos/FreeRtosTimer.h"

namespace a8::hal::freertos {

FreeRtosScheduler::FreeRtosScheduler() : Scheduler() {
}

FreeRtosScheduler::~FreeRtosScheduler() {
}

void FreeRtosScheduler::startSchedule() {
    vTaskStartScheduler();
}
void FreeRtosScheduler::endSchedule() {
    vTaskEndScheduler();
}

Thread *FreeRtosScheduler::schedule(Runnable *runnable) {
    return FreeRtosThread::start(runnable);
}

Timer *FreeRtosScheduler::scheduleTimer(Callback *callback, const Rate& rate) {
    return FreeRtosTimer::start(callback, rate);
}

Thread *FreeRtosScheduler::getCurrentThread() {
    return FreeRtosThread::getCurrentThread();
}

} // namespace a8::hal::freertos
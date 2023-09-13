#include "a8/freertos/FreeRtosScheduler.h"
#include "a8/freertos/FreeRtosThread.h"
#include "a8/freertos/FreeRtosTimer.h"
namespace a8::freertos {
using namespace a8::core;

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

Timer *FreeRtosScheduler::scheduleTimer(Callback *callback, ulong ticks) {
    return FreeRtosTimer::start(callback, ticks);
}

Thread *FreeRtosScheduler::getCurrentThread() {
    return FreeRtosThread::getCurrentThread();
}

} // namespace a8::freertos
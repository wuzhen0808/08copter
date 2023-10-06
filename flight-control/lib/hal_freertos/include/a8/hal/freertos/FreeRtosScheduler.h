#pragma once

#include "a8/hal/freertos/FreeRtosScheduler.h"
#include "a8/hal/freertos/FreeRtosThread.h"
#include "a8/hal/freertos/FreeRtosTimer.h"
#include "a8/util/thread.h"

using namespace a8::util;
using namespace a8::util::thread;

namespace a8::hal::freertos {

class FreeRtosScheduler : public Scheduler {
public:
    FreeRtosScheduler() : Scheduler() {
    }

    ~FreeRtosScheduler() {
    }

    void startSchedule() override {
        vTaskStartScheduler();
    }
    void endSchedule() override {
        vTaskEndScheduler();
    }

    Thread *schedule(void (*run)(void*), void * context) override {
        return FreeRtosThread::start(run, context);
    }

    Timer *scheduleTimer(thread::FuncType::run runF, void *context, const Rate &rate) override {
        return FreeRtosTimer::start(runF, context, rate);
    }

};

} // namespace a8::hal::freertos
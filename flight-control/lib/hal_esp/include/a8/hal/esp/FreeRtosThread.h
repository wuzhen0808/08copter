#pragma once
#include "a8/util.h"
#include "a8/util/schedule.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace a8::hal::esp {
using namespace a8::util;
using namespace a8::util::schedule;

class FreeRtosThread : public Thread {
private:
    schedule::run run_;
    void *context;
    TaskHandle_t handle;

public:
    FreeRtosThread(schedule::run runF, void *contextF) {
        this->run_ = runF;
        this->context = contextF;
    }

    ~FreeRtosThread() {
    }

    void run() {
        this->run_(this->context);
    }
    TaskHandle_t getHandle() {
        return this->handle;
    }
    void setHandle(TaskHandle_t handle) {
        this->handle = handle;
    }
};

} // namespace a8::hal::esp

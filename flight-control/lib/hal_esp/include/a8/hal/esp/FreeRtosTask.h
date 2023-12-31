#pragma once
#include "a8/util.h"
#include "a8/util/sched.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace a8::hal::esp {
using namespace a8::util;
using namespace a8::util::sched;

class FreeRtosTask : public Task {
private:
    sched::run run_;
    void *context;
    TaskHandle_t handle;

public:
    FreeRtosTask(sched::run runF, void *contextF) {
        this->run_ = runF;
        this->context = contextF;
    }

    ~FreeRtosTask() {
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

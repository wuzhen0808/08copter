#pragma once
#include "FreeRTOS.h"
#include "a8/util.h"
#include "a8/util/schedule.h"
#include "semphr.h"

namespace a8::hal::freertos {
using namespace a8::util;
using namespace a8::util::schedule;

class FreeRtosSemaphore : public Semaphore {
private:
    SemaphoreHandle_t handle;

public:
    FreeRtosSemaphore(SemaphoreHandle_t handle) {
        this->handle = handle;
    }

    ~FreeRtosSemaphore() {
    }

    int give(Result &res) override {
        BaseType_t ret = xSemaphoreGive(handle);
        if (ret != pdTRUE) {
            res << "failed to give semaphore.";
            return -1;
        }
        return 1;
    }

    int take(long timeout, Result &res) override {
        long ticks = timeout / portTICK_PERIOD_MS;
        BaseType_t ret = xSemaphoreTake(handle, ticks);
        if (ret != pdTRUE) {
            return 0;
        }
        return 1;
    }
};

} // namespace a8::hal::freertos

#pragma once
#include "a8/util.h"
#include "a8/util/sched.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

namespace a8::hal::esp {
using namespace a8::util;
using namespace a8::util::sched;

class FreeRtosSyncQueue : public SyncQueue<void *> {
private:
    QueueHandle_t handle;

public:
    FreeRtosSyncQueue(QueueHandle_t handle) {
        this->handle = handle;
    }

    ~FreeRtosSyncQueue() {        
        vQueueDelete(this->handle);        
    }

    int offer(void *eleP, long timeout) override {
        long ticks = timeout / portTICK_PERIOD_MS;
        BaseType_t ret = xQueueSend(handle, eleP, ticks);
        if (ret != pdTRUE) {
            return -1;
        }
        return 1;
    }

    int take(void *&eleP, long timeout) override {
        long ticks = timeout / portTICK_PERIOD_MS;
        BaseType_t ret = xQueueReceive(handle, eleP, ticks);
        if (ret != pdTRUE) {
            return -1;
        }
        return 1;
    }
    
    int peek(void *&eleP,long timeout) override{        
        long ticks = timeout / portTICK_PERIOD_MS;
        BaseType_t ret = xQueuePeek(handle, eleP, ticks);
        if (ret != pdTRUE) {
            return -1;
        }
        return 1;
    }
};

} // namespace a8::hal::esp

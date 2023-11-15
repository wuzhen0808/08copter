#pragma once
#include "a8/util.h"
#include "a8/util/schedule.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

namespace a8::hal::esp {
using namespace a8::util;
using namespace a8::util::schedule;

class FreeRtosSyncQueue : public SyncQueue {
private:
    QueueHandle_t handle;

public:
    FreeRtosSyncQueue(QueueHandle_t handle) {
        this->handle = handle;
    }

    ~FreeRtosSyncQueue() {
    }

    int offer(void *ele, long timeout) {
        long ticks = timeout / portTICK_PERIOD_MS;
        BaseType_t ret = xQueueSend(handle, ele, ticks);
        if (ret != pdTRUE) {
            return -1;
        }
        return 1;
    }

    int take(void *&ele, long timeout) {
        void *pEle;
        long ticks = timeout / portTICK_PERIOD_MS;
        BaseType_t ret = xQueueReceive(handle, pEle, ticks);
        if (ret != pdTRUE) {
            return -1;
        }
        ele = pEle;
        return 1;
    }
};

} // namespace a8::hal::esp

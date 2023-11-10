#include "a8/hal/esp/FreeRtosThread.h"
#include "a8/util.h"
#include "a8/util/schedule.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace a8::hal::esp {

using namespace a8::util;
using namespace a8::util::schedule;

#define DEFAULT_THREAD_NAME ("My Thread")

void FreeRtosThread::taskFunction(void *pvParameters) {
    FreeRtosThread *thread = static_cast<FreeRtosThread *>(pvParameters);
    thread->run_(thread->context);
}
Thread *FreeRtosThread::start(run runF, void *context) {
    FreeRtosThread *th_ = new FreeRtosThread(runF, context);
    th_->start();
    return th_;
}

Thread *FreeRtosThread::getCurrentThread() {
    TaskHandle_t currentHandle = xTaskGetCurrentTaskHandle();
    void *pv = pvTaskGetThreadLocalStoragePointer(currentHandle, LOCAL_THIS);
    Thread *thread = static_cast<Thread *>(pv);
    return thread;
}
FreeRtosThread::FreeRtosThread(run runF, void *context) {
    this->run_ = runF;
    this->context = context;
}
FreeRtosThread::~FreeRtosThread() {
}
void FreeRtosThread::start() {

    TaskHandle_t handle = static_cast<TaskHandle_t>(handle_);
    BaseType_t result = xTaskCreate(
        taskFunction,
        DEFAULT_THREAD_NAME,
        STACK_DEPTH,      /* usStackDepth in words */
        this,             /* pvParameters */
        DEFAULT_PRIORITY, /* uxPriority */
        &handle           /* pxCreatedTask */
    );

    if (result != pdPASS) {
        // TODO ?
    }
    vTaskSetThreadLocalStoragePointer(handle, LOCAL_THIS, this);
}

} // namespace a8::hal::esp

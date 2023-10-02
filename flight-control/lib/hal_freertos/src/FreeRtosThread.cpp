#include "a8/hal/Hal.h"
#include "a8/hal/freertos/FreeRtosThread.h"
#include <FreeRTOS.h>
#include <task.h>

using ::a8::hal::S;

namespace a8::hal::freertos {
static int LOCAL_THIS = 0;
static int STACK_DEPTH = 521 / 4;
static int DEFAULT_PRIORITY = 1;
static const char *DEFAULT_THREAD_NAME = static_cast<const char *>("My Thread");
FreeRtosThread::FreeRtosThread(Runnable *pvRunnable) {
    this->runnable = pvRunnable;
}

FreeRtosThread::~FreeRtosThread() {
}

Thread *FreeRtosThread::start(Runnable *task) {
    FreeRtosThread *thread = new FreeRtosThread(task);
    thread->start();
    return thread;
}
/**
 * create thread
 */
void FreeRtosThread::start() {

    BaseType_t result = xTaskCreate(
        taskFunction,
        DEFAULT_THREAD_NAME,
        STACK_DEPTH,      /* usStackDepth in words */
        this,             /* pvParameters */
        DEFAULT_PRIORITY, /* uxPriority */
        &handle           /* pxCreatedTask */
    );

    if (result != pdPASS) {
        a8::hal::S->out->println("Failed to create a thread");
        // throw std::invalid_argument("failed");
    }
    vTaskSetThreadLocalStoragePointer(handle, LOCAL_THIS, this);
}

Thread *FreeRtosThread::getCurrentThread() {
    TaskHandle_t currentHandle = xTaskGetCurrentTaskHandle();
    void *pv = pvTaskGetThreadLocalStoragePointer(currentHandle, LOCAL_THIS);
    Thread *thread = static_cast<Thread *>(pv);
    return thread;
}

void FreeRtosThread::taskFunction(void *pvParameters) {

    FreeRtosThread *thread = static_cast<FreeRtosThread *>(pvParameters);

    thread->runnable->run();
}

} // namespace a8::hal::freertos
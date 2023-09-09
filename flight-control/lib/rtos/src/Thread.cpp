#include "a8/rtos/Thread.hpp"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>


namespace a8 {
namespace rtos {
static int LOCAL_THIS = 0;
static int STACK_DEPTH = 521 / 4;
static int DEFAULT_PRIORITY = 1;
static const char *DEFAULT_THREAD_NAME = static_cast<const char *>("My Thread");
Thread::Thread(Runnable *pvRunnable) : runnable(pvRunnable) {
}

Thread::~Thread() {
}
/**
 * create thread
 */
Thread *Thread::start() {

    BaseType_t result = xTaskCreate(
        taskFunction,
        DEFAULT_THREAD_NAME,
        STACK_DEPTH,      /* usStackDepth in words */
        this,             /* pvParameters */
        DEFAULT_PRIORITY, /* uxPriority */
        &handle           /* pxCreatedTask */
    );

    if (result != pdPASS) {
        Serial.println("Failed to create a thread");
        // throw std::invalid_argument("failed");
        return 0;
    }
    vTaskSetThreadLocalStoragePointer(handle, LOCAL_THIS, this);
    return this;
}

void Thread::startScheduler() {
    vTaskStartScheduler();
}
void Thread::endScheduler() {
    vTaskEndScheduler();
}
Thread *Thread::getCurrentThread() {
    TaskHandle_t currentHandle = xTaskGetCurrentTaskHandle();
    void *pv = pvTaskGetThreadLocalStoragePointer(currentHandle, LOCAL_THIS);
    Thread *thread = static_cast<Thread *>(pv);
    return thread;
}

void Thread::taskFunction(void *pvParameters) {

    Thread *thread = static_cast<Thread *>(pvParameters);

    thread->runnable->run();
}
} // namespace rtos
} // namespace a8
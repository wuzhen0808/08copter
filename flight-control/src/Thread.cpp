#include "Thread.h"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <stdexcept>
#define LOCAL_THIS 0
#define LOCAL_LAST_WAKE 1

Thread::Thread(Runnable *pvRunnable) : runnable(pvRunnable) {
}

Thread::~Thread() {
}
/**
 * create thread
 */
Thread *Thread::start() {

    auto const result = xTaskCreate(
        taskFunction,
        static_cast<const char *>("My Thread"),
        512 / 4, /* usStackDepth in words */
        this,    /* pvParameters */
        1,       /* uxPriority */
        &handle  /* pxCreatedTask */
    );

    if (result != pdPASS) {
        Serial.println("Failed to create a thread");
        throw std::invalid_argument("failed");
    }
    vTaskSetThreadLocalStoragePointer(handle, LOCAL_THIS, this);
    return this;
}
/**
 * Periodical execution.
 */
Thread *Thread::delayUtil(TickType_t frequency) {
    void *pv = pvTaskGetThreadLocalStoragePointer((*this).handle, LOCAL_LAST_WAKE);
    TickType_t *lastWakeTime = static_cast<TickType_t *>(pv);
    vTaskDelayUntil(lastWakeTime, frequency);
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
    Serial.println("taskFunction");

    Thread *thread = static_cast<Thread *>(pvParameters);
    TickType_t lastWakeTime = xTaskGetTickCount();
    vTaskSetThreadLocalStoragePointer((*thread).handle, LOCAL_LAST_WAKE, &lastWakeTime);

    thread->runnable->run();
}

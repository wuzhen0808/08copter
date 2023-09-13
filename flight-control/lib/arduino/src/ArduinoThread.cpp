#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "a8/arduino/ArduinoThread.h"

namespace a8 {
namespace arduino {
static int LOCAL_THIS = 0;
static int STACK_DEPTH = 521 / 4;
static int DEFAULT_PRIORITY = 1;
static const char *DEFAULT_THREAD_NAME = static_cast<const char *>("My Thread");
ArduinoThread::ArduinoThread(Runnable *pvRunnable) {
    this->runnable = pvRunnable;
}

ArduinoThread::~ArduinoThread() {
}

Thread *ArduinoThread::start(Runnable *task) {
    ArduinoThread *thread = new ArduinoThread(task);
    thread->start();
    return thread;
}
/**
 * create thread
 */
void ArduinoThread::start() {

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
    }
    vTaskSetThreadLocalStoragePointer(handle, LOCAL_THIS, this);
}

Thread *ArduinoThread::getCurrentThread() {
    TaskHandle_t currentHandle = xTaskGetCurrentTaskHandle();
    void *pv = pvTaskGetThreadLocalStoragePointer(currentHandle, LOCAL_THIS);
    Thread *thread = static_cast<Thread *>(pv);
    return thread;
}

void ArduinoThread::taskFunction(void *pvParameters) {

    ArduinoThread *thread = static_cast<ArduinoThread *>(pvParameters);

    thread->runnable->run();
}
} // namespace ardui
} // namespace a8
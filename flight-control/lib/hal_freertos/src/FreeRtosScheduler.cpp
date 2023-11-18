#include "a8/hal/freertos/FreeRtosScheduler.h"
#include "FreeRTOS.h"
#include "a8/hal/freertos/FreeRtosSemaphore.h"
#include "a8/hal/freertos/FreeRtosSyncQueue.h"
#include "a8/hal/freertos/FreeRtosThread.h"
#include "task.h"

namespace a8::hal::freertos {

void taskCallback(void *pvParameters) {
    FreeRtosThread *thread = static_cast<FreeRtosThread *>(pvParameters);
    thread->run();
    vTaskDelete(thread->getHandle());
}

void timerCallback(TimerHandle_t handle) {
    void *timerId = pvTimerGetTimerID(handle);
    FreeRtosTimer *timer = static_cast<FreeRtosTimer *>(timerId);
    timer->run();
}
/**
 * For unkown reason, xTaskCreate does not work:
 * 1) Unhandled exception raised when calling the task function;
 * 2) Because the function pointer of FreeRtosThread.run_ is 0.
 * 3) Actually FreeRtosThread.context is 0 as well.
 * After changed the creation method to xTaskCreatePinnedToCore(or xTaskCreateUniversal which is declared by header esp32-hal.h) the error disappeared.
 *
 */
Thread *FreeRtosScheduler::createTask(const String name, void *context, sched::run run) {
    FreeRtosThread *thread = new FreeRtosThread(run, context);
    TaskHandle_t handle = 0;

    BaseType_t result =
        xTaskCreate(
            taskCallback,
            name.text(),
            1024,   // usStackDepth in words
            thread, // pvParameters
            1,      // uxPriority
            &handle // pxCreatedTask
        );

    if (result != pdPASS) {
        delete thread;
        return 0;
    }
    thread->setHandle(handle);
    return thread;
}

Timer *FreeRtosScheduler::createTimer(const String name, const Rate &rate, void *context, sched::run run) {
    FreeRtosTimer *timer = new FreeRtosTimer(run, context, rate);
    TimerHandle_t handle = 0;
    long ticks = configTICK_RATE_HZ / rate.Hz();
    handle = xTimerCreate(
        static_cast<const char *>("MyTimer"), // name
        ticks,                                /*  */
        true,                                 /*  */
        timer,                                // timer id
        timerCallback);
    BaseType_t isOk = xTimerStart(handle, 0);
    if (isOk != pdPASS) {
        delete timer;
        return 0;
    }
    return timer;
}
Semaphore *FreeRtosScheduler::createSemaphore(int cap, int initial) {
    SemaphoreHandle_t handle = xSemaphoreCreateCounting(cap, initial);
    return new FreeRtosSemaphore(handle);
}

SyncQueue<void *> *FreeRtosScheduler::doCreateSyncQueue(int cap, int itemSize) {
    QueueHandle_t handle = xQueueCreate(cap, itemSize);
    return new FreeRtosSyncQueue(handle);
}

} // namespace a8::hal::freertos
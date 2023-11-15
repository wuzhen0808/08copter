#include "a8/hal/esp/FreeRtosScheduler.h"
#include "a8/hal/esp/FreeRtosSemaphore.h"
#include "a8/hal/esp/FreeRtosSyncQueue.h"
#include "a8/hal/esp/FreeRtosThread.h"

#include <esp32-hal.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace a8::hal::esp {

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
Thread *FreeRtosScheduler::schedule(schedule::run run, void *context) {
    FreeRtosThread *thread = new FreeRtosThread(run, context);
    TaskHandle_t handle = 0;

    BaseType_t result =
        xTaskCreatePinnedToCore(
            taskCallback,
            "MyTask",
            8192,
            thread,
            1,
            &handle,
            ARDUINO_RUNNING_CORE);

    /*
    //NOTE:

    xTaskCreate(
        taskCallback,
        "MyTask",
        521 / 4, // usStackDepth in words
        thread,  // pvParameters
        1,       // uxPriority
        &handle  // pxCreatedTask
    );
    */

    if (result != pdPASS) {
        delete thread;
        return 0;
    }
    thread->setHandle(handle);
    return thread;
}

Timer *FreeRtosScheduler::scheduleTimer(schedule::run run, void *context, const Rate &rate) {
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

SyncQueue* FreeRtosScheduler::createSyncQueue(int cap){
    QueueHandle_t handle = xQueueCreate(cap, sizeof(void*));
    return new FreeRtosSyncQueue(handle);
}

} // namespace a8::hal::esp
#include "a8/hal/esp/EspScheduler.h"
#include "a8/hal/esp/FreeRtosSemaphore.h"
#include "a8/hal/esp/FreeRtosSyncQueue.h"
#include "a8/hal/esp/FreeRtosTask.h"

#include <esp32-hal.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace a8::hal::esp {

void taskCallback(void *pvParameters) {
    FreeRtosTask *thread = static_cast<FreeRtosTask *>(pvParameters);
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
Task *EspScheduler::createTask(const String name, int priority, void *context, sched::run run) {
    FreeRtosTask *thread = new FreeRtosTask(run, context);
    TaskHandle_t handle = 0;
    
    BaseType_t result =
        xTaskCreatePinnedToCore(
            taskCallback,
            name.text(),
            8192,
            thread,
            priority,
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

Timer *EspScheduler::createTimer(const String name, const Rate &rate, void *context, sched::run run) {
    FreeRtosTimer *timer = new FreeRtosTimer(run, context, rate);
    TimerHandle_t handle = 0;
    long ticks = configTICK_RATE_HZ / rate.Hz();
    handle = xTimerCreate(
        name.text(), // name
        ticks,                                /*  */
        true,                                 /*  */
        timer,                                // timer id
        timerCallback);
    BaseType_t isOk = xTimerStart(handle, 0);
    if (isOk != pdPASS) {
        delete timer;
        return 0;
    }
    timer->setHandle(handle);
    return timer;
}
Semaphore *EspScheduler::createSemaphore(int cap, int initial) {
    SemaphoreHandle_t handle = xSemaphoreCreateCounting(cap, initial);
    return new FreeRtosSemaphore(handle);
}

SyncQueue<void *> *EspScheduler::doCreateSyncQueue(int cap, int itemSize) {
    QueueHandle_t handle = xQueueCreate(cap, itemSize);
    return new FreeRtosSyncQueue(handle);
}

} // namespace a8::hal::esp
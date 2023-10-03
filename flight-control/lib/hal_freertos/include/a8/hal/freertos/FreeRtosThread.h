#pragma once
#include "a8/util.h"
#include "a8/util/thread.h"

#include <FreeRTOS.h>
#include <task.h>

using namespace a8::util;
using namespace a8::util::thread;

namespace a8::hal::freertos {

#define DEFAULT_THREAD_NAME ("My Thread")

class FreeRtosThread : public Thread {
private:
    static const int LOCAL_THIS = 0;
    static const int STACK_DEPTH = 521 / 4;
    static const int DEFAULT_PRIORITY = 1;
    TaskHandle_t handle;
    thread::FuncType::run run;
    void *context;

    static void taskFunction(void *pvParameters) {

        FreeRtosThread *thread = static_cast<FreeRtosThread *>(pvParameters);
        thread->run(thread->context);
    }

public:
    static Thread *start(thread::FuncType::run run, void *context) {
        FreeRtosThread *th_ = new FreeRtosThread(run, context);
        th_->start();
        return th_;
    }

    static Thread *getCurrentThread() {
        TaskHandle_t currentHandle = xTaskGetCurrentTaskHandle();
        void *pv = pvTaskGetThreadLocalStoragePointer(currentHandle, LOCAL_THIS);
        Thread *thread = static_cast<Thread *>(pv);
        return thread;
    }
    FreeRtosThread(thread::FuncType::run run, void *context) {
        this->run = run;
        this->context = context;
    }
    ~FreeRtosThread() {
    }
    virtual void start() {

        BaseType_t result = xTaskCreate(
            taskFunction,
            DEFAULT_THREAD_NAME,
            STACK_DEPTH,      /* usStackDepth in words */
            this,             /* pvParameters */
            DEFAULT_PRIORITY, /* uxPriority */
            &handle           /* pxCreatedTask */
        );

        if (result != pdPASS) {
            // a8::hal::S->out->println("Failed to create a thread");
            //  throw std::invalid_argument("failed");
        }
        vTaskSetThreadLocalStoragePointer(handle, LOCAL_THIS, this);
    }
};

} // namespace a8::hal::freertos

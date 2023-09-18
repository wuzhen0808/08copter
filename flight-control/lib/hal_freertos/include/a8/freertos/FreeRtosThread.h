#ifndef ARDUINO_THREAD_H__
#define ARDUINO_THREAD_H__
#include "a8/core/Runnable.h"
#include "a8/core/Thread.h"

#include <FreeRTOS.h>
#include <task.h>
namespace a8::freertos {
using namespace a8::core;

class FreeRtosThread : public Thread {
private:
    TaskHandle_t handle;
    Runnable *runnable;
    static void taskFunction(void *pvParameters);

public:
    FreeRtosThread(Runnable *runnable);
    ~FreeRtosThread();
    virtual void start();
    static Thread *start(Runnable *task);
    static Thread *getCurrentThread();
};

} // namespace a8::freertos

#endif
#ifndef ARDUINO_THREAD_H__
#define ARDUINO_THREAD_H__
#include "a8/util/Runnable.h"
#include "a8/util/Thread.h"

#include <FreeRTOS.h>
#include <task.h>

using namespace a8::util;
namespace a8::hal::freertos {

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

} // namespace a8::hal::freertos

#endif
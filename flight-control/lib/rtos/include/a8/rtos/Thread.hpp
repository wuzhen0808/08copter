#ifndef THREAD__
#define THREAD__
#include "a8/rtos/Runnable.hpp"
#include <Arduino_FreeRTOS.h>
namespace a8 {
namespace rtos {
class Thread {
public:
    Thread(Runnable *runnable);
    ~Thread();
    Thread *start();
    // static
    static Thread *start(Runnable *runnable) {
        Thread *thread = new Thread(runnable);
        return thread->start();
    };
    static void startScheduler();
    static void endScheduler();
    static Thread *getCurrentThread();

private:
    TaskHandle_t handle;
    Runnable *runnable;
    static void taskFunction(void *pvParameters);
};
} // namespace rtos
} // namespace a8
#endif
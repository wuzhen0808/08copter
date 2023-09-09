#ifndef THREAD__
#define THREAD__
#include "a8/Runnable.hpp"
#include <Arduino_FreeRTOS.h>
namespace a8 {
class Thread {
public:
    Thread(Runnable *runnable);
    ~Thread();
    Thread *start();
    Thread *delayUtil(TickType_t frequency);

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
} // namespace a8
#endif
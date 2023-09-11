#ifndef ARDUINO_THREAD_H__
#define ARDUINO_THREAD_H__
#include "a8/core/Thread.hpp"
#include "a8/core/Runnable.hpp"
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
namespace a8 {
namespace ardui {
using namespace a8::core;

class ArduinoThread : public Thread {
public:
    ArduinoThread(Runnable *runnable);
    ~ArduinoThread();
    virtual void start();
    static Thread * start(Runnable * task);
    static Thread * getCurrentThread();
private:
    TaskHandle_t handle;
    Runnable *runnable;
    static void taskFunction(void *pvParameters);
};
} // namespace arduino
} // namespace a8

#endif
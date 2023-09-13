#include "a8/arduino/ArduinoScheduler.h"
#include "a8/arduino/ArduinoTimer.h"
#include "a8/arduino/ArduinoThread.h"
#include "a8/arduino/ArduinoScheduler.h"
namespace a8 {
namespace arduino {
using namespace a8::core;


ArduinoScheduler::ArduinoScheduler() : Scheduler() {
}

ArduinoScheduler::~ArduinoScheduler() {
}

void ArduinoScheduler::startSchedule() {
    vTaskStartScheduler();
}
void ArduinoScheduler::endSchedule() {
    vTaskEndScheduler();
}

Thread *ArduinoScheduler::schedule(Runnable *runnable) {
    return ArduinoThread::start(runnable);
}

Timer *ArduinoScheduler::scheduleTimer(Callback *callback, ulong ticks) {
    return ArduinoTimer::start(callback, ticks);
}

Thread *ArduinoScheduler::getCurrentThread() {
    return ArduinoThread::getCurrentThread();
}
} // namespace arduino
} // namespace a8
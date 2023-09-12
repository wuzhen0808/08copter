#include "a8/arduino/ArduinoScheduler.hpp"
#include "a8/arduino/ArduinoTimer.hpp"
#include "a8/arduino/ArduinoThread.hpp"
#include "a8/arduino/ArduinoScheduler.hpp"
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
#include "a8/core/Copter.hpp"
#include "a8/core/AttitudeControl.hpp"
#include "a8/rtos/Runnable.hpp"
#include "a8/rtos/Thread.hpp"
#define TOTAL_COMPONENTS 10
namespace a8 {
namespace core {
using namespace a8::rtos;
Copter::Copter(Scheduler *scheduler) {
    this->scheduler = scheduler;
}

Copter::~Copter() {
}
Scheduler *Copter::getScheduler() {
    return scheduler;
}

void Copter::destroy() {
}
int Copter::getServoCount() {
    return (*this).servoCount;
}
int Copter::getServoAttachPin(int servoId) {
    return (*this).servoAttachPins[servoId];
}
void Copter::log(String message) {
    Serial.println(message);
}

} // namespace core
} // namespace a8
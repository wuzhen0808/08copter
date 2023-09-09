#include "a8/Copter.hpp"
#include "a8/AttitudeControl.hpp"
#include "a8/Runnable.hpp"
#include "a8/Thread.hpp"
#define TOTAL_COMPONENTS 10
namespace a8 {

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

} // namespace a8
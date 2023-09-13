#include "a8/core/Copter.h"
#include "a8/core/AttitudeControl.h"
#include "a8/core/Runnable.h"
#include "a8/core/Thread.h"
#include "a8/hal/System.h"
#define TOTAL_COMPONENTS 10
namespace a8 {
namespace core {
using a8::core::Copter;
using a8::hal::S;

Copter::Copter(int servoCount, Scheduler *Scheduler) {
    this->servoCount = servoCount;
    this->scheduler = scheduler;
}

Copter::~Copter() {
}
Scheduler* Copter::getScheduler(){
    return this->scheduler;
}
void Copter::setup() {
    this->log(">>Copter::setup()");
    servosControl = newServosControl();
    servosControl->attachAll();
    servosControl->setVelocity(0, 1.0f);
    attitudeSensor = newAttitudeSensor();
    attitudeControl = newAttitudeControl(servosControl, attitudeSensor);
    attitudeTimer = scheduler->scheduleTimer(attitudeControl, 1000);
    this->log("<<Copter::setup()");
}

void Copter::start() {
    S->out->println(">>Copter::start()");
    scheduler->startSchedule();
    // scheduler->schedule(attitudeControl);
    // attitudeControl->run();
    // copter->log("done of active copter.");
    S->out->println("<<Copter::start()");
}
void Copter::stop() {
}
int Copter::getServoCount() {
    return this->servoCount;
}

void Copter::log(String message) {
    S->out->println(message);
}

} // namespace core
} // namespace a8
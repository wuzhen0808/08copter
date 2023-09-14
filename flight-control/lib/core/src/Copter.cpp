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

Copter::Copter(Scheduler *Scheduler) {
    this->scheduler = scheduler;
}
void Copter::configServos(int totalServos, int *servoAttachPins) {
    if (this->servoAttachPins) {
        // throw exception, re-config unsupported.
    }
    this->totalServos = totalServos;
    this->servoAttachPins = new int[totalServos]{0};
    for (int i = 0; i < totalServos; i++) {
        this->servoAttachPins[i] = servoAttachPins[i];
    }
}

Copter::~Copter() {
}

Scheduler *Copter::getScheduler() {
    return this->scheduler;
}
void Copter::setup() {

    this->log(">>Copter::setup()");
    servosControl = newServosControl(this->totalServos, this->servoAttachPins);
    servosControl->attachAll();
    servosControl->setVelocity(0, 1.0f);
    attitudeSensor = newAttitudeSensor();
    attitudeControl = new AttitudeControl(this, servosControl, attitudeSensor);
    if (scheduler == 0) {
        this->log("scheduler is null");
    } else {
        this->log("scheduler is NOT null");
    }
    long ticks = 1000;
    Callback *callback = static_cast<Callback *>(attitudeControl);

    //attitudeTimer = scheduler->scheduleTimer(callback, ticks);
    
    attitudeTimer = scheduler->tmpTimer();
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
    return this->totalServos;
}

void Copter::log(String message) {
    S->out->println(message);
}

} // namespace core
} // namespace a8
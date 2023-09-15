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

Copter::Copter(Scheduler *scheduler) {
    this->scheduler_ = scheduler;
}
void Copter::configServos(int totalServos, int *servoAttachPins) {
    
    if (this->servoAttachPins_) {
        // throw exception, re-config unsupported.
    }
    this->totalServos_ = totalServos;
    this->servoAttachPins_ = new int[totalServos];
    for (int i = 0; i < totalServos_; i++) {
        this->servoAttachPins_[i] = servoAttachPins[i];
    }
    
}

Copter::~Copter() {
}

Scheduler *Copter::getScheduler() {
    return this->scheduler_;
}
void Copter::setup() {

    this->log(String::format(">>Copter::setup(),totalServos:%i", totalServos_));
    servosControl_ = newServosControl(this->totalServos_, this->servoAttachPins_);
    servosControl_->attachAll();
    servosControl_->setVelocity(0, 1.0f);
    attitudeSensor_ = newAttitudeSensor();
    attitudeControl_ = new AttitudeControl(this, servosControl_, attitudeSensor_);

    long ticks = 1000;

    attitudeTimer_ = scheduler_->scheduleTimer(attitudeControl_, ticks);

    this->log("<<Copter::setup()");
}

void Copter::start() {
    S->out->println(">>Copter::start()");
    scheduler_->startSchedule();
    // scheduler->schedule(attitudeControl);
    // attitudeControl->run();
    // copter->log("done of active copter.");
    S->out->println("<<Copter::start()");
}
void Copter::stop() {
}
int Copter::getServoCount() {
    return this->totalServos_;
}

void Copter::log(String message) {
    S->out->println(message);
}

} // namespace core
} // namespace a8
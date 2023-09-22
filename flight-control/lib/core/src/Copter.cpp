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
using a8::util::Result;

Copter::Copter(Scheduler *scheduler) {
    this->totalServos_ = 4;
    this->scheduler_ = scheduler;
}

Copter::Copter(int totalServos, Scheduler *scheduler) {
    this->totalServos_ = totalServos;
    this->scheduler_ = scheduler;
}

Copter::~Copter() {
}

Scheduler *Copter::getScheduler() {
    return this->scheduler_;
}

Result Copter::setup() {

    this->log(String::format(">>Copter::setup(),totalServos:%i", totalServos_));
    servosControl_ = setupServosControl();

    if (!servosControl_) {
        return false;
    }

    attitudeSensor_ = setupAttitudeSensor();
    if (!servosControl_) {
        return false;
    }
    attitudeControl_ = new AttitudeControl(this, servosControl_, attitudeSensor_);

    //

    int totalTasks = servosControl_->getTotalTasks();
    for (int i = 0; i < totalTasks; i++) {
        Runnable *task = servosControl_->getTask(i);
        scheduler_->schedule(task);
    }
    //

    long ticks = 1000;

    attitudeTimer_ = scheduler_->scheduleTimer(attitudeControl_, ticks);

    this->log("<<Copter::setup()");
    return true;
}

Result Copter::start() {
    S->out->println(">>Copter::start()");
    scheduler_->startSchedule();
    S->out->println("<<Copter::start()");
    return true;
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
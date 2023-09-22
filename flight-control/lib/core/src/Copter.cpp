#include "a8/core/Copter.h"
#include "a8/core/AttitudeControl.h"
#include "a8/util/Runnable.h"
#include "a8/util/Thread.h"
#include "a8/hal/System.h"
#define TOTAL_COMPONENTS 10
namespace a8 {
namespace core {
using a8::core::Copter;
using a8::hal::S;
using a8::util::Result;

Copter::Copter() {
    this->totalServos_ = 4;
}

Copter::Copter(int totalServos) {
    this->totalServos_ = totalServos;
}

Copter::~Copter() {
}

void Copter::setup(Context * context) {

    this->log(String::format(">>Copter::setup(),totalServos:%i", totalServos_));
    attitudeSensor_ = setupAttitudeSensor(context);    
    servosControl_ = setupServosControl(context);
    attitudeControl_ = new AttitudeControl(this, servosControl_, attitudeSensor_);
    //

    long ticks = 1000;

    attitudeTimer_ = context->scheduler->scheduleTimer(attitudeControl_, ticks);
    this->log("<<Copter::setup()");    
}

void Copter::start(Context * context) {
    S->out->println(">>Copter::start()");
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
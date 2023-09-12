#include "a8/core/Copter.hpp"
#include "a8/core/AttitudeControl.hpp"
#include "a8/core/Runnable.hpp"
#include "a8/core/Thread.hpp"
#include "a8/hal/System.hpp"
#define TOTAL_COMPONENTS 10
namespace a8 {
namespace core {
using namespace a8::hal;
using namespace a8::core;

Copter::Copter(int servoCount, Scheduler *scheduler) {
    this->scheduler = scheduler;
    this->servoCount = servoCount;
}

Copter::~Copter() {
}
Scheduler *Copter::getScheduler() {
    return scheduler;
}

void Copter::start() {
}
void Copter::stop() {
}
int Copter::getServoCount() {
    return this->servoCount;
}

void Copter::log(String message) {
    log(message);
}

} // namespace core
} // namespace a8
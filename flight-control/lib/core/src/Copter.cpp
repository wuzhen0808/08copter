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

Copter::Copter(int servoCount) {
    this->servoCount = servoCount;
    
}

Copter::~Copter() {
}

void Copter::start() {
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
#include "a8/core/Copter.hpp"
#include "a8/core/AttitudeControl.hpp"
#include "a8/core/Runnable.hpp"
#include "a8/core/Thread.hpp"
#include "a8/hal/System.hpp"
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
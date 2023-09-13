#include "a8/native/NativeCopter.h"
#include <iostream>

namespace a8 ::native {
using namespace std;
using namespace a8::core;
using namespace a8::util;
using a8::core::Scheduler;

NativeCopter::NativeCopter(Scheduler *scheduler) : Copter(0, scheduler) {
}
void NativeCopter::setup() {
    Copter::setup();
}
void NativeCopter::start() {
    Copter::start();
}
void NativeCopter::stop() {
    Copter::stop();
}
int NativeCopter::getServoAttachPin(int servoId) {
    return 4;
}
ServosControl *NativeCopter::newServosControl() {
    return 0;
}
AttitudeSensor *NativeCopter::newAttitudeSensor() {
    return 0;
}
AttitudeControl *NativeCopter::newAttitudeControl(ServosControl *sc, AttitudeSensor *as) {
    return 0;
}
} // namespace a8::native

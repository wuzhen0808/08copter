#include "a8/native/NativeCopter.h"
#include "a8/native/NativeAttitudeSensor.h"
#include "a8/native/NativeServosControl.h"
#include <iostream>

namespace a8 ::native {
using namespace std;
using namespace a8::core;
using namespace a8::util;
using a8::core::Scheduler;

NativeCopter::NativeCopter(Scheduler *scheduler) : Copter(scheduler) {

    int pins[] = {1, 2, 3, 4};
    this->configServos(4, pins);
}
NativeCopter::~NativeCopter() {
}
// void NativeCopter::setup() {
//     Copter::setup();
// }
void NativeCopter::start() {
    Copter::start();
}
void NativeCopter::stop() {
    Copter::stop();
}
int NativeCopter::getServoAttachPin(int servoId) {
    return 4;
}
ServosControl *NativeCopter::newServosControl(int totalServos, int *servoAttachPins) {
    return new NativeServosControl(totalServos, servoAttachPins);
}
AttitudeSensor *NativeCopter::newAttitudeSensor() {
    return new NativeAttitudeSensor();
}
AttitudeControl *NativeCopter::newAttitudeControl(ServosControl *sc, AttitudeSensor *as) {
    return 0;
}
} // namespace a8::native

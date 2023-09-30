#include "a8/hal/native/NativeCopter.h"
#include "a8/hal/native/NativeAttitudeSensor.h"
#include "a8/hal/native/NativeServosControl.h"
#include <iostream>

namespace a8::hal::native {
using namespace std;
using namespace a8::core;
using namespace a8::util;

void NativeCopter::setup(Context *context) {
    Copter::setup(context);
}
void NativeCopter::start(Context *context) {
    Copter::start(context);
}
void NativeCopter::stop() {
    Copter::stop();
}

ServosControl *NativeCopter::createServosControl(Context *context) {
    ServosControl *servos = new NativeServosControl(totalServos_, jsbSimIo);
    
    return servos;
}
AttitudeSensor *NativeCopter::createAttitudeSensor(Context *context) {
    NativeAttitudeSensor *sensor = new NativeAttitudeSensor(this->jsbSimIo);
    
    return sensor;
}

} // namespace a8::hal::native

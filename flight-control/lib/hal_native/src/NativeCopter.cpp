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

ServosControl *NativeCopter::setupServosControl(Context * context) {
    ServosControl *servos = new NativeServosControl(totalServos_, jsbSimIo);
    Result setupRst = servos->setup();
    if (setupRst.error) {
        log(setupRst.toString());
        return 0;
    }
    return servos;
}
AttitudeSensor *NativeCopter::setupAttitudeSensor(Context * context) {
    return new NativeAttitudeSensor(this->jsbSimIo);
}

} // namespace a8::hal::native

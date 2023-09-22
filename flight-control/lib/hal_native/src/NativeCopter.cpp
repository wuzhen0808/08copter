#include "a8/hal/native/NativeCopter.h"
#include "a8/hal/native/NativeAttitudeSensor.h"
#include "a8/hal/native/NativeServosControl.h"
#include <iostream>

namespace a8::hal::native {
using namespace std;
using namespace a8::core;
using namespace a8::util;
using a8::core::Scheduler;

NativeCopter::NativeCopter(Scheduler *scheduler, SocketFactory *sFac, String host, int port) : Copter(4, scheduler) {

    this->socketFactory_ = sFac;
    this->host = host;
    this->port = port;
}
NativeCopter::~NativeCopter() {
}

Result NativeCopter::setup() {
    this->socket_ = socketFactory_->socket();
    return Copter::setup();
}
Result NativeCopter::start() {
    return Copter::start();
}
void NativeCopter::stop() {
    Copter::stop();
}

ServosControl *NativeCopter::setupServosControl() {
    ServosControl *servos = new NativeServosControl(totalServos_, this->socket_, this->host, this->port);
    Result setupRst = servos->setup();
    if (setupRst.error) {
        log(setupRst.toString());
        return 0;
    }
    return servos;
}
AttitudeSensor *NativeCopter::setupAttitudeSensor() {
    return new NativeAttitudeSensor();
}
AttitudeControl *NativeCopter::setupAttitudeControl(ServosControl *sc, AttitudeSensor *as) {
    return 0;
}
} // namespace a8::hal::native

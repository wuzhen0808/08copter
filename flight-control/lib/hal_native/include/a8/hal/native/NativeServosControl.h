#pragma once
#include "a8/hal/native/JSBSimIO.h"
#include "a8/core/Copter.h"
#include "a8/core/ServosControl.h"
#include "a8/hal/Servo.h"
#include "a8/util/Socket.h"
using namespace a8::core;
using namespace a8::util;

namespace a8::hal::native {

class NativeServosControl : public ServosControl {

private:
    JSBSimIO *jio;

public:
    NativeServosControl(int totalServos, JSBSimIO *jio) : ServosControl(totalServos) {
        this->jio = jio;
    }
    ~NativeServosControl() {}
    virtual Result setup();
    virtual Servo *setupServo(int servoId);
};

} // namespace a8::hal::native

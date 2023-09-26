#pragma once
#include "a8/core/Copter.h"
#include "a8/core/ServosControl.h"
#include "a8/hal/Servo.h"
#include "a8/hal/native/JSBSimIO.h"
#include "a8/hal/native/NativeServo.h"

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
    virtual Servo *createServo(int servoId) override{
        return new NativeServo(jio, servoId);
    }
};

} // namespace a8::hal::native

#pragma once
#include "a8/fc.h"
#include "a8/fc/FlightControl.h"
#include "a8/fc/ServosControl.h"
#include "a8/fc/native/JSBSimIO.h"
#include "a8/fc/native/NativeServo.h"

using namespace a8::fc;
using namespace a8::util;

namespace a8::fc::native {

class NativeServosControl : public ServosControl {

private:
    JSBSimIO *jio;

public:
    NativeServosControl(int totalServos, JSBSimIO *jio) : ServosControl(totalServos) {
        this->jio = jio;
    }
    ~NativeServosControl() {}
    virtual Servo *createServo(int servoId) override {
        return new NativeServo(jio, servoId);
    }
};

} // namespace a8::fc::native

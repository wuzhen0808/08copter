#pragma once
#include "a8/hal/Servo.h"
#include "a8/fcs/native/JSBSimIO.h"


using namespace a8::util;

namespace a8::hal::native {

class NativeServo : public a8::hal::Servo {

private:
    JSBSimIO *jio;

public:
    NativeServo(JSBSimIO *jio, int id) : Servo(id) {
        this->jio = jio;
    }

    ~NativeServo() {}

    virtual void setThrottleNorm(float throttle) {
        this->jio->setThrottleNorm(this->id, throttle);
    }
    virtual void setup();
};

} // namespace a8::hal::native

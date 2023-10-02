#pragma once
#include "a8/fc.h"
#include "a8/fc/native/JSBSimIO.h"

using namespace a8::util;
using namespace a8::fc;
namespace a8::fc::native {

class NativeServo : public Servo {

private:
    JSBSimIO *jio;

public:
    NativeServo(JSBSimIO *jio, int id) : Servo(id) {
        this->jio = jio;
    }

    ~NativeServo() {}

    void setThrottleNorm(float throttle) override {
        this->jio->setThrottleNorm(this->id, throttle);
    }
    void setup() override {
    }
};

} // namespace a8::fc::native

#pragma once
#include "a8/fc.h"
#include <ESP32Servo.h>
namespace a8::fc {
using namespace a8::fc;

class EspPropeller : public Propeller {
    Servo servo;

public:
    EspPropeller() {
    }

    void hz(int hz) {
        servo.setPeriodHertz(hz);
    }

    int attach(int pin) {
        return servo.attach(pin);
    }
    void setup() {
        servo.writeMicroseconds(1000);
    }

    void setThrottlePwm(long pwm) override {
        servo.writeMicroseconds(pwm);
    }
};
} // namespace a8::fc
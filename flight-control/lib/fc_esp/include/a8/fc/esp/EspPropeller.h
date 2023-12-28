#pragma once
#include "a8/fc.h"
#include <ESP32Servo.h>
namespace a8::fc::esp {
using namespace a8::fc;

class EspPropeller : public Propeller {
    Servo servo;

public:
    EspPropeller(String name, int idx) : Propeller(name, idx) {
    }

    void hz(int hz) override {
        servo.setPeriodHertz(hz);
    }

    int attach(int pin) override {
        return servo.attach(pin);
    }
    void setup() override {
        servo.writeMicroseconds(1000);
    }

    void doApply(long pwm) override {
        servo.writeMicroseconds(pwm);
    }
};
} // namespace a8::fc::esp
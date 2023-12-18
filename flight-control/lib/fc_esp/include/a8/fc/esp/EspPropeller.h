#pragma once
#include "a8/fc.h"
#include <ESP32Servo.h>
namespace a8::fc::esp {
using namespace a8::fc;

class EspPropeller : public Propeller {
    Servo servo;    
public:
    EspPropeller(String name, PwmManage *pwmManage) : Propeller(name, pwmManage) {
    }

    void hz(int hz) {
        servo.setPeriodHertz(hz);
    }

    int attach(int pin) {
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
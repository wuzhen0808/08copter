#pragma once
#include "a8/hal/Servo.h"
namespace a8::jsbsim {

using a8::hal::Servo;
class JsbsimServo : public Servo {
public:
    virtual void writeMicroseconds(int pw);
    virtual void attach(int pin);
};
} // namespace a8::jsbsim
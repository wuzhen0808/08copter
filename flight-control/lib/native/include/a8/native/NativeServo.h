#pragma once
#include "a8/hal/Servo.h"

namespace a8::native {

class NativeServo : public a8::hal::Servo {
private:
public:
    NativeServo();
    ~NativeServo();

    virtual void writeMicroseconds(int pw);
    virtual void attach(int pin);
};

} // namespace a8::native

#pragma once
#include "a8/hal/Servo.h"
#include <Servo.h>

namespace a8::arduino {

class ArduinoServo : public a8::hal::Servo {
private:
    Servo servo;

public:
    virtual void writeMicroseconds(int pw);
    virtual void attach(int pin);
};

} // namespace a8::arduino

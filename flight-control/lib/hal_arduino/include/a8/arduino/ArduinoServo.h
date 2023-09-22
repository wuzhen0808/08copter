#pragma once
#include "a8/hal/Servo.h"
#include <Servo.h>

namespace a8::arduino {

class ArduinoServo : public a8::hal::Servo {
private:
    int pin;
    Servo servo;

public:
    ArduinoServo(int id, int pin) : Servo(id) {
        this->pin = pin;
    }
    virtual void writeMicroseconds(int pw);
    void setup();
};

} // namespace a8::arduino

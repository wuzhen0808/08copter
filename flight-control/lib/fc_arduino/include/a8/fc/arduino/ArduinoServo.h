#pragma once
#include "a8/fc.h"
#include <Servo.h>

namespace a8::fc::arduino {

class ArduinoServo : public a8::fc::Servo {
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

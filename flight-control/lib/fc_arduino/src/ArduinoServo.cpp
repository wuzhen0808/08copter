#include "a8/fc/arduino/ArduinoServo.h"
#include <Servo.h>

namespace a8::fc::arduino {

ArduinoServo::ArduinoServo() {
    this->servo = new Servo();
}

void ArduinoServo::writeMicroseconds(int pw) {
    this->servo->writeMicroSeconds(pw);
}

void ArduinoServo::setup() {
    this->servo->attach(pin);
    Servo::setup();    
}

} // namespace a8
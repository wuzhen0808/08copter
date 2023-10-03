#include "a8/fc/arduino/ArduinoServosControl.h"
#include "a8/fc/arduino/ArduinoServo.h"

namespace a8::fc::arduino {

ArduinoServosControl::ArduinoServosControl(int totalServos, int *servoAttachPins) : ServosControl(totalServos, servoAttachPins) {
}

Servo *ArduinoServosControl::newServo(int servoId) {
    return new ArduinoServo();
}

ArduinoServosControl::~ArduinoServosControl() {
}

} // namespace a8::arduino
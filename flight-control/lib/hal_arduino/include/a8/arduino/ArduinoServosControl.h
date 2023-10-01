#ifndef ARDUINO_SERVOS_CONTROL_H__
#define ARDUINO_SERVOS_CONTROL_H__
#include "a8/fcs/FlightControl.h"
#include "a8/fcs/ServosControl.h"
#include "a8/hal/Servo.h"

namespace a8::arduino {
using namespace a8::fcs;

class ArduinoServosControl : public ServosControl {

private:
public:
    ArduinoServosControl(int totalServos, int *servoAttachPins);
    ~ArduinoServosControl();
    virtual Servo *newServo(int servoId);
};

} // namespace a8::arduino
#endif
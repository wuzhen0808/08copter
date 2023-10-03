#pragma once
#include "a8/fc.h"

using namespace a8::fc;
namespace a8::fc::arduino {

class ArduinoServosControl : public ServosControl {

private:
public:
    ArduinoServosControl(int totalServos, int *servoAttachPins);
    ~ArduinoServosControl();
    virtual Servo *newServo(int servoId);
};

} // namespace a8::arduino

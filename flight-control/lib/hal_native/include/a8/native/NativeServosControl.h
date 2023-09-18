#pragma once
#include "a8/core/Copter.h"
#include "a8/core/ServosControl.h"
#include "a8/hal/Servo.h"

namespace a8::native {
using namespace a8::core;

class NativeServosControl : public ServosControl {

private:
public:
    NativeServosControl(int totalServos, int *servoAttachPins);
    ~NativeServosControl();
    virtual Servo *newServo(int servoId);
};

} // namespace a8::arduino

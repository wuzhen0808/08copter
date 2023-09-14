#pragma once
#include "a8/core/AttitudeSensor.h"
#include "a8/core/Copter.h"

namespace a8::arduino {
using namespace a8::core;
class ArduinoAttitudeSensor : public AttitudeSensor {
public:
    ArduinoAttitudeSensor();
    ~ArduinoAttitudeSensor();
    virtual void update();
    virtual float getRoll();
    virtual float getPitch();
    virtual float getYaw();
};
} // namespace a8
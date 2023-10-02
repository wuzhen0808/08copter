#pragma once
#include "a8/fc/AttitudeSensor.h"
#include "a8/fc/FlightControl.h"

namespace a8::arduino {
using namespace a8::fc;
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
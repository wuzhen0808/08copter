#pragma once
#include "a8/fcs/AttitudeSensor.h"
#include "a8/fcs/FlightControl.h"

namespace a8::arduino {
using namespace a8::fcs;
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
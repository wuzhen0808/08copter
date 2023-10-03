#pragma once
#include "a8/fc.h"

namespace a8::fc::arduino {
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
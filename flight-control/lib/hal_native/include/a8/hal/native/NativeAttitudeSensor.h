#pragma once
#include "a8/core/AttitudeSensor.h"
#include "a8/core/Copter.h"

namespace a8::hal::native {
using namespace a8::core;
class NativeAttitudeSensor : public AttitudeSensor {
public:
    NativeAttitudeSensor();
    ~NativeAttitudeSensor();
    virtual void update();
    virtual float getRoll();
    virtual float getPitch();
    virtual float getYaw();
};
} // namespace a8
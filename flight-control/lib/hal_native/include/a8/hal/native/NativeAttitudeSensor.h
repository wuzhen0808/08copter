#pragma once
#include "a8/hal/native/JSBSimIO.h"
#include "a8/core/AttitudeSensor.h"
#include "a8/core/Copter.h"

using namespace a8::core;
using namespace a8::util;
using namespace a8::hal::native;

namespace a8::hal::native {
class NativeAttitudeSensor : public AttitudeSensor {
private:
    JSBSimIO *jio;

public:
    NativeAttitudeSensor(JSBSimIO *jio);
    ~NativeAttitudeSensor();
    virtual void update();
    virtual float getRoll();
    virtual float getPitch();
    virtual float getYaw();
};
} // namespace a8::hal::native
#pragma once
#include "a8/core/AttitudeSensor.h"
#include "a8/core/Copter.h"
#include "a8/hal/native/JSBSimIO.h"
#include "a8/util/Vector3f.h"

using namespace a8::core;
using namespace a8::util;
using namespace a8::hal::native;

namespace a8::hal::native {
class NativeAttitudeSensor : public AttitudeSensor {
private:
    JSBSimIO *jio;

public:
    NativeAttitudeSensor(JSBSimIO *jio) : AttitudeSensor() {
        this->jio = jio;
    }
    ~NativeAttitudeSensor() {}
    virtual Vector3f getAngVel() override {
        return jio->getAngVel();
    }
    virtual Vector3f getAccVel() override {
        return jio->getAccVel();
    }
};
} // namespace a8::hal::native
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
    ~NativeAttitudeSensor() {

    }
    /**
     * Altitude in Meter.
    */
    virtual double getAlt() {
        return jio->getAltitude();
    }
    virtual double getAgl() {
        return jio->getAgl();
    }
    /**
     * Angular velocity in Degree.
    */
    virtual Vector3f getAngVel() override {
        return jio->getAngVel();
    }
    /**
     * Acceleration velocity in ?
    */
    virtual Vector3f getAccVel() override {
        return jio->getAccVel();
    }
};
} // namespace a8::hal::native
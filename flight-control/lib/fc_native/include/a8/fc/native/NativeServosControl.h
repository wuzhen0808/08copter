#pragma once
#include "a8/fc.h"
#include "a8/fc/FlightControl.h"
#include "a8/fc/ServosControl.h"
#include "a8/fc/native/SimOutStub.h"

namespace a8::fc::native {
using namespace a8::fc;
using namespace a8::util;


class NativeServosControl : public Servos {
    SimOutStub *stub;

public:
    NativeServosControl(int totalServos, LoggerFactory *loggerFactory) : Servos(totalServos, loggerFactory) {
    }
    ~NativeServosControl() {}

    void setStub(SimOutStub *stub) {
        this->stub = stub;
    }
    
    void setThrottleNorm(int servoId, float velocity) override {
        stub->setThrottleNorm(servoId, velocity);
    }
};

} // namespace a8::fc::native

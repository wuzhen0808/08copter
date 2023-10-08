#pragma once
#include "a8/fc.h"
#include "a8/fc/FlightControl.h"
#include "a8/fc/ServosControl.h"
#include "a8/fc/native/SimOutStub.h"

using namespace a8::fc;
using namespace a8::util;

namespace a8::fc::native {

class NativeServosControl : public ServosControl {
    SimOutStub *stub;

public:
    NativeServosControl(int totalServos, LoggerFactory *loggerFactory) : ServosControl(totalServos, loggerFactory) {
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

#pragma once
#include "a8/fc.h"
#include "a8/fc/FlightControl.h"
#include "a8/fc/ServosControl.h"
#include "a8/fc/native/NativeServo.h"
#include "a8/fc/native/SimOutStub.h"

using namespace a8::fc;
using namespace a8::util;

namespace a8::fc::native {

class NativeServosControl : public ServosControl {

private:
    SimOutStub *soStub;

public:
    NativeServosControl(int totalServos, SimOutStub *soStub) : ServosControl(totalServos) {
        this->soStub = soStub;
    }
    ~NativeServosControl() {}
    virtual Servo *createServo(int servoId) override {
        return new NativeServo(soStub, servoId);
    }
};

} // namespace a8::fc::native

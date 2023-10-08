#pragma once
#include "a8/fc.h"
#include "a8/fc/native/SimOutStub.h"

using namespace a8::util;
using namespace a8::fc;
namespace a8::fc::native {

class NativeServo : public Servo {

private:
    SimOutStub *soStub;

public:
    NativeServo(SimOutStub *soStub, int id) : Servo(id) {
        this->soStub = soStub;
    }

    ~NativeServo() {}

    void setThrottleNorm(float throttle) override {
        this->soStub->setThrottleNorm(this->id, throttle);
    }
};

} // namespace a8::fc::native

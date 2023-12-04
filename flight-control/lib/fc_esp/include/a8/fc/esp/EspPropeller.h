#pragma once
#include "a8/fc.h"

namespace a8::fc {
using namespace a8::fc;

class EspPropeller : public Propeller {
public:
    void setThrottlePwm(long pwm) override {
        
    }
};
} // namespace a8::fc
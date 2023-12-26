#pragma once
#include "a8/util.h"
#include "a8/fc/pwm/PwmCalculator.h"
namespace a8::fc {
using namespace a8::util;

class EmptyPwmCalculator : public PwmCalculator {

public:
    long calculate(int idx, float throttle) override {
        return throttle;
    }
};

} // namespace a8::fc
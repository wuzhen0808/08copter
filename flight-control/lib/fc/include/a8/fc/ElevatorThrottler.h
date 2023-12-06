#pragma once
#include "a8/fc/Pid.h"
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/Throttler.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

class ElevatorThrottler : public Throttler {
    long pwmMin = 1000;
    long pwmElevation = 0;

public:
    ElevatorThrottler(System *sys, LoggerFactory *logFac) : Throttler(logFac, "ElevatorThrottler") {
    }

    void setPwmElevation(long pwm) {
        this->pwmElevation = pwm;
    }

    int update(Context &ctx, Result &res) override {
        ctx.addPwm(pwmMin + pwmElevation);
        return 1;
    }
};
} // namespace a8::fc
#pragma once
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/throttle/Pid.h"
#include "a8/fc/throttle/Throttler.h"
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class LimitThrottler : public Throttler {

    long pwmMin;
    long pwmMax;

public:
    LimitThrottler(long pwmMin, long pwmMax, LoggerFactory *logFac) : Throttler(logFac, "LimitThrottler") {
        this->pwmMin = pwmMin;
        this->pwmMax = pwmMax;
    }
    void printHistory(int intend, String &msg) override{
        
    }
    int update(Context &ctx, Result &res) override {
        long pwmLH = ctx.propellers->getPwm(0);
        long pwmRH = ctx.propellers->getPwm(1);
        long pwmLA = ctx.propellers->getPwm(2);
        long pwmRA = ctx.propellers->getPwm(3);

        limitPwm(pwmLH, pwmMin, pwmMax);
        limitPwm(pwmRH, pwmMin, pwmMax);
        limitPwm(pwmLA, pwmMin, pwmMax);
        limitPwm(pwmRA, pwmMin, pwmMax);

        ctx.propellers->setPwm(pwmLH, pwmRH, pwmLA, pwmRA);
        return 1;
    }

    void limitPwm(long &pwm, long min, long max) {
        if (min >= 0 && pwm < min) {
            pwm = min;
        }
        if (max >= 0 && pwm > max) {
            pwm = max;
        }
    }
};
} // namespace a8::fc::throttle
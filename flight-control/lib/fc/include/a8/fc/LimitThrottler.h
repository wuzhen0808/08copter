#pragma once
#include "a8/fc/Pid.h"
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/Throttler.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

class LimitThrottler : public Throttler {

    long pwmMin = 1000;
    long pwmMax = 2000;

    long pwmMaxLH = -1;
    long pwmMaxRH = -1;
    long pwmMaxLA = -1;
    long pwmMaxRA = -1;

public:
    LimitThrottler(LoggerFactory *logFac) : Throttler(logFac, "LimitThrottler") {
    }
    void limitPwmLH(long pwm) {
        this->pwmMaxLH = pwm;
    }

    void limitPwmRH(long pwm) {
        this->pwmMaxRH = pwm;
    }
    void limitPwmLA(long pwm) {
        this->pwmMaxLA = pwm;
    }
    void limitPwmRA(long pwm) {
        this->pwmMaxRA = pwm;
    }

    int update(Context &ctx, Result &res) override {
        long pwmLH = ctx.pwmLH;
        long pwmRH = ctx.pwmRH;
        long pwmLA = ctx.pwmLA;
        long pwmRA = ctx.pwmRA;

        limitPwm(pwmLH, pwmMin, pwmMax, pwmMaxLH);
        limitPwm(pwmRH, pwmMin, pwmMax, pwmMaxRH);
        limitPwm(pwmLA, pwmMin, pwmMax, pwmMaxLA);
        limitPwm(pwmRA, pwmMin, pwmMax, pwmMaxRA);

        ctx.setPwm(pwmLH, pwmRH, pwmLA, pwmRA);
        return 1;
    }

    void limitPwm(long &pwm, long min, long max, long max2) {
        if (min >= 0 && pwm < min) {
            pwm = min;
        }
        if (max >= 0 && pwm > max) {
            pwm = max;
        }
        if (max2 >= 0 && pwm > max2) {
            pwm = max2;
        }
    }
};
} // namespace a8::fc
#pragma once
#include "a8/fc/throttle/Pid.h"
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/throttle/Throttler.h"
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class LandingThrottler : public Throttler {

    long timeBeforeLanding_ = -1;
    long timeLimitForLanding = 5 * 1000; // time interval to come down.
    long comeDownStartTimeMs = -1;

public:
    LandingThrottler(LoggerFactory *logFac) : Throttler(logFac, "SafetyThrottler") {
    }

    LandingThrottler *timeBeforeLanding(long ms) {
        this->timeBeforeLanding_ = ms;
        return this;
    }

    int update(Context &ctx, Result &res) override {
        long pwmLH = ctx.pwmLH_;
        long pwmRH = ctx.pwmRH_;
        long pwmLA = ctx.pwmLA_;
        long pwmRA = ctx.pwmRA_;
        if (timeBeforeLanding_ > 0) {

            if (this->comeDownStartTimeMs == -1) {
                if (ctx.timeMs_ - ctx.startTimeMs > timeBeforeLanding_) {
                    ctx << (String() << ";start come down.");
                    this->comeDownStartTimeMs = ctx.timeMs_;
                }
            } else {
                float comeDownProgress = (ctx.timeMs_ - comeDownStartTimeMs) / (float)timeLimitForLanding;
                if (comeDownProgress > 1) {
                    comeDownProgress = 1;
                }
                long pwmDelta = 1000 * comeDownProgress;
                ctx.addPwm(-pwmDelta);
            }
        }
        return 1;
    }
};
} // namespace a8::fc
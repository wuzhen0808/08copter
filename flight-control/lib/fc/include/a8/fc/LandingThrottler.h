#pragma once
#include "a8/fc/Pid.h"
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/Throttler.h"
#include "a8/util.h"

namespace a8::fc {
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
        long pwmLH = ctx.pwmLH;
        long pwmRH = ctx.pwmRH;
        long pwmLA = ctx.pwmLA;
        long pwmRA = ctx.pwmRA;
        if (timeBeforeLanding_ > 0) {

            if (this->comeDownStartTimeMs == -1) {
                if (ctx.timeMs - ctx.startTimeMs > timeBeforeLanding_) {
                    ctx << (String() << ";start come down.");
                    this->comeDownStartTimeMs = ctx.timeMs;
                }
            } else {
                float comeDownProgress = (ctx.timeMs - comeDownStartTimeMs) / (float)timeLimitForLanding;
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
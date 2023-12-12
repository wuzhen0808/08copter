#pragma once
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/throttle/Pid.h"
#include "a8/fc/throttle/Throttler.h"
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class LandingThrottler : public Throttler {

    long timeLimitForLanding = 2 * 1000; // time interval to come down.
    long landingStartTimeMs = -1;
    float progress = 0;

public:
    LandingThrottler(LoggerFactory *logFac) : Throttler(logFac, "SafetyThrottler") {
    }
    void printHistory(int intend, String &msg) override{
        
    }
    bool isLanded() {
        if (this->landingStartTimeMs < 0) {
            return false;
        }
        return progress > 0.99f;
    }

    int startLanding(long timeMs) {
        this->landingStartTimeMs = timeMs;
        return 1;
    }

    int update(Context &ctx, Result &res) override {

        if (this->landingStartTimeMs > 0) {
            progress = (ctx.timeMs_ - landingStartTimeMs) / (float)timeLimitForLanding;
            progress = progress > 1 ? 1.0f : progress;
            long pwmDelta = 1000 * progress;
            ctx.propellers->addPwm(-pwmDelta);
        }

        return 1;
    }
};
} // namespace a8::fc::throttle
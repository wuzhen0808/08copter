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

    void getLimitInTheory(float &minSample, float &maxSample) override {
        minSample = 0.0f;
    }

    void printHistory(int depth, History &his) override {
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
        if (A8_THROTTLE_DEBUG) {

            A8_LOG_DEBUG(logger, ">>Landing.update.");
        }
        if (this->landingStartTimeMs > 0) {
            progress = (ctx.timeMs_ - landingStartTimeMs) / (float)timeLimitForLanding;
            progress = progress > 1 ? 1.0f : progress;
            float throttleDelta = this->maxThrottle * progress;
            ctx.propellers->addThrottle(-throttleDelta);
        }
        if (A8_THROTTLE_DEBUG) {
            A8_LOG_DEBUG(logger, "<<Landing.update.");
        }
        return 1;
    }
};
} // namespace a8::fc::throttle
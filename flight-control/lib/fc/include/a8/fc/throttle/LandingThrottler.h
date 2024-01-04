#pragma once
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/throttle/Pid.h"
#include "a8/fc/throttle/Throttler.h"
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class LandingThrottler : public Throttler {

    TimeUs timeUsLimitForLanding = 2 * A8_US_PER_SEC; // time interval to come down.
    TimeUs landingStartTimeUs = 0;
    float progress = 0;

public:
    LandingThrottler(LoggerFactory *logFac) : Throttler(logFac, "SafetyThrottler") {
    }
    void setup() override {}

    bool isLanded() {
        if (this->landingStartTimeUs == 0) {
            return false;
        }
        return progress > 0.99f;
    }

    int startLanding(TimeUs timeUs) {
        this->landingStartTimeUs = timeUs;
        return 1;
    }

    int update(Throttle &ctx, Result &res) override {
        if (A8_THROTTLE_DEBUG) {
            A8_LOG_DEBUG(logger, ">>Landing.update.");
        }
        if (this->landingStartTimeUs > 0) {
            A8_LOG_DEBUG(logger, String() << ">>Landing.update,ctx.timeUs_" << ctx.timeUs_ << ",landingStartTimeUs:" << landingStartTimeUs << ",limitUs:" << timeUsLimitForLanding);
            progress = (double)(ctx.timeUs_ - landingStartTimeUs) / timeUsLimitForLanding;
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
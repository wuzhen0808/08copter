#pragma once
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/throttle/Pid.h"
#include "a8/fc/throttle/Throttler.h"
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class LimitThrottler : public Throttler {

    float maxThrottle;

public:
    LimitThrottler(float max, LoggerFactory *logFac) : Throttler(logFac, "LimitThrottler") {
        this->maxThrottle = max;
    }

    void getLimitInTheory(float &minSample, float &maxSample) override {
        limitThrottle(maxSample, 0, maxThrottle);
    }

    void printHistory(int intend, History &his) override {
    }
    int update(Context &ctx, Result &res) override {
        if (A8_THROTTLE_DEBUG) {
            A8_LOG_DEBUG(logger, ">>Limit.update.");
        }
        float thLH = ctx.propellers->getThrottle(0);
        float thRH = ctx.propellers->getThrottle(1);
        float thLA = ctx.propellers->getThrottle(2);
        float thRA = ctx.propellers->getThrottle(3);

        limitThrottle(thLH, 0, maxThrottle);
        limitThrottle(thRH, 0, maxThrottle);
        limitThrottle(thLA, 0, maxThrottle);
        limitThrottle(thRA, 0, maxThrottle);

        ctx.propellers->setThrottle(thLH, thRH, thLA, thRA);
        if (A8_THROTTLE_DEBUG) {
            A8_LOG_DEBUG(logger, "<<Limit.update.");
        }
        return 1;
    }

    void limitThrottle(float &throttle, float min, float max) {
        if (min >= 0 && throttle < min) {
            throttle = min;
        }
        if (max >= 0 && throttle > max) {
            throttle = max;
        }
    }
};
} // namespace a8::fc::throttle
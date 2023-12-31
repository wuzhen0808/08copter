#pragma once
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/throttle/Pid.h"
#include "a8/fc/throttle/Throttler.h"
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class ActiveLimitThrottler : public Throttler {
    float &activeThrottle0;
    float &activeThrottle1;
    float &activeThrottle2;
    float &activeThrottle3;

public:
    ActiveLimitThrottler(float &activeThrottle0,
                         float &activeThrottle1,
                         float &activeThrottle2,
                         float &activeThrottle3,
                         LoggerFactory *logFac) : Throttler(logFac, "ActiveLimitThrottler"),
                                                  activeThrottle0(activeThrottle0),
                                                  activeThrottle1(activeThrottle1),
                                                  activeThrottle2(activeThrottle2),
                                                  activeThrottle3(activeThrottle3) {
    }
    void setup() override {}
   
    int update(Throttle &ctx, Result &res) override {
        if (A8_THROTTLE_DEBUG) {
            A8_LOG_DEBUG(logger, ">>Limit.update.");
        }
        float thLH = ctx.propellers->getThrottle(0);
        float thRH = ctx.propellers->getThrottle(1);
        float thLA = ctx.propellers->getThrottle(2);
        float thRA = ctx.propellers->getThrottle(3);

        thLH = limitThrottle(thLH, activeThrottle0);
        thRH = limitThrottle(thRH, activeThrottle1);
        thLA = limitThrottle(thLA, activeThrottle2);
        thRA = limitThrottle(thRA, activeThrottle3);

        ctx.propellers->setThrottle(thLH, thRH, thLA, thRA);
        if (A8_THROTTLE_DEBUG) {
            A8_LOG_DEBUG(logger, "<<Limit.update.");
        }
        return 1;
    }

    float limitThrottle(float throttle, float min) {
        if (min >= 0 && throttle < min) {
            throttle = min;
        }        
        return throttle;
    }
};
} // namespace a8::fc::throttle
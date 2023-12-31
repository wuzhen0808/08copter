#pragma once
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/throttle/Pid.h"
#include "a8/fc/throttle/Throttler.h"
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class ElevatorThrottler : public Throttler {
    float &elevationThrottle;

public:
    ElevatorThrottler(float &throttle, LoggerFactory *logFac) : Throttler(logFac, "ElevatorThrottler"),
                                                                elevationThrottle(throttle) {
    }
    void setup() override {}
    void setElevationThrottle(float pwm) {
        this->elevationThrottle = pwm;
    }

    int update(Throttle &ctx, Result &res) override {
        if (A8_THROTTLE_DEBUG) {
            A8_LOG_DEBUG(logger, String() << ">>Ele.update.");
        }
        ctx.propellers->addThrottle(elevationThrottle);
        if (A8_THROTTLE_DEBUG) {
            A8_LOG_DEBUG(logger, String() << "<<Ele.update.");
        }
        return 1;
    }
};
} // namespace a8::fc::throttle
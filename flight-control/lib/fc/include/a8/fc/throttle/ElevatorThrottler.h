#pragma once
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/throttle/Pid.h"
#include "a8/fc/throttle/Throttler.h"
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class ElevatorThrottler : public Throttler {
    long pwmMin = 1000;
    long pwmElevation = 0;

public:
    ElevatorThrottler(LoggerFactory *logFac) : Throttler(logFac, "ElevatorThrottler") {
    }
    void getLimitInTheory(long &minSample, long &maxSample) override {
        minSample += pwmMin + this->pwmElevation;
        maxSample += pwmMin + this->pwmElevation;
    }
    void setPwmElevation(long pwm) {
        this->pwmElevation = pwm;
    }

    int update(Context &ctx, Result &res) override {
        A8_LOG_DEBUG(logger, String()<<">>Ele.update.");
        ctx.propellers->addPwm(pwmMin + pwmElevation);
        A8_LOG_DEBUG(logger, String()<<"<<Ele.update.");
        return 1;
    }
    void printHistory(int intend, String &msg) override {
    }
};
} // namespace a8::fc::throttle
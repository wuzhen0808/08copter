#pragma once
#include "a8/fc/throttle/MainThrottler.h"

#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

class Pilot : FlyWeight {
    System *sys;

    Propeller *propellerLH;
    Propeller *propellerRH;
    Propeller *propellerLA;
    Propeller *propellerRA;

    throttle::Context *context = 0;
    throttle::MainThrottler *throttler;

    int doUpdate(Result &res) {
        int ret = this->throttler->update(*this->context, res);
        if (ret < 0) {
            return ret;
        }
        // apply pwm .
        propellerLH->setThrottlePwm(this->context->pwmLH_);
        propellerRH->setThrottlePwm(this->context->pwmRH_);
        propellerLA->setThrottlePwm(this->context->pwmLA_);
        propellerRA->setThrottlePwm(this->context->pwmRA_);
        return 1;
    }

public:
    Pilot(Propeller *propellerLH, Propeller *propellerRH, Propeller *propellerLA, Propeller *propellerRA,
          Rpy *rpy, System *sys, LoggerFactory *logFac) : FlyWeight(logFac, "Pilot") {
        this->sys = sys;
        this->propellerLH = propellerLH;
        this->propellerRH = propellerRH;
        this->propellerLA = propellerLA;
        this->propellerRA = propellerRA;
        this->throttler = new throttle::MainThrottler(rpy, sys, logFac);
    }
    int start() {

        if (this->context != 0) {
            return -1;
        }
        long timeMs = this->sys->getSteadyTime();
        this->context = new throttle::Context(timeMs);
        return 1;
    }
    int update(Result &res) {
        long timeMs = sys->getSteadyTime();
        this->context->beforeUpdate(timeMs);
        int ret = doUpdate(res);
        log(String() << this->context);
        return ret;
    }
};

} // namespace a8::fc
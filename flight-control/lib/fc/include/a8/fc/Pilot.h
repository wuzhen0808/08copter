#pragma once
#include "a8/fc/throttle/MainThrottler.h"

#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

class Pilot : FlyWeight {
    Propeller *propellerLH;
    Propeller *propellerRH;
    Propeller *propellerLA;
    Propeller *propellerRA;

    throttle::Context *context;
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
    Config config;

protected:
    Rpy *rpy;

public:
    Pilot(Config &config, Buffer<Propeller *> propellers,
          Rpy *rpy, LoggerFactory *logFac) : FlyWeight(logFac, "Pilot") {
        this->rpy = rpy;
        this->propellerLH = propellers.get(0);
        this->propellerRH = propellers.get(1);
        this->propellerLA = propellers.get(2);
        this->propellerRA = propellers.get(3);
        this->throttler = new throttle::MainThrottler(config, rpy, logFac);
        this->context = 0;
    }
    ~Pilot() {
        delete this->throttler;
        if (this->context != 0) {
            delete this->context;
        }
    }

    int checkIfReady(Result res) {
        
        if (this->context != 0) {
            // cannot restart.
            return -1;
        }      

        return 1;
    }
    int start(long timeMs, Result &res) {
        int ret = this->checkIfReady(res);
        if (ret < 0) {
            return ret;
        }
        this->context = new throttle::Context(timeMs);
        return 1;
    }

    int landing(long timeMs) {
        return this->throttler->startLanding(timeMs);
    }

    bool isLanded() {
        return this->throttler->isLanded();
    }

    int update(long timeMs, Result &res) {

        if (this->context == 0) {
            res << "not started yet.";
            return -1;
        }
        this->context->beforeUpdate(timeMs);
        int ret = doUpdate(res);
        log(String() << this->context);
        return ret;
    }
};

} // namespace a8::fc
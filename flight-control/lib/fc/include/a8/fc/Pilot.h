#pragma once
#include "a8/fc/BalanceThrottler.h"
#include "a8/fc/ElevatorThrottler.h"
#include "a8/fc/LandingThrottler.h"
#include "a8/fc/LimitThrottler.h"
#include "a8/fc/Throttler.h"

#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

class Pilot : FlyWeight {
    System *sys;
    Buffer<Throttler *> throttlers;
    Throttler::Context *context = 0;
    Propeller *propellerLH;
    Propeller *propellerRH;
    Propeller *propellerLA;
    Propeller *propellerRA;
    long pwmElevation = 210;
    long pwmLimitLH = -1; // 1000;
    long pwmLimitRH = -1; // 1000;
    long pwmLimitLA = -1;
    long pwmLimitRA = -1;
    long flyingTimeLimitMs = 20 * 1000;

public:
    Pilot(Propeller *propellerLH, Propeller *propellerRH, Propeller *propellerLA, Propeller *propellerRA,
          Rpy *rpy, System *sys, LoggerFactory *logFac) : FlyWeight(logFac, "Pilot") {
        this->sys = sys;
        this->propellerLH = propellerLH;
        this->propellerRH = propellerRH;
        this->propellerLA = propellerLA;
        this->propellerRA = propellerRA;
        {
            ElevatorThrottler *et = new ElevatorThrottler(sys, logFac);
            et->setPwmElevation(pwmElevation);
            throttlers.append(et);
        }
        {

            BalanceThrottler *t1 = new BalanceThrottler(rpy, sys, logFac);
            throttlers.append(t1);
        }
        {

            LimitThrottler *lt = new LimitThrottler(logFac);
            throttlers.append(lt);
        }
        {

            LandingThrottler *lt = new LandingThrottler(logFac);
            lt->timeBeforeLanding(flyingTimeLimitMs);
            throttlers.append(lt);
        }
        {

            LimitThrottler *lt2 = new LimitThrottler(logFac);
            lt2->limitPwmLH(this->pwmLimitLH);
            lt2->limitPwmRH(this->pwmLimitRH);
            lt2->limitPwmLA(this->pwmLimitLA);
            lt2->limitPwmRA(this->pwmLimitRA);

            throttlers.append(lt2);
        }
    }
    int start() {

        if (this->context != 0) {
            return -1;
        }
        long timeMs = this->sys->getSteadyTime();
        this->context = new Throttler::Context(timeMs);
        return 1;
    }

    int update(Result &res) {
        int ret = -1;
        long timeMs = sys->getSteadyTime();
        this->context->beforeUpdate(timeMs);
        for (int i = 0; i < throttlers.len(); i++) {
            Throttler *th = throttlers.get(i);
            ret = th->update(*this->context, res);

            if (ret < 0) {
                break;
            }
        }
        log(String() << this->context);

        applyPwm(this->context);
        this->context->afterUpdate();
        return 1;
    }
    void applyPwm(Throttler::Context *ctx) {
        propellerLH->setThrottlePwm(ctx->pwmLH);
        propellerRH->setThrottlePwm(ctx->pwmRH);
        propellerLA->setThrottlePwm(ctx->pwmLA);
        propellerRA->setThrottlePwm(ctx->pwmRA);
    }
};

} // namespace a8::fc
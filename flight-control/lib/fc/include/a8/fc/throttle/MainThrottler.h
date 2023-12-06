#pragma once
#include "a8/fc/throttle/BalanceThrottler.h"
#include "a8/fc/throttle/Context.h"
#include "a8/fc/throttle/ElevatorThrottler.h"
#include "a8/fc/throttle/LandingThrottler.h"
#include "a8/fc/throttle/LimitThrottler.h"
#include "a8/fc/throttle/Throttler.h"

#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class MainThrottler : public Throttler {
    Buffer<throttle::Throttler *> throttlers;
    long pwmElevation = 210;
    long pwmLimitLH = -1; // 1000;
    long pwmLimitRH = -1; // 1000;
    long pwmLimitLA = -1;
    long pwmLimitRA = -1;
    long flyingTimeLimitMs = 20 * 1000;

public:
    MainThrottler(Rpy *rpy, System *sys, LoggerFactory *logFac) : Throttler(logFac, String("MainThrottle")) {
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

    int update(Context &ctx, Result &res) {
        int ret = -1;
        for (int i = 0; i < throttlers.len(); i++) {
            Throttler *th = throttlers.get(i);
            ret = th->update(ctx, res);
            if (ret < 0) {
                break;
            }
        }
        return ret;
    }
};

} // namespace a8::fc::throttle
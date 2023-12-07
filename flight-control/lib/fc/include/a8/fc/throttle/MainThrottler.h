#pragma once
#include "a8/fc/Config.h"
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
    ElevatorThrottler *elevator;
    BalanceThrottler *balance;
    LandingThrottler *landing;
    LimitThrottler *lastLimit;
    LimitThrottler *firstLimit;

public:
    MainThrottler(Config &config, Rpy *rpy, LoggerFactory *logFac) : Throttler(logFac, String("MainThrottle")) {

        elevator = new ElevatorThrottler(logFac);
        elevator->setPwmElevation(config.pwmElevation);
        throttlers.append(elevator);

        balance = new BalanceThrottler(rpy, logFac);
        balance->setup(config.pidKp, config.pidKi, config.pidKd);
        throttlers.append(balance);

        firstLimit = new LimitThrottler(logFac);
        throttlers.append(firstLimit);

        landing = new LandingThrottler(logFac);
        throttlers.append(landing);

        lastLimit = new LimitThrottler(logFac);
        lastLimit->limitPwmLH(config.pwmLimitLH);
        lastLimit->limitPwmRH(config.pwmLimitRH);
        lastLimit->limitPwmLA(config.pwmLimitLA);
        lastLimit->limitPwmRA(config.pwmLimitRA);
        throttlers.append(lastLimit);
    }
    ~MainThrottler() {
        this->throttlers.forEach<MainThrottler *>(this, [](MainThrottler *this_, throttle::Throttler *throttler) {
            delete throttler;
        });
        this->throttlers.clear();
    }
    
    bool isLanded() {
        return this->landing->isLanded();
    }
    
    int startLanding(long timeMs) {
        return this->landing->startLanding(timeMs);
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
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
    LimitThrottler *limit;

public:
    MainThrottler(Config &config, Rpy *rpy, LoggerFactory *logFac) : Throttler(logFac, String("MainThrottle")) {

        elevator = new ElevatorThrottler(logFac);
        elevator->setPwmElevation(config.pwmElevation);
        throttlers.append(elevator);

        balance = new BalanceThrottler(rpy, logFac);
        balance->setPidArgument(config.pidKp, config.pidKi, config.pidKd, config.maxBalancePidOutput, config.maxBalancePidIntegralOutput);
        throttlers.append(balance);

        limit = new LimitThrottler(config.pwmMin, config.pwmMax, logFac);
        throttlers.append(limit);

        landing = new LandingThrottler(logFac);
        throttlers.append(landing);

        throttlers.append(limit);
    }

    ~MainThrottler() {
        this->throttlers.clear();
        delete this->balance;
        delete this->elevator;
        delete this->limit;
        delete this->landing;
    }

    void printHistory(int depth, String &msg) override {
        msg << StringUtil::space(depth) << "MainThrottler-history:\n";
        this->balance->printHistory(depth + 1, msg);
    }

    bool isLanded() {
        return this->landing->isLanded();
    }

    int startLanding(long timeMs) {
        return this->landing->startLanding(timeMs);
    }

    int update(Context &ctx, Result &res) {
        int ret = -1;
        int totalPropellers = ctx.propellers->getTotalPropellers();
        for (int i = 0; i < throttlers.len(); i++) {
            Throttler *th = throttlers.get(i);

            ret = th->update(ctx, res);
            ctx.message << "(";
            for (int j = 0; j < totalPropellers; j++) {
                ctx.message << String() << ctx.propellers->getPwm(j) << ",";
            }
            ctx.message << ")";
            if (ret < 0) {
                break;
            }
        }
        return ret;
    }
};

} // namespace a8::fc::throttle
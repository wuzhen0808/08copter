#pragma once
#include "a8/fc/Context.h"
#include "a8/fc/config/Config.h"
#include "a8/fc/throttle/BalanceThrottler.h"
#include "a8/fc/throttle/ElevatorThrottler.h"
#include "a8/fc/throttle/LandingThrottler.h"
#include "a8/fc/throttle/LimitThrottler.h"
#include "a8/fc/throttle/Throttler.h"
#include "a8/util.h"

#if A8_THROTTLE_DEBUG == 1
#define A8_M_THRO_DEBUG(logger, msg) A8_LOG_DEBUG(logger, msg)
#else
#define A8_M_THRO_DEBUG(logger, msg) 
#endif

namespace a8::fc::throttle {
using namespace a8::util;

class MainThrottler : public Throttler {
    Buffer<throttle::Throttler *> throttlers;
    ElevatorThrottler *elevator;
    BalanceThrottler *balance;
    LandingThrottler *landing;
    LimitThrottler *limit;
    a8::fc::Config &config;

public:
    MainThrottler(a8::fc::Config &config, Rpy *rpy, LoggerFactory *logFac) : Throttler(logFac, String("MainThrottle")), config(config) {

        elevator = new ElevatorThrottler(logFac);
        elevator->setElevationThrottle(config.elevationThrottle);
        throttlers.append(elevator);

        balance = new BalanceThrottler(rpy, logFac);
        balance->setPidArgument(config.pidKp, config.pidKi, config.pidKd, config.pidOutputLimit, config.pidOutputLimitI);
        throttlers.append(balance);

        limit = new LimitThrottler(config.maxThrottle, logFac);
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
    void getLimitInTheory(float &minSample, float &maxSample) override {
        for (int i = 0; i < throttlers.len(); i++) {
            throttlers.get(i)->getLimitInTheory(minSample, maxSample);
        }
    }
    void printHistory(int depth, History &his) override {
        his.add(depth, "MainThrottler-history:");
        this->balance->printHistory(depth + 1, his);
    }

    bool isLanded() {
        return this->landing->isLanded();
    }

    int startLanding(long timeMs) {
        return this->landing->startLanding(timeMs);
    }

    int update(Context &ctx, Result &res) {
        int ret = -1;
        A8_M_THRO_DEBUG(logger, String()<<">>update.");
        int totalPropellers = ctx.propellers->getTotalPropellers();
        A8_M_THRO_DEBUG(logger, String()<<">>update.");
        for (int i = 0; i < throttlers.len(); i++) {
            Throttler *th = throttlers.get(i);

            ret = th->update(ctx, res);
            ctx.message << "(";
            for (int j = 0; j < totalPropellers; j++) {
                ctx.message << String() << ctx.propellers->getThrottle(j) << ",";
            }
            ctx.message << ")";
            if (ret < 0) {
                break;
            }
        }
        A8_M_THRO_DEBUG(logger, String()<<"<<update.");
        return ret;
    }
};

} // namespace a8::fc::throttle
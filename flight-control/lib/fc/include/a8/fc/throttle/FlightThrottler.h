#pragma once
#include "a8/fc/config/Config.h"
#include "a8/fc/throttle/BalanceThrottler.h"
#include "a8/fc/throttle/ElevatorThrottler.h"
#include "a8/fc/throttle/LandingThrottler.h"
#include "a8/fc/throttle/LimitThrottler.h"
#include "a8/fc/throttle/Throttle.h"
#include "a8/fc/throttle/Throttler.h"
#include "a8/util.h"

#if A8_THROTTLE_DEBUG == 1
#define A8_M_THRO_DEBUG(logger, msg) A8_LOG_DEBUG(logger, msg)
#else
#define A8_M_THRO_DEBUG(logger, msg)
#endif

namespace a8::fc::throttle {
using namespace a8::util;

class FlightThrottler : public Throttler {
    Buffer<throttle::Throttler *> throttlers;
    ElevatorThrottler *elevator;
    BalanceThrottler *balance;
    LandingThrottler *landing;
    LimitThrottler *limit;
    a8::fc::FlightConfigItem *config;

public:
    FlightThrottler(a8::fc::FlightConfigItem *config, Rpy *rpy, LoggerFactory *logFac) : Throttler(logFac, String("MainThrottler")), config(config) {
        elevator = new ElevatorThrottler(logFac);
        elevator->setElevationThrottle(config->elevationThrottle);
        throttlers.append(elevator);

        balance = new BalanceThrottler(rpy, config->balanceMode, logFac);
        balance->setPidArgument(config->pidKp, config->pidKi, config->pidKd, config->pidOutputLimit, config->pidOutputLimitI);
        throttlers.append(balance);

        limit = new LimitThrottler(config->maxThrottle, logFac);
        throttlers.append(limit);

        landing = new LandingThrottler(logFac);
        throttlers.append(landing);

        throttlers.append(limit);
    }

    ~FlightThrottler() {
        this->throttlers.clear();
        delete this->balance;
        delete this->elevator;
        delete this->limit;
        delete this->landing;
    }
    void setup() override {

        for (int i = 0; i < throttlers.len(); i++) {
            Throttler *th = throttlers.get(i, 0);
            th->setup();
        }
    }

    int collectDataItems(Collector *collector, Result &res) override {
        int ret = 1;
        for (int i = 0; ret > 0 && i < throttlers.len(); i++) {
            Throttler *th = throttlers.get(i, 0);
            ret = th->collectDataItems(collector, res);
        }
        return ret;
    }

    void getLimitInTheory(float &minSample, float &maxSample) override {
        for (int i = 0; i < throttlers.len(); i++) {
            throttlers.get(i, 0)->getLimitInTheory(minSample, maxSample);
        }
    }

    bool isLanded() {
        return this->landing->isLanded();
    }

    int startLanding(long timeMs) {
        return this->landing->startLanding(timeMs);
    }

    int update(Throttle &ctx, Result &res) {
        int ret = -1;
        A8_M_THRO_DEBUG(logger, String() << ">>update.");
        int totalPropellers = ctx.propellers->getTotalPropellers();
        A8_M_THRO_DEBUG(logger, String() << ">>update.");
        for (int i = 0; i < throttlers.len(); i++) {
            Throttler *th = throttlers.get(i, 0);

            ret = th->update(ctx, res);
            if (ret < 0) {
                break;
            }
        }
        A8_M_THRO_DEBUG(logger, String() << "<<update.");
        return ret;
    }
};

} // namespace a8::fc::throttle
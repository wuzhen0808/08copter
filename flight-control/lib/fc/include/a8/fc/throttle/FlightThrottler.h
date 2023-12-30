#pragma once
#include "a8/fc/config/Config.h"
#include "a8/fc/throttle/ActiveLimitThrottler.h"
#include "a8/fc/throttle/BalanceThrottler.h"
#include "a8/fc/throttle/ElevatorThrottler.h"
#include "a8/fc/throttle/LandingThrottler.h"
#include "a8/fc/throttle/MaxLimitThrottler.h"
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
    ActiveLimitThrottler *active;
    MaxLimitThrottler *max;
    a8::fc::FlightConfigItem *config;

public:
    FlightThrottler(a8::fc::FlightConfigItem *config, Rpy *rpy, LoggerFactory *logFac) : Throttler(logFac, String("MainThrottler")), config(config) {
        elevator = new ElevatorThrottler(config->elevationThrottle, logFac);
        throttlers.append(elevator);

        balance = new BalanceThrottler(rpy, config->balanceMode, config->pidErrorDiffMAWidth, logFac);
        balance->setPidArgument(config->pidKp, config->pidKi, config->pidKd, config->pidOutputLimit, config->pidOutputLimitI);
        throttlers.append(balance);

        active = new ActiveLimitThrottler(config->activeThrottle0, config->activeThrottle1, config->activeThrottle2, config->activeThrottle3, logFac);
        throttlers.append(active);

        landing = new LandingThrottler(logFac);
        throttlers.append(landing);

        max = new MaxLimitThrottler(config->maxThrottle, logFac);
        throttlers.append(max);
    }

    ~FlightThrottler() {
        this->throttlers.clear();
        delete this->balance;
        delete this->elevator;
        delete this->active;
        delete this->landing;
        delete this->max;
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
            if (ret < 0) {
                break;
            }
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
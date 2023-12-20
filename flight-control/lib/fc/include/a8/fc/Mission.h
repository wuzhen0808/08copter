#pragma once
#include "a8/fc/History.h"
#include "a8/fc/throttle/MainThrottler.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

class Mission : FlyWeight {
    Propellers *propellers;
    throttle::MainThrottler *throttler;
    Config &config;
    System *sys;

protected:
    Rpy *rpy;

public:
    Mission(System *sys, Config &config, Propellers *propellers,
            Rpy *rpy, LoggerFactory *logFac) : FlyWeight(logFac, "Mission"), config(config) {
        this->rpy = rpy;
        this->sys = sys;
        this->propellers = propellers;
        this->throttler = new throttle::MainThrottler(config, rpy, logFac);
    }
    ~Mission() {
        delete this->throttler;
    }

    int beforeRun(ConfigContext &cc, Result &res) {
        int ret = checkRpy(cc, res);
        if (ret < 0) {
            return ret;
        }
        float minInTheory = 0;
        float maxInTheory = 0;
        throttler->getLimitInTheory(minInTheory, maxInTheory);
        this->propellers->setLimitInTheory(minInTheory, maxInTheory);
        return ret;
    }

    int checkRpy(ConfigContext &cc, Result &res) {
        int ret = rpy->checkStable(config.stableCheckRetries, res);
        if (ret < 0) {
            res << (String() << "cannot create mission, rpy is not stable after retries:" << config.stableCheckRetries);
            return -1;
        }

        ret = rpy->checkBalance(res);
        if (ret < 0) {

            if (config.unBalanceAction == UnBalanceAction::END_OF_MISSION) {
                log("cannot create mission, it is not allowed to start with a un-balanced-rpy.");
                return -1;
            } else if (config.unBalanceAction == UnBalanceAction::ASK) {
                bool ok = ConfigItems::confirm(cc, "Start mission with UN-balanced rpy?", false);
                if (!ok) {
                    return -1;
                } // ask to start.
            } else if (config.unBalanceAction == UnBalanceAction::IGNORE) {

            } else if (config.unBalanceAction == UnBalanceAction::IGNORE_IF_SAFE) {
                Result res;
                if (checkSafetyWhenIgnoreBalance(config, res) < 0) {
                    log(res.errorMessage);
                    return -1;
                }
            } else {
                log("cannot create mission, it is not allowed to start with a un-balanced-rpy.");
                return -1;
            }
        }
        return 1;
    }

    int checkSafetyWhenIgnoreBalance(Config &config, Result &res) {
        if (config.enablePropeller) {
            res << "propeller is enabled,it's not safe to start the mission with un-balance rpy.";
            return -1;
        }
        return 1;
    }
    int run(ConfigContext &cc, History &his, Result res) {
        int ret = this->beforeRun(cc, res);
        if (ret < 0) {
            return ret;
        }
        propellers->open(config.enablePropeller);
        sys->out->println(String() << "delay ... " << config.delayBeforeStartSec << " sec before start.");
        sys->delay(config.delayBeforeStartSec * 1000);
        ret = doRun(his, res);
        propellers->close();
        return ret;
    }
    int doRun(History &his, Result res) {
        // wait 3 secs.
        sys->out->println(String() << "running ... ");

        bool landing = false;
        long startTimeMs = sys->getSteadyTime();
        long preTimeMs = startTimeMs;
        int ret = -1;
        throttle::Context tc(startTimeMs, propellers, his);
        while (true) {
            Result res;
            long timeMs = sys->getSteadyTime();
            long remainMs = config.tickTimeMs - (timeMs - preTimeMs);
            if (remainMs > 0) {
                sys->delay(remainMs);
            }
            ret = this->update(&tc, timeMs, res);

            if (ret < 0) {
                log(String() << "failed to update pilot, detail:" << res);
            }

            if (this->isLanded()) {
                break;
            }

            if (!landing) { //
                // check if need start landing.
                if (timeMs - startTimeMs > config.flyingTimeLimitSec * 1000) {
                    landing = true;
                    this->landing(timeMs);
                }
            }
            preTimeMs = timeMs;
        }
        sys->out->println(String() << "done of mission.");

        return 1;
    }

    int landing(long timeMs) {
        return this->throttler->startLanding(timeMs);
    }

    bool isLanded() {
        return this->throttler->isLanded();
    }

    int update(throttle::Context *tc, long timeMs, Result &res) {

        A8_LOG_DEBUG(logger, String() << ">>update,timeMs:" << timeMs);

        tc->startUpdate(timeMs);

        int ret = this->throttler->update(*tc, res);
        tc->message << ",Propellers:[";
        this->propellers->getAll().forEach<throttle::Context *>(tc, [](throttle::Context *c, Propeller *propeller) {
            c->message << (propeller->isEnabled() ? "Enabled" : "Disabled") << ",";
        });
        tc->message << "]";

        if (ret > 0) {
            tc->commitUpdate();
            tc->message << ",Commited.";
        } else {
            tc->message << ",Give-up.";
        }
        log(String() << tc);
        A8_LOG_DEBUG(logger, String() << "<<update,timeMs:" << timeMs);
        return ret;
    }
    void printHistory(int depth, History &his) {
        propellers->printHistory(0, his);
        his.add(depth, "Pilot-history:");
        this->throttler->printHistory(depth + 1, his);
    }
};

} // namespace a8::fc
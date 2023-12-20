#pragma once
#include "a8/fc/History.h"
#include "a8/fc/throttle/MainThrottler.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
using namespace a8::fc::throttle;

class Mission : FlyWeight {
public:
    class Context {
    private:
        Context(const Context &);
        Context &operator=(const Context &);

    public:
        ConfigContext &configContext;
        Config &config;
        Throttle &throttle;
        bool landing;
        long startTimeMs;
        long tickCostTimeMs;
        Context(Config &config, ConfigContext &configContext, Throttle &throttle) : configContext(configContext), config(config), throttle(throttle) {
            this->landing = false;
        }
        ~Context() {
            A8_DEBUG("~Mission::Context()");
        }
    };

protected:
    Rpy *rpy;
    Propellers *propellers;
    MainThrottler *throttler;
    Config &config;
    System *sys;

protected:
    int beforeRun(Context &mc, Result &res) {
        int ret = checkRpy(mc.configContext, res);
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
                    bool ok = ConfigItems::confirm(cc, "Start mission with UN-balanced rpy?", false);
                    if (!ok) {
                        log(res.errorMessage);
                        return -1;
                    } // ask to start.
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
    void checkLanding(Context &mc, long timeMs) {
        if (!mc.landing) { //
            // check if need start landing.
            if (timeMs - mc.startTimeMs > config.flyingTimeLimitSec * 1000) {
                mc.landing = true;
                this->landing(timeMs);
            }
        }
    }

    int landing(long timeMs) {
        return this->throttler->startLanding(timeMs);
    }

    bool isLanded() {
        return this->throttler->isLanded();
    }

public:
    Mission(System *sys, Config &config, Propellers *propellers,
            Rpy *rpy, LoggerFactory *logFac) : FlyWeight(logFac, "Mission"), config(config) {
        this->rpy = rpy;
        this->sys = sys;
        this->propellers = propellers;
        this->throttler = new throttle::MainThrottler(config, rpy, logFac);
    }
    ~Mission() {
        A8_DEBUG(">>~Mission()");
        delete this->throttler;
        A8_DEBUG("<<~Mission()");
    }
    int run(Context &mc, Result &res) {
        int ret = this->beforeRun(mc, res);
        if (ret < 0) {
            return ret;
        }
        propellers->open(config.enablePropeller);
        // wait 3 secs.
        sys->out->println(String() << "running ... ");
        mc.startTimeMs = sys->getSteadyTime();
        mc.throttle.reset(mc.startTimeMs);
        long timeMs = mc.startTimeMs;
        while (true) {

            mc.throttle.beforeUpdate(timeMs);
            Result res;

            int ret = this->throttler->update(mc.throttle, res);
            if (ret > 0) {
                mc.throttle.commitUpdate();
            } else {
                mc.throttle.discardUpdate();
            }

            if (ret < 0) {
                log(String() << "failed to update throttle, detail:" << res);
            }
            bool running = true;

            if (running && throttler->isLanded()) {
                running = false;
            }

            long now = sys->getSteadyTime();

            if (running && !mc.landing) {
                this->checkLanding(mc, timeMs);
            }
            mc.tickCostTimeMs = now - timeMs;

            if (!running) {
                break;
            }
            long remain = mc.config.tickTimeMs - mc.tickCostTimeMs;
            if (remain > 0) {
                sys->delay(remain);
                timeMs = now + remain;
            } else {
                timeMs = now;
            }
        }
        // end of mission.
        propellers->close();
        return ret;
    }
};

} // namespace a8::fc
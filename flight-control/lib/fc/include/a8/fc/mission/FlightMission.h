#pragma once
#include "a8/fc/config/FlightConfigItem.h"
#include "a8/fc/mission/Context.h"
#include "a8/fc/mission/Mission.h"
#include "a8/fc/pwm/VoltageCompensatePwmCalculator.h"
#include "a8/fc/throttle/FlightThrottler.h"
#include "a8/util.h"

namespace a8::fc::mission {
using namespace a8::util;
using namespace a8::fc::throttle;

class FlightMission : public Mission {
    FlightConfigItem *config;
    FlightThrottler *throttler;
    Propellers *propellers;
    Rpy *rpy;
    PwmCalculator *pwmCalculator;

public:
    FlightMission(FlightConfigItem *config, PowerManage *pm, Rpy *rpy, Propellers *propellers, System *sys, LoggerFactory *logFac) : Mission(sys, logFac, "FlightMission") {
        this->config = config;
        this->rpy = rpy;
        this->propellers = propellers;
        this->throttler = new throttle::FlightThrottler(config, rpy, logFac);
        this->pwmCalculator = new VoltageCompensatePwmCalculator(pm, logFac);
    }

    ~FlightMission() {
        delete this->pwmCalculator;
        delete this->throttler;
    }

    int setup(Result &res) override {
        throttler->setup();
        pwmCalculator->setup();
        this->propellers->setPwmCalculator(this->pwmCalculator);
        return 1;
    }
    int collectDataItems(Collector &collector, Result &res) override {
        int ret = pwmCalculator->collectDataItems(collector, res);
        return ret;
    }
    int run(Context &mc, Result &res) override {
        int ret = this->beforeRun(mc, res);
        if (ret < 0) {
            return ret;
        }
        ret = this->run2(mc, res);
        return ret;
    }
    int updateRpy(Context &mc, Result &res) {
        int retries = 0;
        while (true) {
            bool ok = rpy->update();
            if (ok) {
                break;
            }
            if (retries >= config->maxRpyUpdateRetries) {
                res << "failed to update rpy after retries:" << retries;
                return -1;
            }
            retries++;
            A8_LOG_DEBUG(logger, "<<Bal.update.failed");
        }
        return 1;
    }
    int preUpdate(Context &mc, Result &res) {
        int ret = updateRpy(mc, res);
        if (ret < 0) {
            return ret;
        }

        return ret;
    }
    int doUpdate(Context &mc, Result &res) {
        int ret = this->throttler->update(mc.throttle, res);
        if (ret < 0) {
            return ret;
        }
        mc.throttle.commitUpdate();
        return 1;
    }
    int run2(Context &mc, Result &res) {
        float minInTheory = 0;
        float maxInTheory = 0;
        throttler->getLimitInTheory(minInTheory, maxInTheory);
        this->propellers->setLimitInTheory(minInTheory, maxInTheory);
        propellers->open(config->enablePropeller);
        sys->out->println(String() << "running ... ");
        mc.throttle.reset(mc.startTimeMs);
        mc.collector.preWrite();
        mc.collector.writeHeader();
        long timeMs = sys->getSteadyTime();
        mc.startTimeMs = timeMs; // m
        int ret = -1;
        while (true) {
            Result res;
            ret = preUpdate(mc, res);
            mc.preUpdate(timeMs);
            bool running = true;
            ret = this->doUpdate(mc, res);

            if (running && throttler->isLanded()) {
                running = false;
            }

            if (running && !mc.landing) {
                this->checkLanding(ret, mc, timeMs);
            }

            mc.collector.update();

            long now = sys->getSteadyTime();
            mc.tickCostTimeMs = now - timeMs;

            // check if done of mission.
            if (!running) {
                break;
            }

            // calculate next tick time.
            long remain = config->tickTimeMs - mc.tickCostTimeMs;
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

protected:
    int beforeRun(Context &mc, Result &res) {
        int ret = checkRpy(mc.configContext, res);
        if (ret < 0) {
            return ret;
        }
        ret = propellers->isReady(res);
        if (ret < 0) {
            return ret;
        }
        return ret;
    }

    int checkRpy(ConfigContext &cc, Result &res) {
        int ret = rpy->checkStable(config->stableCheckRetries, res);
        if (ret < 0) {
            res << (String() << "cannot create mission, rpy is not stable after retries:" << config->stableCheckRetries);
            return -1;
        }

        ret = rpy->checkBalance(res);
        if (ret < 0) {

            if (config->unBalanceAction == UnBalanceAction::END_OF_MISSION) {
                log("cannot create mission, it is not allowed to start with a un-balanced-rpy.");
                return -1;
            } else if (config->unBalanceAction == UnBalanceAction::ASK) {
                bool ok = ConfigItems::confirm(cc, "Start mission with UN-balanced rpy?", false);
                if (!ok) {
                    return -1;
                } // ask to start.
            } else if (config->unBalanceAction == UnBalanceAction::IGNORE) {

            } else if (config->unBalanceAction == UnBalanceAction::IGNORE_IF_SAFE) {
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

    int checkSafetyWhenIgnoreBalance(FlightConfigItem *config, Result &res) {
        if (config->enablePropeller) {
            res << "propeller is enabled,it's not safe to start the mission with un-balance rpy.";
            return -1;
        }
        return 1;
    }
    void checkLanding(int updateRet, Context &mc, long timeMs) {
        if (!mc.landing) { //
            // check if need start landing.
            if (updateRet < 0 || timeMs - mc.startTimeMs > config->flyingTimeLimitSec * 1000) {
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
};

} // namespace a8::fc::mission
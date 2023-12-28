#pragma once
#include "a8/fc/CollectorSetup.h"
#include "a8/fc/config/FlightConfigItem.h"
#include "a8/fc/mission/Mission.h"
#include "a8/fc/pwm/VoltageCompensatePwmCalculator.h"
#include "a8/fc/throttle/FlightThrottler.h"
#include "a8/util.h"
#define A8_SIGNAL_CONTINUE (1)
#define A8_SIGNAL_GIVE_UP (2)
#define A8_SIGNAL_FG_EXIT (3)
namespace a8::fc::mission {
using namespace a8::util;
using namespace a8::fc::throttle;
using namespace a8::fc::collect;

class FlightMission : public Mission {
    class Foreground : public ConfigItem {
        FlightMission *mission;

    public:
        Foreground(FlightMission *mission) {
            this->mission = mission;
        }

        void onAttached() override {
            A8_DEBUG("Foreground::onAttached.1");

            ConfigItem *ci = this;
            
            ConfigItems::add<FlightMission *>(ci, "Continue-the-mission & Exit(foreground)", this->mission, [](FlightMission *mission, ConfigContext &cc) {
                int ok = mission->signalQueue->offer(A8_SIGNAL_CONTINUE, 200);
                if (ok < 0) {
                    cc.println("failed to offer signal to mission.");
                }
                // todo:remove this line and allow mission open more navigator;
                // solution:use dispatcher instead of reader for nav of dir.
                cc.navigator->stop(1);
            });

            ConfigItems::add<FlightMission *>(ci, "Give-up(mission) & Exit(foreground)", this->mission, [](FlightMission *mission, ConfigContext &cc) {
                if (mission->running) {

                    int ok = mission->signalQueue->offer(A8_SIGNAL_GIVE_UP, 200);
                    if (ok < 0) {
                        cc.println("failed to offer signal to mission.");
                        return;
                    }
                }
                cc.navigator->stop(1);
            });

            ConfigItems::add<FlightMission *>(ci, "Notify mission & Exit(foreground)", this->mission, [](FlightMission *mission, ConfigContext &cc) {
                if (mission->running) {
                    int ok = mission->signalQueue->offer(A8_SIGNAL_FG_EXIT, 200);
                    if (ok < 0) {
                        cc.println("failed to offer signal to mission.");
                        return;
                    }
                }
                cc.navigator->stop(1);
            });
            ci = ConfigItems::addReturn(ci, "Collector");
            {
                ConfigItems::add<FlightMission *>(ci, "Print-active-dataItems", this->mission, [](FlightMission *mission, ConfigContext &cc) {
                    mission->collector->printActiveDataItems(cc.out);
                });

                ConfigItems::add<FlightMission *>(ci, "Print-all-dataItems", this->mission, [](FlightMission *mission, ConfigContext &cc) {
                    mission->collector->printAllDataItems(cc.out);
                });

                ci = this;
            }
            A8_DEBUG("Foreground::onAttached.2");
        }
        void enter(ConfigContext &cc) override {
            ConfigItems::runNav(this->dir, cc);
        }

    }; // end of Foreground.

    FlightConfigItem *config;
    FlightThrottler *throttler;
    Propellers *propellers;
    Rpy *rpy;
    PwmCalculator *pwmCalculator;
    bool landing;
    long startTimeMs;
    long tickCostTimeMs;
    long timeMs;
    Foreground *fg;
    bool running = true;

public:
    FlightMission(FlightConfigItem *config, PowerManage *pm, Rpy *rpy, Propellers *propellers, Collector *collector, ConfigContext &configContext, Throttle &throttle, SyncQueue<int> *signalQueue, System *sys, LoggerFactory *logFac)
        : Mission(configContext, throttle, signalQueue, collector, sys, logFac, "FlightMission") {
        this->config = config;
        this->rpy = rpy;
        this->propellers = propellers;
        this->throttler = new throttle::FlightThrottler(config, rpy, logFac);
        this->pwmCalculator = new VoltageCompensatePwmCalculator(pm, logFac);
        this->fg = new Foreground(this);
        this->landing = false;
        this->tickCostTimeMs = 0;
    }

    ~FlightMission() {
        delete this->pwmCalculator;
        delete this->throttler;
        delete this->fg;
    }

    ConfigItem *getForeground() override {
        return fg;
    }

    int setup(Result &res) override {
        throttler->setup();
        pwmCalculator->setup();
        this->propellers->setPwmCalculator(this->pwmCalculator);
        int ret = this->collectDataItems(this->collector, res);
        if (ret > 0) {
            ret = CollectorSetup::setupFlight(collector, res);
        }
        return 1;
    }
    int collectDataItems(Collector *collector, Result &res) {
        int ret = 1;
        ret = pwmCalculator->collectDataItems(collector, res);
        if (ret > 0)
            throttler->collectDataItems(collector, res);
        if (ret > 0)
            ret = collector->add("timeMs", this->timeMs, res);
        if (ret > 0)
            ret = collector->add("tickCostTimeMs", this->tickCostTimeMs, res);
        if (ret > 0)
            ret = collector->add("Landing", this->landing, res);

        return ret;
    }
    int run(Result &res) override {
        // wait the signal from foreground.
        int signal = signalQueue->take();
        if (signal == A8_SIGNAL_CONTINUE) {
            // continue;
        } else if (signal == A8_SIGNAL_GIVE_UP) {
            res << "mission give up by signal of give-up.";
            return -1;
        } else if (signal == A8_SIGNAL_FG_EXIT) {
            res << "mission give up by signal of fg-exit.";
            return -1;
        } else {
            res << "unknown signal:" << signal;
            return -1;
        }

        int ret = checkRpy(configContext, res);
        if (ret < 0) {
            return ret;
        }
        ret = propellers->isReady(res);
        if (ret < 0) {
            return ret;
        }
        ret = this->run2(res);
        return ret;
    }
    int updateRpy(Result &res) {
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
    int preUpdate(Result &res) {
        throttle.preUpdate(timeMs);

        int ret = updateRpy(res);

        if (ret < 0) {
            return ret;
        }

        return ret;
    }
    int doUpdate(Result &res) {
        int ret = this->throttler->update(throttle, res);
        if (ret < 0) {
            return ret;
        }
        throttle.commitUpdate();
        return 1;
    }
    int run2(Result &res) {
        float minInTheory = 0;
        float maxInTheory = 0;
        this->throttler->getLimitInTheory(minInTheory, maxInTheory);
        this->propellers->setLimitInTheory(minInTheory, maxInTheory);
        this->propellers->open(config->enablePropeller);
        this->sys->out->println(String() << "running ... ");
        this->throttle.reset(startTimeMs);        
        this->timeMs = sys->getSteadyTime();
        this->startTimeMs = timeMs; // m
        int ret = -1;

        for (int ticks = 0; this->running; ticks++) {
            Result res;
            ret = this->preUpdate(res);
            ret = this->doUpdate(res);

            if (running && throttler->isLanded()) {
                running = false;
            }

            if (running && !landing) {
                this->checkLanding(ret, timeMs);
            }

            long now = sys->getSteadyTime();
            tickCostTimeMs = now - timeMs;
            //
            if (ticks % config->collectEveryTicks == 0) {
                collector->update();
            }

            // check if done of mission.
            if (!running) {
                break;
            }

            // calculate next tick time.
            long remain = config->tickTimeMs - tickCostTimeMs;
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
    int checkRpy(ConfigContext &cc, Result &res) {
        int ret = rpy->checkStable(config->stableCheckRetries, res);
        if (ret < 0) {
            res << (String() << "cannot create mission, rpy is not stable after retries:" << config->stableCheckRetries);
            return -1;
        }
        A8_LOG_DEBUG(logger, String() << "checkRpy.1");
        ret = rpy->checkBalance(false, res);
        A8_LOG_DEBUG(logger, String() << "checkRpy.2");
        if (ret < 0) {

            if (config->unBalanceAction == UnBalanceAction::END_OF_MISSION) {
                log("cannot create mission, it is not allowed to start with a un-balanced-rpy.");
                return -1;
            } else if (config->unBalanceAction == UnBalanceAction::ASK) {

                bool ok = ConfigItems::confirm(cc, "Start mission with UN-balanced rpy?", true);
                if (!ok) {
                    return -1;
                } // ask to start.
            } else if (config->unBalanceAction == UnBalanceAction::IGNORE) {

            } else if (config->unBalanceAction == UnBalanceAction::IGNORE_IF_SAFE) {
                Result res;
                if (checkSafetyWhenIgnoreBalance(config, res) < 0) {

                    bool ok = ConfigItems::confirm(cc, "Start mission with UN-balanced rpy?", true);
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
    void checkLanding(int updateRet, long timeMs) {
        if (!landing) { //
            // check if need start landing.
            if (updateRet < 0 || timeMs - startTimeMs > config->flyingTimeLimitSec * 1000) {
                landing = true;
                this->throttler->startLanding(timeMs);
            }
        }
    }

    bool isLanded() {
        return this->throttler->isLanded();
    }

public:
};

} // namespace a8::fc::mission
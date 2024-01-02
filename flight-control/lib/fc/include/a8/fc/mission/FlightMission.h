#pragma once
#include "a8/fc/collect/CollectorSetup.h"
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
        void onLeftFailure(ConfigContext &cc) override {
            cc.navigator->right();
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
    FlightMission(long id, FlightConfigItem *config, Factory *fac, PowerManage *pm, Imu *imu, Propellers *propellers, Collector *collector, ConfigContext &configContext, Throttle &throttle, SyncQueue<int> *signalQueue, System *sys, LoggerFactory *logFac)
        : Mission(id, configContext, throttle, signalQueue, collector, sys, logFac, "FlightMission") {
        this->config = config;
        this->rpy = new Rpy(imu, config->maxImuUpdateRetries);
        Rate tickRate = Rate::ms(config->tickTimeMs);
        if (config->rpyFilter == GyroFilter::LP1) {
            this->rpy->setFilter(fac->newLowPassFilter(config->rpyFilterCutOffHz, tickRate, 1), Lang::delete_<Filter>);
        } else if (config->rpyFilter == GyroFilter::LP2) {
            this->rpy->setFilter(fac->newLowPassFilter(config->rpyFilterCutOffHz, tickRate, 2), Lang::delete_<Filter>);
        } else {
            this->rpy->setFilter(new EmptyFilter(), Lang::delete_<Filter>);
        }

        this->propellers = propellers;
        this->throttler = new throttle::FlightThrottler(config, fac, rpy, logFac);
        this->pwmCalculator = 0;
        if (config->enableVoltageCompensate) {
            this->pwmCalculator = new VoltageCompensatePwmCalculator(pm, config->minVoltForPwmCompensate, config->maxVoltForPwmCompensate, logFac);
        } else {
            this->pwmCalculator = new EmptyPwmCalculator(pm, logFac);
        }
        if (config->enableForeground) {
            this->fg = new Foreground(this);
        } else {
            fg = 0;
        }
        this->landing = false;
        this->tickCostTimeMs = 0;
    }

    ~FlightMission() {
        delete this->pwmCalculator;
        delete this->throttler;
        if (fg != 0) {
            delete this->fg;
        }
        delete this->rpy;
    }

    long getPreStartDelaySec() override {
        return config->preStartCountDown;
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
            ret = CollectorSetup::setupFlight(config, collector, res);
        }
        return ret;
    }
    int collectDataItems(Collector *collector, Result &res) {
        int ret = 1;
        if (ret > 0) {
            this->rpy->collectDataItems(collector, res);
        }
        if (ret > 0) {
            ret = pwmCalculator->collectDataItems(collector, res);
        }
        if (ret > 0)
            ret = throttler->collectDataItems(collector, res);
        if (ret > 0)
            ret = collector->add("timeMs", this->timeMs, res);
        if (ret > 0)
            ret = collector->add("tickCostTimeMs", this->tickCostTimeMs, res);
        if (ret > 0)
            ret = collector->add("Landing", this->landing, res);
        if (ret > 0) {
            ret = collector->add("missionId", this->id, res);
        }

        return ret;
    }
    int checkFg(Result &res) {
        if (fg == 0) {
            return 1;
        }

        log(String() << "wait signal to continue.");
        int signal = signalQueue->take();
        if (signal == A8_SIGNAL_CONTINUE) {
            return 1;
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
        return 1;
    }
    int run(Result &res) override {
        // wait the signal from foreground.

        int ret = 1;
        ret = checkFg(res);

        if (ret > 0) {
            // rpy and count down.
            this->rpy->reset();

            for (int i = config->preStartCountDown; i > 0; i--) {
                ret = this->rpy->continueUpdate(sys, 1000, 4, res);
                if (ret < 0) {
                    break;
                }
                float buf[3];
                rpy->get(buf);
                log(String() << "Pre-start count down:" << i << ",roll:" << buf[0] << ",pitch:" << buf[1] << ",yaw:" << buf[2]);
            }

            ret = checkRpy(configContext, res);

            if (ret > 0) {

                ret = this->rpy->alignYaw(res);
            }
        }

        if (ret > 0) {
            ret = propellers->isReady(res);
        }
        if (ret > 0) {
            Mission::onStart(sys->getSteadyTime());
            ret = this->run2(res);
            Mission::onEnd(sys->getSteadyTime());
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
        if (config->lockPropellers) {
            this->propellers->lockAll();
        } else {
            this->propellers->unlockAll();
        }

        this->propellers->open();
        this->sys->out->println(String() << "running ... " << this->id);
        this->throttle.reset(startTimeMs);
        this->timeMs = sys->getSteadyTime();
        this->startTimeMs = timeMs; // m
        int ret = 1;

        for (int ticks = 0; this->running; ticks++) {
            Result res2;
            throttle.preUpdate(timeMs);
            int rpyUpdateRet = rpy->update(res2);

            int updateRet = this->doUpdate(res2);

            if (running && throttler->isLanded()) {
                running = false;
            }

            if (running && !landing) {
                int err = this->checkLanding(rpyUpdateRet, updateRet, timeMs);
                if (err < 0) {
                    res << res2.errorMessage;
                    ret = err;
                }
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
        int ret = rpy->getImu()->checkStable(config->stableCheckRetries, res);
        if (ret < 0) {
            res << (String() << "cannot create mission, rpy is not stable after retries:" << config->stableCheckRetries);
            return -1;
        }
        A8_LOG_DEBUG(logger, String() << "checkRpy.1");
        float deg;
        ret = rpy->getImu()->checkBalance(false, config->unBalanceDegLimit, deg, res);
        if (ret < 0) { //
            if (deg > config->unBalanceDegUpLimit && !config->lockPropellers) {
                res << String() << "cannot create mission, rpy un-balance degree(" << deg << ") exceed the up limit(" << config->unBalanceDegUpLimit << ") and the propellers are unLocked.";
                return -1;
            }

            if (config->unBalanceAction == UnBalanceAction::END_OF_MISSION) {
                res << "cannot create mission, config saying it is not allowed to start with a un-balanced-rpy.";
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
                res << "cannot create mission, it is not allowed to start with a un-balanced-rpy.";
                return -1;
            }
            res.reset();
        }
        return 1;
    }

    int checkSafetyWhenIgnoreBalance(FlightConfigItem *config, Result &res) {

        if (!config->lockPropellers) {
            res << "propeller is not locked,it's not safe to start the mission with un-balance rpy.";
            return -1;
        }
        return 1;
    }
    int checkLanding(int rpyUpdateRet, int updateRet, long timeMs) {
        int ret = 1;
        if (!landing) { //
            // check if need start landing.
            if (rpyUpdateRet < 0 || updateRet < 0) {
                ret = -1; // abnormal end.
                landing = true;
            }
            if (timeMs - startTimeMs > config->flyingTimeLimitSec * 1000) {
                ret = 1; // normal end.
                landing = true;
            }
            if (landing) {
                this->throttler->startLanding(timeMs);
            }
        }
        return ret;
    }

    bool isLanded() {
        return this->throttler->isLanded();
    }

public:
};

} // namespace a8::fc::mission
#pragma once
#include "a8/fc/Mission.h"
#include "a8/fc/PowerManage.h"
#include "a8/fc/config/Config.h"
#include "a8/util.h"
#include "a8/util/sched.h"

namespace a8::fc {
using namespace a8::util;
class Commander : public FlyWeight {
protected:
    System *sys;
    Reader *reader;
    History his;
    Scheduler *sch;

public:
    Commander(System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, "Executor") {
        this->sys = sys;
        this->reader = sys->input;
        this->sch = sch;
    }
    ~Commander() {
    }

    int doRun(long startTimeMs, Context &context, Config &config, Mission *pilot, Result res) {

        int ret = pilot->start(context, res);
        if (ret < 0) {
            return ret;
        }

        // wait 3 secs.
        sys->out->println(String() << "delay ... " << config.delayBeforeStartSec << " sec before start.");
        sys->delay(config.delayBeforeStartSec * 1000);

        bool landing = false;
        long preTimeMs = startTimeMs;
        while (true) {
            Result res;
            long timeMs = sys->getSteadyTime();
            long remainMs = config.tickTimeMs - (timeMs - preTimeMs);
            if (remainMs > 0) {
                sys->delay(remainMs);
            }
            ret = pilot->update(&context, timeMs, res);

            if (ret < 0) {
                log(String() << "failed to update pilot, detail:" << res);
            }

            if (pilot->isLanded()) {
                break;
            }

            if (!landing) { //
                // check if need start landing.
                if (timeMs - startTimeMs > config.flyingTimeLimitSec * 1000) {
                    landing = true;
                    pilot->landing(timeMs);
                }
            }
            preTimeMs = timeMs;
        }
        return 1;
    }

    virtual Rpy *getRpy() = 0;
    virtual Propellers *getPropellers() = 0;
    virtual Mission *createMission(Config &config) = 0;
    virtual PowerManage *getPowerManage() = 0;
    virtual void releaseMission(Mission *pilot) = 0;
    virtual void setup() {
    }
    int run(Result &res) {
        PowerManage *pm = this->getPowerManage();
        int ret = pm->isReady(res);
        if (ret < 0) {
            return ret;
        }
        Rpy *rpy = this->getRpy();
        Propellers *propellers = this->getPropellers();
        ret = propellers->isReady(res);
        if (ret < 0) {
            return ret;
        }
        log(">>config");
        Config config(reader, sys->out, pm, rpy, logger, his, sch);

        log(">>config.2");
        while (true) {
            Result res;
            ConfigContext cc(reader, sys->out, logger, res);
            log(">>configTree->config");
            config.enter(cc);

            log("<<configTree->config");
            if (!config.isValid()) {
                // print all the invalid ones.
                log("not valid and config again.");
                continue;
            }

            if (!config.enableStart) {
                log("cannot create mission, start is NOT enabled.");
                continue;
            }
            his.reset();
            // todo print and confirm again.
            log("todo print all config items and confirm.");
            //

            int ret = rpy->checkStable(config.stableCheckRetries, res);
            if (ret < 0) {
                log(String() << "cannot create mission, rpy is not stable after retries:" << config.stableCheckRetries);
                continue;
            }

            ret = rpy->checkBalance(res);
            if (ret < 0) {

                if (config.unBalanceAction == UnBalanceAction::END_OF_MISSION) {
                    log("cannot create mission, it is not allowed to start with a un-balanced-rpy.");
                    continue;
                } else if (config.unBalanceAction == UnBalanceAction::ASK) {
                    bool ok = ConfigItems::confirm(cc, "Start mission with UN-balanced rpy?", false);
                    if (!ok) {
                        continue;
                    } // ask to start.
                }
            }

            Mission *mission = this->createMission(config);
            long startTimeMs = sys->getSteadyTime();
            propellers->open(config.enablePropeller);
            Context context(startTimeMs, propellers, his);
            ret = this->doRun(startTimeMs, context, config, mission, res);
            A8_LOG_DEBUG(cc.logger, "after doRun()");
            propellers->printHistory(0, his);
            A8_LOG_DEBUG(cc.logger, "after doRun()2");
            mission->printHistory(0, his);
            A8_LOG_DEBUG(cc.logger, "after doRun()3");
            propellers->close();
            A8_LOG_DEBUG(cc.logger, "after doRun()5");
            releaseMission(mission);
            if (ret < 0) {
                log(res.errorMessage);
            }
            log("done of mission.");
            his.print(sys->out);
        }
        return 1;
    }
};

} // namespace a8::fc

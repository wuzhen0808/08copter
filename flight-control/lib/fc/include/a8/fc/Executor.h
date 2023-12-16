#pragma once
#include "a8/fc/config/Config.h"
#include "a8/fc/Pilot.h"
#include "a8/fc/PowerManage.h"
#include "a8/util.h"
#include "a8/util/sched.h"

namespace a8::fc {
using namespace a8::util;
class Executor : public FlyWeight {
protected:
    System *sys;
    Reader *reader;

public:
    Executor(System *sys, LoggerFactory *logFac) : FlyWeight(logFac, "Executor") {
        this->sys = sys;
        this->reader = sys->input;
    }
    ~Executor() {
    }

    int doRun(Propellers *propellers, Config &config, Pilot *pilot, Result res) {
        long startTimeMs = sys->getSteadyTime();
        propellers->enableAll(config.enablePropeller);
        throttle::Context context(startTimeMs, propellers);
        int ret = pilot->start(context, res);
        if (ret < 0) {
            return ret;
        }

        // wait 3 secs.
        sys->out->println(String() << "delay ... " << config.delayBeforeStartSec << " sec before start.");
        sys->delay(config.delayBeforeStartSec * 1000);

        bool landing = false;
        long preTimeMs = sys->getSteadyTime();
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
    virtual Pilot *createPilot(Config &config) = 0;
    virtual PowerManage *getPowerManage() = 0;
    virtual void releasePilot(Pilot *pilot) = 0;

    int run(Result &res) {
        PowerManage *pm = this->getPowerManage();
        int ret = pm->isReady(res);
        if (ret < 0) {
            return ret;
        }
        Rpy *rpy = this->getRpy();
        Propellers *propellers = this->getPropellers();
        log(">>config");
        Config config(reader, sys->out, pm, rpy, logger);
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

            if (!config.startAfterConfig) {
                log("start is disabled.");
                continue;
            }
            // todo print and confirm again.
            log("todo print all config items and confirm.");
            Pilot *pilot = this->createPilot(config);
            int ret = this->doRun(propellers, config, pilot, res);
            String msg;
            propellers->printHistory(msg);
            pilot->printHistory(0, msg);
            this->sys->out->println(msg);
            releasePilot(pilot);
            if (ret < 0) {
                log(res.errorMessage);
            }
        }
        return 1;
    }
};

} // namespace a8::fc

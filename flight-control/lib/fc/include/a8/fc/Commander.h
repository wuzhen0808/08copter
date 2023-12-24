#pragma once
#include "a8/fc/Mission.h"
#include "a8/fc/PowerManage.h"
#include "a8/fc/collect/Collector.h"
#include "a8/fc/config/Config.h"
#include "a8/util.h"
#include "a8/util/sched.h"

namespace a8::fc {
using namespace a8::util;
using namespace a8::fc::collect;
using namespace a8::fc::throttle;

class Commander : public FlyWeight {
protected:
    System *sys;
    Reader *reader;
    Scheduler *sch;
    PowerManage *pm;
    Rpy *rpy;
    Collector *collector;
    Throttler *throttler;
    Buffer<String> nameWithExprs;

public:
    Commander(PowerManage *pm, Rpy *rpy, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, "Executor") {
        this->sys = sys;
        this->reader = sys->input;
        this->sch = sch;
        this->pm = pm;
        this->rpy = rpy;

        nameWithExprs.add("timeMs");
        nameWithExprs.add("tickCostTimeMs");
        nameWithExprs.add("tickCostTimeMsAvg[avg(tickCostTimeMs)]");
        nameWithExprs.add("rowNum[rowNum()]");
        nameWithExprs.add("maxPwm[maxOf(prop0-pwm,prop1-pwm,prop2-pwm,prop3-pwm)]");
        nameWithExprs.add("avgMaxPwm[avg(maxPwm)]");
        /*
        names.add("timeMs[diff]");
        names.add("roll");
        names.add("roll[diff]");
        names.add("pitch");
        names.add("pitch[diff]");
        names.add("yaw");
        names.add("yaw[diff]");
        names.add("RollPid-err");
        names.add("RollPid-err[diff]");
        names.add("RollPid-ets");
        names.add("RollPid-ets[*1000]");
        names.add("RollPid-lmt");
        names.add("RollPid-lmtI");
        names.add("RollPid-p");
        names.add("RollPid-i");
        names.add("RollPid-d");
        names.add("RollPid-output");
        names.add("PitchPid-ets");
        names.add("PitchPid-ets*1000");
        names.add("PitchPid-err");
        names.add("PitchPid-err[diff]");
        names.add("PitchPid-lmt");
        names.add("PitchPid-lmtI");
        names.add("PitchPid-p");
        names.add("PitchPid-i");
        names.add("PitchPid-d");
        names.add("PitchPid-output");
        names.add("prop0-pwm");
        names.add("prop1-pwm");
        names.add("prop2-pwm");
        names.add("prop3-pwm");
        */
    }

    ~Commander() {
    }

    virtual Propellers *getPropellers() = 0;
    virtual Mission *createMission(Config &config) = 0;
    virtual void setup() {
    }

    int run(Result &res) {

        int ret = pm->isReady(res);
        if (ret < 0) {
            return ret;
        }

        Propellers *propellers = this->getPropellers();
        ret = propellers->isReady(res);
        if (ret < 0) {
            return ret;
        }
        log(">>config");
        Config config(reader, sys->out, pm, rpy, logger, sch);

        log(">>config.2");
        while (true) {
            Result res;
            A8_LOG_DEBUG(logger, "start config.");
            ConfigContext cc(reader, sys->out, logger, res);
            A8_LOG_DEBUG(logger, "start config.2.");
            config.enter(cc);
            A8_LOG_DEBUG(logger, "done of config.");
            if (!config.isValid()) {
                // print all the invalid ones.
                log("not valid and config again.");
                continue;
            }

            if (!config.enableStart) {
                log("cannot create mission, start is NOT enabled.");
                continue;
            }

            // todo print and confirm again.
            log("todo print all config items and confirm.");
            //
            Throttle throttle(propellers);
            OutputWriter writer(this->sys->out);
            Collector collector(&writer);
            Mission::Context mc(collector, config, cc, throttle);
            Mission *mission = this->createMission(config);

            ret = mc.collectDataItems(collector, res);
            if (ret < 0) {
                log(res.errorMessage);
                continue;
            }
            ret = rpy->collectDataItems(collector, res);
            if (ret < 0) {
                log(res.errorMessage);
                continue;
            }
            ret = pm->collectDataItems(collector, res);
            if (ret < 0) {
                log(res.errorMessage);
                continue;
            }
            ret = mission->collectDataItems(collector, res);
            if (ret < 0) {
                log(res.errorMessage);
                continue;
            }
            ret = propellers->collectDataItems(collector, res);
            if (ret < 0) {
                log(res.errorMessage);
                continue;
            }
            Buffer<String> names;
            ret = collector.addAllIfNotExists(nameWithExprs, names, res);
            if (ret < 0) {
                log(String() << ret << ":" << res.errorMessage);
                continue;
            }
            ret = collector.setup(res);
            if (ret < 0) {
                log(String() << "collector error:" << res.errorMessage);
                continue;
            }
            collector.setDefaultEnable(false);
            collector.enable(nameWithExprs, true);
            collector.enable(names, true);

            ret = mission->run(mc, res);
            delete mission;
            /*
            A8_LOG_DEBUG(cc.logger, "after run()");
            */
            if (ret < 0) {
                log(res.errorMessage);
            }
            log("done of mission.");
        } // end of while
        return 1;
    }
};

} // namespace a8::fc

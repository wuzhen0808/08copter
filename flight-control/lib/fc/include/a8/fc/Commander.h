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

            Mission *mission = this->createMission(config);
            long startTimeMs = sys->getSteadyTime();            
            ret = mission->run(cc, his, res);
            A8_LOG_DEBUG(cc.logger, "after doRun()");
            mission->printHistory(0, his);
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

#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/collect/Collector.h"
#include "a8/fc/config/Config.h"
#include "a8/fc/mission/EscCalibrateMission.h"
#include "a8/fc/mission/FlightMission.h"
#include "a8/util.h"
#include "a8/util/sched.h"
#include "a8/fc/CollectorSetup.h"

namespace a8::fc {
using namespace a8::util;
using namespace a8::fc::collect;
using namespace a8::fc::throttle;
using namespace a8::fc::mission;
class MissionEntry {
public:
    Mission *mission;
    Result *res;
    int ret;
    MissionEntry(Mission *mission) {
        this->mission = mission;
        this->res = new Result();
    }
    ~MissionEntry() {
        delete this->res;
    }
};
class Commander : public FlyWeight {
protected:
    System *sys;
    Reader *reader;
    Scheduler *sch;
    PowerManage *pm;
    Rpy *rpy;
    Collector *collector;
    Throttler *throttler;

    SyncQueue<MissionEntry *> *missionInQueue;
    SyncQueue<MissionEntry *> *missionOutQueue;
    bool running = true;

public:
    Commander(PowerManage *pm, Rpy *rpy, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, "Executor") {
        this->sys = sys;
        this->reader = sys->input;
        this->sch = sch;
        this->pm = pm;
        this->rpy = rpy;
        this->missionInQueue = this->sch->createSyncQueue<MissionEntry *>(1);
        this->missionOutQueue = this->sch->createSyncQueue<MissionEntry *>(1);
    }

    ~Commander() {
    }

    virtual Propellers *getPropellers() = 0;
    virtual void setup() {
    }

    Mission *buildNextMission(Config *config, Propellers *propellers, Throttle &throttle, OutputWriter &writer, Collector *collector) {
        Mission *mission;
        while (true) {
            Result res;
            int ret = buildNextMission(config, propellers, throttle, writer, collector, mission, res);
            if (ret < 0) {
                continue;
            }
            break;
        }
        return mission;
    }

    int buildNextMission(Config *config, Propellers *propellers, Throttle &throttle, OutputWriter &writer, Collector *collector, Mission *&mission, Result &res) {
        ConfigContext cc(reader, sys->out, logger, res);
        config->enter(cc);
        if (!config->isValid()) {
            // print all the invalid ones.
            log("not valid and config again.");
            return -1;
        }
        // todo print and confirm again.
        log("todo print all config items and confirm.");
        //

        int ret = 0;
        ret = pm->collectDataItems(collector, res);
        if (ret < 0) {
            log(res.errorMessage);
            return ret;
        }
        ret = rpy->collectDataItems(collector, res);
        if (ret < 0) {
            log(res.errorMessage);
            return ret;
        }
        ret = propellers->collectDataItems(collector, res);
        if (ret < 0) {
            return ret;
        }

        if (config->missionSelect == Config::MissionType::FLIGHT) {
            mission = new FlightMission(config->flightConfigItem, pm, rpy, propellers, collector, cc, throttle, sys, loggerFactory);

        } else if (config->missionSelect == Config::MissionType::ESC_CALIBRATE) {
            mission = new EscCalibrateMission(propellers, collector, cc, throttle, sys, loggerFactory);
        } else {
            res << String() << "no such mission with type:" << config->missionSelect;
            return -1;
        }

        ret = postBuildMission(mission, collector, res);
        if (ret < 0) {
            delete mission;
            mission = 0;
        }
        return ret;
    }
    int postBuildMission(Mission *mission, Collector *collector, Result &res) {
        int ret = 0;
        ret = mission->setup(res);
        if (ret > 0)
            ret = mission->collectDataItems(collector, res);
        if (ret > 0)
            ret = CollectorSetup::setup(collector, res);
        return ret;
    }

    void missionLoop() {
        while (this->running) {
            MissionEntry *me;
            int ret = this->missionInQueue->take(me, 200);
            if (ret < 0) {
                continue;
            }
            me->ret = me->mission->run(*me->res);
            this->missionOutQueue->offer(me);
        }
    }

    int run(Result &res) {
        A8_TRACE(">>Commander::run()");
        int ret = pm->isReady(res);
        if (ret < 0) {
            log(res.errorMessage);
            return ret;
        }
        A8_TRACE(">>Commander::run()1");
        Propellers *propellers = this->getPropellers();
        A8_TRACE(">>Commander::run()2");
        Directory<ConfigItem *> *root = new Directory<ConfigItem *>("Root", 0);
        Config *config = new Config(reader, sys->out, pm, rpy, sch);
        config->attach(root);
        A8_TRACE(">>config->2");
        this->sch->createTask<Commander *>("MissionQueueRunner", this, [](Commander *this_) {
            this_->missionLoop();
        });
        while (true) {
            Throttle throttle(propellers);
            OutputWriter writer(this->sys->out);
            Collector collector(&writer);
            Mission *mission = this->buildNextMission(config, propellers, throttle, writer, &collector);
            ConfigItem *fg = mission->getForeground();
            MissionEntry *me = new MissionEntry(mission);
            this->missionInQueue->offer(me);
            Directory<ConfigItem *> *fgDir;
            if (fg != 0) {
                fgDir = new Directory<ConfigItem *>("Foreground", 0);
                fg->attach(fgDir);
                ConfigContext cc(reader, sys->out, logger, res);
                fg->enter(cc);
                delete fg;
                delete fgDir;
            }

            // waiting mission done.
            log(String() << "waiting mission done.");
            me = missionOutQueue->take();
            /*
            A8_LOG_DEBUG(cc.logger, "after run()");
            */
            if (me->ret < 0) {
                log(String() << "fail of mission, detail:" << me->res->errorMessage);
            } else {
                log("done of mission.");
            }
            delete me->mission;
            delete me;
        }                      // end of while
        this->running = false; // no more mission to run.
        delete config;
        delete root;
        return 1;
    }
};

} // namespace a8::fc

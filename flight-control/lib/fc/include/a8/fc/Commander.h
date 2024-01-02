#pragma once
#include "a8/fc/Factory.h"
#include "a8/fc/Imu.h"
#include "a8/fc/PowerManage.h"
#include "a8/fc/collect/Collector.h"
#include "a8/fc/config/Config.h"
#include "a8/fc/mission/EscCalibrateMission.h"
#include "a8/fc/mission/FlightMission.h"
#include "a8/util.h"
#include "a8/util/sched.h"

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
    Propellers *propellers;
    PowerManage *pm;
    Imu *imu;
    Collector *collector;
    Throttler *throttler;

    SyncQueue<MissionEntry *> *missionInQueue;
    SyncQueue<MissionEntry *> *missionOutQueue;
    bool running = true;
    SyncQueue<int> *signalQueue;

    SyncQueue<Collector *> *collectorInQueue;
    SyncQueue<Collector *> *collectorOutQueue;

    SyncQueue<double *> *collectQueue;
    Factory *fac;

    int missionTaskPriority = 10;
    int collectorTaskPriority = 1;

    long missionId = 0;

    Config *config;

public:
    Commander(Config *config, Factory *fac, PowerManage *pm, Imu *imu, Scheduler *sch, System *sys, LoggerFactory *logFac) : FlyWeight(logFac, "Commander") {
        this->config = config;
        this->fac = fac;
        this->sys = sys;
        this->reader = sys->input;
        this->sch = sch;
        this->pm = pm;
        this->imu = imu;
        this->propellers = new Propellers(fac, 17, 18, 19, 20, loggerFactory);
        this->missionInQueue = this->sch->createSyncQueue<MissionEntry *>(1);
        this->missionOutQueue = this->sch->createSyncQueue<MissionEntry *>(1);
        this->signalQueue = this->sch->createSyncQueue<int>(1);
        this->collectQueue = this->sch->createSyncQueue<double *>(1024);
        this->collectorInQueue = this->sch->createSyncQueue<Collector *>(1);
        this->collectorOutQueue = this->sch->createSyncQueue<Collector *>(1);
    }

    ~Commander() {
        delete this->missionInQueue;
        delete this->missionOutQueue;
        delete this->signalQueue;
        delete this->collectQueue;
        delete this->collectorInQueue;
        delete this->collectorOutQueue;
        delete this->propellers;
    }

    void setup() {
        this->propellers->setup();
    }

    int buildNextMission(Config *config, ConfigContext &cc, Propellers *propellers, Throttle &throttle, OutputWriter &writer, Collector *collector, Mission *&mission, Result &res) {

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
        ret = imu->collectDataItems(collector, res);
        if (ret < 0) {
            log(res.errorMessage);
            return ret;
        }
        ret = propellers->collectDataItems(collector, res);
        if (ret < 0) {
            return ret;
        }
        A8_DEBUG("buildNextMission.2");
        if (config->missionSelect == Config::MissionType::FLIGHT) {
            mission = new FlightMission(missionId++, config->flightConfigItem, fac, pm, imu, propellers, collector, cc, throttle, signalQueue, sys, loggerFactory);
        } else if (config->missionSelect == Config::MissionType::ESC_CALIBRATE) {
            mission = new EscCalibrateMission(missionId++, pm, propellers, collector, cc, throttle, signalQueue, sys, loggerFactory);
        } else {
            res << String() << "no such mission with type:" << config->missionSelect;
            return -1;
        }
        A8_DEBUG2("buildNextMission.3", ret);
        // clear signal queue.
        signalQueue->clear();
        A8_DEBUG2("buildNextMission.4", ret);
        ret = mission->setup(res);
        if (ret < 0) {
            delete mission;
            mission = 0;
        }
        A8_DEBUG2("buildNextMission.5", ret);
        return ret;
    }

    void missionLoop() {
        while (this->running) {
            MissionEntry *me;
            int ret = this->missionInQueue->take(me, 200);
            if (ret < 0) {
                continue;
            }
            Collector *collector = me->mission->getCollector();
            this->collectorInQueue->offer(collector);
            // run mission.
            me->ret = me->mission->run(*me->res);
            // close collector.
            collector->close();
            // waiting collector done.
            this->collectorOutQueue->take();
            // notify mission done.
            this->missionOutQueue->offer(me);
        }
    }

    void collectorLoop() {
        while (this->running) {
            Result res;
            Collector *col;
            int ret = this->collectorInQueue->take(col, 200);
            if (ret < 0) {
                continue;
            }
            ret = col->run(res);
            if (ret < 0) {
                log(String() << "failed to run collector,detail:" << res.errorMessage);
            }
            // notify collector done.
            collectorOutQueue->offer(col);
        }
    }

    int run(Result &res) {
        A8_TRACE(">>Commander::run()");
        int ret = pm->isReady(res);
        if (ret < 0) {
            log(res.errorMessage);

            if (!ConfigItems::confirm(this->reader, this->sys->out, "Power is not ready, continue or break the commander?", false, this->logger)) {

                return ret;
            }
        }
        A8_TRACE(">>Commander::run()1");

        A8_TRACE(">>config->2");
        this->sch->createTask<Commander *>("MissionQueueRunner", missionTaskPriority, this, [](Commander *this_) {
            this_->missionLoop();
        });

        this->sch->createTask<Commander *>("CollectorRunner", collectorTaskPriority, this, [](Commander *this_) {
            this_->collectorLoop();
        });

        while (true) {
            Throttle throttle(propellers);
            OutputWriter writer(this->sys->out);
            Collector collector(collectQueue, &writer);

            ConfigContext cc(reader, sys->out, logger, res);
            Mission *mission = 0;
            Result res;
            int ret = this->buildNextMission(config, cc, propellers, throttle, writer, &collector, mission, res);
            if (ret < 0) {
                // failed to build mission, build next .
                sys->out->println(res.errorMessage);
                continue;
            }
            ConfigItem *fg = mission->getForeground();
            MissionEntry *me = new MissionEntry(mission);
            this->missionInQueue->offer(me);

            Directory<ConfigItem *> *fgDir;
            if (fg != 0) {
                fgDir = new Directory<ConfigItem *>(String() << "Foreground(" << mission->getName() << ")", 0);
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
                log(String() << "done of mission(" << me->mission->getId() << "), time cost in sec:" << (mission->getTimeCost() / 1000.0f));
            }
            delete me->mission;
            delete me;
        }                      // end of while
        this->running = false; // no more mission to run.
        return 1;
    }
};

} // namespace a8::fc

#pragma once
#include "a8/fc/Config.h"
#include "a8/fc/FcConfigTree.h"

#include "a8/fc/Pilot.h"
#include "a8/util.h"
#include "a8/util/sched.h"

namespace a8::fc {
using namespace a8::util;
class Executor : public FlyWeight {
protected:
    System *sys;
    ConfigTree *configTree;
    LineReader *reader;

public:
    Executor(System *sys, LoggerFactory *logFac) : FlyWeight(logFac, "Executor") {
        this->sys = sys;
        this->reader = new LineReader(sys->input);
        this->configTree = new FcConfigTree(reader, sys->out, 0, "Root", logFac);
    }
    ~Executor() {
        delete this->reader;
    }

    void getRpyReady(Rpy *rpy, FcConfigTree *configTree) {

        while (true) { // stable check.
            Result res;
            int stable = rpy->checkIfStable(res);
            if (stable < 0) {
                log(res.errorMessage);
                String prompt = "Where to go?";
                String option0 = "Force start with UN-stable rpy? ";
                String option1 = "Check again.";
                if (0 == configTree->select(prompt, option0, option1, 0)) {
                    break;
                }
            }
            log("rpy is stable.");
            break;
        }

        while (true) {
            Result res;
            int balance = rpy->checkIfBalance(res);

            if (balance < 0) {
                log(res.errorMessage);

                if (0 == configTree->select("Where to go?",
                                            String() << "Force start with the UN-balanced rpy(Roll:" << rpy->getRoll() << ",Pitch:" << rpy->getPitch() << ")", //
                                            "Adjust the attitude of the quad copter and try again.", 0)) {
                    break;
                }
            }
            log("rpy is balanced.");
            break;
        }
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
    virtual void releasePilot(Pilot *pilot) = 0;

    void run() {
        Config config;
        Rpy *rpy = this->getRpy();
        Propellers *propellers = this->getPropellers();

        FcConfigTree *configTree = new FcConfigTree(reader, sys->out, config, loggerFactory);
        while (true) {
            this->getRpyReady(rpy, configTree);
            Result res;
            log(">>configTree->config");
            int ret = configTree->config(this->logger, res);
            log("<<configTree->config");
            if (ret < 0) {
                continue;
            }
            Pilot *pilot = this->createPilot(config);
            ret = this->doRun(propellers, config, pilot, res);
            String msg;
            propellers->printHistory(msg);
            pilot->printHistory(0, msg);
            this->sys->out->println(msg);
            releasePilot(pilot);
            if (ret < 0) {
                log(res.errorMessage);
            }
        }
        delete configTree;
    }
};

} // namespace a8::fc

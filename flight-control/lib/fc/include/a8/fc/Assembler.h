

#pragma once
#include "a8/fc/Commander.h"
#include "a8/fc/Factory.h"
#include "a8/fc/GlobalVars.h"
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/util/comp.h"

namespace a8::fc {
class Assembler : public Component {

protected:
    Commander *commander;

    PowerManage *pm;
    Factory *fac;
    Directory<ConfigItem *> *root;
    Imu *imu;
    Config *config;
    GlobalVars vars;

public:
    Assembler(Factory *fac) : Component("Assembler") {
        this->fac = fac;
    }

    virtual void populate(StagingContext *sc) override {
        log(">>populate.");
        Component::populate(sc);
        fac->populate(sc);
        System *sys = sc->getSys();
        this->pm = new PowerManage(sys, 5, 21, loggerFactory);
        imu = fac->newImu();
        config = new Config(vars, sys->input, sys->out, pm, imu, sc->scheduler);
        root = new Directory<ConfigItem *>("Root", 0);
        config->attach(root);
        this->commander = new Commander(config, fac, pm, imu, sc->scheduler, sc->getSys(), loggerFactory);
        log("<<populate.");
    }

    virtual void setup(StagingContext *sc) override {
        log(">>setup...");
        this->fac->setupWire();
        log(">>setup pm.");
        this->pm->setup();
        log(">>setup imu.");
        this->imu->setup();
        log(">>setup commander.");
        this->commander->setup();
        log("<<setup done.");
    }
    virtual void postShutdown(StagingContext *sc) override {
    }

    virtual void start(StagingContext *sc) override {
        log(">>start.");
        Scheduler *sch = sc->scheduler;
        Component::scheduleHz1<Assembler>([](TickingContext *tc, Assembler *this_) {
            this_->hz1(tc);
        });
        Component::scheduleHz100<Assembler>([](TickingContext *tc, Assembler *this_) {
            this_->hz100(tc);
        });
        Component::scheduleHz500<Assembler>([](TickingContext *tc, Assembler *this_) {
            this_->hz500(tc);
        });

        Component::schedule<Assembler>([](TickingContext *tc, Assembler *this_) {
            this_->run(tc);
        });
        log("<<start.");
    }
    void hz1(TickingContext *tc) {
        // this->rpy->tick();
    }
    void hz500(TickingContext *tc) {
        TimeUs timeUs = tc->getSteadyTimeUs();
        // this->pm->tick(timeMs);
    }

    void hz100(TickingContext *tc) {
        TimeUs timeUs = tc->getSteadyTimeUs();
        this->pm->tick(timeUs);
    }

    void run(TickingContext *tc) {
        System *sys = tc->getSys();
        while (true) {
            Result res;
            int ret = this->commander->run(res);
            if (ret < 0) {
                log(res.errorMessage);
            }
            sys->delay(5000);
        }
        log("done of application.");
    }
};

} // namespace a8::fc
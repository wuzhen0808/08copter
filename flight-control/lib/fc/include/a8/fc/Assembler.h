#pragma once
#include "a8/fc/Commander.h"
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/util/comp.h"

namespace a8::fc {
class Assembler : public Component {

protected:
    Commander *executor = 0;
    Rpy *rpy = 0;
    PowerManage *pm;

public:
    Assembler() : Component("Assembler") {
    }

    virtual void populate(StagingContext *sc) override {
        Component::populate(sc);
        this->pm = new PowerManage(sc->getSys(), 5, 21, loggerFactory);
    }

    virtual void setup(StagingContext *sc) override {
        this->pm->setup();
    }
    virtual void postShutdown(StagingContext *sc) override {
    }

    virtual void start(StagingContext *sc) override {
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
    }
    void hz1(TickingContext *tc) {
        // this->rpy->tick();
    }
    void hz500(TickingContext *tc) {
        long timeMs = tc->getSteadyTime();
        // this->pm->tick(timeMs);
    }

    void hz100(TickingContext *tc) {
        long timeMs = tc->getSteadyTime();
        this->pm->tick(timeMs);
    }

    void run(TickingContext *tc) {
        System *sys = tc->getSys();
        while (true) {
            while (true) {
                Result res;
                int ret = pm->isReady(res);
                if (ret < 0) {
                    log(res.errorMessage);
                    sys->delay(1000);
                    continue;
                }
                break;
            }
            log("calling executor to run.");
            Result res;
            int ret = this->executor->run(res);
            if (ret < 0) {
                log(res.errorMessage);
            }
            sys->delay(5000);
        }
        log("done of application.");
    }
};

} // namespace a8::fc
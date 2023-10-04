#pragma once
#include "a8/util.h"
#include "a8/util/comp/Component.h"
#include "a8/util/comp/StagingContext.h"
#include "a8/util/comp/TickingContext.h"
#include "a8/util/sys.h"
#include "a8/util/thread.h"

using namespace a8::util;
using namespace a8::util::thread;
using namespace a8::util::sys;
namespace a8::util::comp {

class TickRunner {
public:
    static void tickFunc(void *me) {
        TickRunner *rr = static_cast<TickRunner *>(me);
        rr->tick();
    }

    Rate rate;
    Timer *timer;
    Thread *thread;
    Buffer<Component *> *components;
    TickingContext *ticking;
    System *sys;

    TickRunner(Rate &rate, System *sys) {
        this->sys = sys;
        this->rate = rate;
        this->components = new Buffer<Component *>();
        this->ticking = new TickingContext();
    }

    void add(Component *component) {
        components->append(component);
    }

    void tick() {

        this->ticking->startTimestamp = sys->millis();
        bool isInThread = this->rate.isForEver();

        for (int i = 0; i < components->length(); i++) {
            Component *comp = components->get(i);
            if (isInThread) {
                comp->run(ticking);
            } else {
                comp->tick(ticking);
            }
        }
    }

    void run(StagingContext *context) {
        if (rate.isZero()) {
            return;
        }
        if (!rate.isForEver()) {
            this->timer = context->scheduler->scheduleTimer(TickRunner::tickFunc, this, this->rate);
            return;
        }
        for (int j = 0; j < components->length(); j++) {
            Component *com = components->get(j);
            TickRunner *runner = new TickRunner(this->rate, this->sys);
            runner->add(com);
            runner->thread = context->scheduler->schedule(tickFunc, runner);
            com->log(String() << "a new thread scheduled for comp:" << com);
            // todo storage of thread?
        }

        return;
    }
};

} // namespace a8::util::comp
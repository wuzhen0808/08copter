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

    Timer *timer;
    Thread *thread;
    Buffer<Component *> *components;
    TickingContext *ticking;

public:
    static void tickFunc(void *me) {
        TickRunner *rr = static_cast<TickRunner *>(me);
        rr->tick();
    }

    TickRunner(TickingContext *ticking) {
        this->ticking = ticking;
        this->components = new Buffer<Component *>();
    }
    TickingContext *getTicking() {
        return this->ticking;
    }
    void add(Component *component) {
        components->append(component);
    }

    void tick() {

        this->ticking->beforTick();
        bool isInThread = this->ticking->getRate().isRun();

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
        Rate rate = ticking->getRate();
        if (ticking->getRate().isZero()) {
            return;
        }
        if (!ticking->getRate().isRun()) {
            this->timer = context->scheduler->scheduleTimer(TickRunner::tickFunc, this, rate);
            return;
        }
        for (int j = 0; j < components->length(); j++) {
            Component *com = components->get(j);
            TickRunner *runner = new TickRunner(this->ticking);
            runner->add(com);
            runner->thread = context->scheduler->schedule(tickFunc, runner);
            com->log(String() << "a new thread scheduled for component:" << com);
            // todo storage of thread?
        }

        return;
    }
};

} // namespace a8::util::comp
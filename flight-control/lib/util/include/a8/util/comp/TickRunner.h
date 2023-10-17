#pragma once
#include "a8/util.h"
#include "a8/util/comp/Component.h"
#include "a8/util/comp/StagingContext.h"
#include "a8/util/comp/TickingContext.h"
#include "a8/util/System.h"
#include "a8/util/thread.h"

using namespace a8::util;
using namespace a8::util::thread;
namespace a8::util::comp {

class TickRunner {
    using tickHandle = void (*)(TickingContext *, void *, void *);//ticking, component, handle

    Timer *timer;
    Thread *thread;
    Buffer<Component::TickEntry *> *entries;
    TickingContext *ticking;

public:
    static void tickFunc(void *me) {
        TickRunner *rr = static_cast<TickRunner *>(me);
        rr->tick();
    }

    TickRunner(TickingContext *ticking) {
        this->ticking = ticking;
        this->entries = new Buffer<Component::TickEntry *>();
    }
    TickingContext *getTicking() {
        return this->ticking;
    }
    void add(Component::TickEntry *entry) {
        entries->append(entry);
    }

    void tick() {

        this->ticking->beforTick();
        bool isInThread = this->ticking->getRate().isRun();

        for (int i = 0; i < entries->length(); i++) {
            Component::TickEntry *entry = entries->get(i);
            if (isInThread) {
                entry->tickHandle(ticking, entry->component, entry->handle);
            } else {
                entry->tickHandle(ticking, entry->component, entry->handle);
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
        for (int j = 0; j < entries->length(); j++) {
            Component::TickEntry *entry = entries->get(j);
            TickRunner *runner = new TickRunner(this->ticking);
            runner->add(entry);
            runner->thread = context->scheduler->schedule(tickFunc, runner);            
            // todo storage of thread?
        }

        return;
    }
};

} // namespace a8::util::comp
#pragma once
#include "a8/util.h"
#include "a8/util/System.h"
#include "a8/util/comp/Component.h"
#include "a8/util/comp/StagingContext.h"
#include "a8/util/comp/TickingContext.h"
#include "a8/util/schedule.h"

namespace a8::util::comp {
using namespace a8::util;
using namespace a8::util::schedule;

class TickRunner : public FlyWeight {
    using tickHandle = void (*)(TickingContext *, void *, void *); // ticking, component, handle

    Timer *timer;
    Thread *thread;
    Buffer<Component::TickEntry *> *entries;
    TickingContext *ticking;

public:
    TickRunner(StagingContext *staging, Rate rate, int group) : FlyWeight(staging->loggerFactory) {
        this->ticking = new TickingContext(staging, rate, group);
        this->entries = new Buffer<Component::TickEntry *>();
    }
    TickingContext *getTicking() {
        return this->ticking;
    }
    void add(Component::TickEntry *entry) {
        entries->append(entry);
    }

    void tick() {

        logger->info(String() << ">>TickRunner::tick(),rate:" << this->ticking->getRate().Hz());

        this->ticking->beforTick();

        for (int i = 0; i < entries->length(); i++) {
            Component::TickEntry *entry = entries->get(i);
            entry->tickHandle(ticking, entry->component, entry->handle);
        }

        if (this->ticking->ret < 0) {
            logger->warn(this->ticking->rst.errorMessage);
        }

        this->ticking->afterTick();
        logger->info("<<TickRunner::tick()");
    }

    void start(StagingContext *context) {
        logger->info(">>TickRunner::start()");
        void (*tickFunc)(void *) = [](void *this_) {
            TickRunner *this__ = Lang::cast<TickRunner *>(this_);
            this__->tick();
        };
        Rate rate = ticking->getRate();
        if (ticking->getRate().isZero()) {
            return;
        }
        if (ticking->getRate().isRun()) {
            // start 1 thread for each ticking.
            for (int j = 0; j < entries->length(); j++) {
                Component::TickEntry *entry = entries->get(j);
                TickRunner *runner = new TickRunner(this->ticking->getStaging(), entry->rate, j);
                runner->add(entry);
                logger->info(String() << "schedule thread for runner:" << j);
                runner->thread = context->scheduler->schedule(runner, tickFunc);
                // todo storage of thread?
            }
        } else {

            logger->info(String() << "scheduleTimer:rate:" << rate);
            this->timer = context->scheduler->scheduleTimer(tickFunc, this, rate);
        }

        return;
    }
};

} // namespace a8::util::comp
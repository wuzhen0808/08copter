#pragma once
#include "a8/util/Callback.h"
#include "a8/util/Rate.h"
#include "a8/util/Runnable.h"
#include "a8/util/sched/Lock.h"
#include "a8/util/sched/Semaphore.h"
#include "a8/util/sched/SyncQueue.h"
#include "a8/util/sched/Thread.h"
#include "a8/util/sched/Timer.h"

#include "a8/util/sched/defines.h"

namespace a8::util::sched {
using run = void (*)(void *);

template <typename T>
class WrapperSyncQueue : public SyncQueue<T> {
public:
    SyncQueue<void *> *wrap;
    WrapperSyncQueue(SyncQueue<void *> *wrap) {
        this->wrap = wrap;
    }
    ~WrapperSyncQueue() {
        delete this->wrap;
    }

    int take(T &ele, long timeout) {
        T ele2;
        void *ele2P = &ele2;
        int ret = wrap->take(ele2P, timeout);
        if (ret < 0) {
            return ret;
        }
        ele = ele2;
        return ret;
    }
    int peek(T &ele, long timeout) {
        T ele2;
        void *ele2P = &ele2;
        int ret = wrap->peek(ele2P, timeout);
        if (ret < 0) {
            return ret;
        }
        ele = ele2;
        return ret;
    }

    int offer(T ele, long timeout) {
        return wrap->offer(&ele, timeout);
    }
};
class Scheduler {

protected:
    // the internal implementation should be move to sub class.
    virtual SyncQueue<void *> *doCreateSyncQueue(int cap, int itemSize) = 0;

public:
    Scheduler() {}
    ~Scheduler() {}

    virtual void startSchedule() = 0;
    virtual void endSchedule() = 0;

    template <typename T>
    Thread *createTask(T context, void (*run)(T)) {
        struct Params {
            T context_;
            void (*run_)(T);
        };
        Params *p = new Params();
        p->context_ = context;
        p->run_ = run;
        return schedule(p, [](void *vp) {
            Params *pp = static_cast<Params *>(vp);
            pp->run_(pp->context_);
            delete pp;
        });
    }

    virtual Thread *schedule(void *context, sched::run run) = 0;

    virtual Timer *scheduleTimer(sched::run run, void *context, const Rate &rate) = 0;
    template <typename T>
    Timer *createTimer(const Rate &rate, T c, void (*run)(T c)) {
        struct Params {
            T context_;
            void (*run_)(T);
        };
        Params *p = new Params();
        p->context_ = c;
        p->run_ = run;
        return scheduleTimer(rate, p, [](void *pv) {
            Params *pp = static_cast<Params *>(pv);
            pp->run_(pp->context_);
        });
    }
    
    Timer *scheduleTimer(const Rate &rate, void *context, sched::run run) {
        return scheduleTimer(run, context, rate);
    }
    template <typename T>
    SyncQueue<T> *createSyncQueue(int cap) {
        SyncQueue<void *> *wrap = this->doCreateSyncQueue(cap, sizeof(T));
        WrapperSyncQueue<T> *rt = new WrapperSyncQueue<T>(wrap);
        return rt;
    }

    virtual Semaphore *createSemaphore(int cap, int initial) = 0;

    Lock *createLock() {
        Semaphore *sem = this->createSemaphore(1, 1);
        return new Lock(sem);
    }
};
} // namespace a8::util::sched

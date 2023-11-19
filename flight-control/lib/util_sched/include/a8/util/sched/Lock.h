#pragma once
#include "a8/util/sched/SyncQueue.h"
namespace a8::util::sched {
class Lock {
    SyncQueue<int> *queue;

public:
    Lock(SyncQueue<int> *queue) {
        this->queue = queue;
        this->unLock();
    }

    ~Lock() {
        delete this->queue;
    }

    template <typename C>
    void runInLock(C c, void (*run)(C c)) {
        lock();
        run(c);
        unLock();
    }

    template <typename C, typename R>
    R callInLock(C c, R (*call)(C c)) {
        lock();
        R ret = call(c);
        unLock();
        return ret;
    }

    void unLock() {
        this->queue->offer(1);
    }

    void lock() {
        this->queue->take();
    }
};
} // namespace a8::util::sched
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

    template <typename C, typename C2, typename R>
    R callInLock(C c, C2 c2, R (*call)(C c, C2 c2)) {
        lock();
        R ret = call(c, c2);
        unLock();
        return ret;
    }


    template <typename C, typename C2, typename C3, typename R>
    R callInLock(C c, C2 c2,C3 c3, R (*call)(C c, C2 c2, C3 c3)) {
        lock();
        R ret = call(c, c2, c3);
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
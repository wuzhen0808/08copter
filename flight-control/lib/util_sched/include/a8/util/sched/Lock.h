#pragma once
#include "a8/util/sched/Semaphore.h"
namespace a8::util::sched {
class Lock {
    Semaphore *sem;

public:
    Lock(Semaphore *sem) {
        this->sem;
    }
    ~Lock() {
        delete this->sem;
    }

    template <typename C>
    int lockAndRun(C c, void (*run)(C c), long timeout, Result &res) {
        int ret = lock(timeout, res);
        if (ret == 0) {
            return 0;
        }
        if (ret < 0) {
            res << ";failed to lock.";
            return -1;
        }
        run(c);
        ret = unLock(res);
        if (ret < 0) {
            res << ";failed to unlock after locked and ran the function.";
            return -2;
        }

        return 1;
    }

    int unLock(Result &res) {
        return sem->give(res);
    }

    int lock(long timeout, Result &res) {
        int ret = this->sem->take(timeout, res);        
        if (ret < 0) {
            res << ";failed to lock the resource.";
            return ret;
        }
        return 1;
    }
};
} // namespace a8::util::sched
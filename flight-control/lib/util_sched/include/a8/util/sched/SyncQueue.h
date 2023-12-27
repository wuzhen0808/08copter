#pragma once
#include "a8/util.h"

namespace a8::util::sched {
using namespace a8::util;
template <typename T>
class SyncQueue {

public:
    SyncQueue() {
    }
    ~SyncQueue() {
    }

    T take() {
        T rt;
        while (true) {
            int ret = take(rt, (long)(10 * 1000));
            if (ret < 0) {
                continue;
            }
            break;
        }
        return rt;
    }

    void offer(T ele) {
        while (true) {
            int ret = this->offer(ele, 10 * 1000);
            if (ret < 0) {
                continue;
            }
            break;
        }
    }
    void clear() {
        T tmp;
        while (true) {
            if (this->take(tmp, 0) > 0) {
                continue;
            }
            break;
        }
    }
    virtual int peek(T &ele, long timeout) = 0;
    virtual int take(T &ele, long timeout) = 0;
    virtual int offer(T ele, long timeout) = 0;
};

} // namespace a8::util::sched

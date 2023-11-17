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

    T take(long timeout, T def) {
        T rt;
        int ret = take(rt, timeout);
        if (ret < 0) {
            return def;
        }
        return rt;
    }
    T peek(long timeout, T def) {
        T rt;
        int ret = peek(rt, timeout);
        if (ret < 0) {
            return def;
        }
        return rt;
    }
    
    virtual int peek(T &ele, long timeout) = 0;
    virtual int take(T &ele, long timeout) = 0;
    virtual int offer(T ele, long timeout) = 0;
};

} // namespace a8::util::sched

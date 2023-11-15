#pragma once
#include "a8/util.h"

namespace a8::util::schedule {
using namespace a8::util;
class SyncQueue {
public:
    SyncQueue() {}
    ~SyncQueue() {}
    template <typename T>
    T *take(long timeout) {
        void *ele;
        int ret = take(ele, timeout);
        if (ret < 0) {
            return 0;
        }
        return static_cast<T *>(ele);
    }
    virtual int take(void *&ele, long timeout) = 0;
    virtual int offer(void *ele, long timeout) = 0;
};

} // namespace a8::util::schedule

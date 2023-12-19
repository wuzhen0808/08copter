#pragma once
#include "a8/util/Callback.h"

namespace a8::util::sched {
class Timer {
public:
    Timer(){};
    ~Timer(){};
    virtual int close() = 0;
};
} // namespace a8::util::sched

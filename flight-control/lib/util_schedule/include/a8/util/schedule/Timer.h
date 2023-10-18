#pragma once
#include "a8/util/Callback.h"

namespace a8::util::schedule {
class Timer {
public:
    Timer(){};
    ~Timer(){};
    virtual Timer *start() = 0;
};
} // namespace a8::util::schedule

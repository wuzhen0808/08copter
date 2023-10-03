#pragma once
#include "a8/util/Callback.h"

namespace a8::util::thread {
class Timer {
public:
    Timer(){};
    ~Timer(){};
    virtual Timer *start() = 0;
};
} // namespace a8::util::thread

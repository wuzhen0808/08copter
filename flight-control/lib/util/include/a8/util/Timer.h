#ifndef A8_TIMER__
#define A8_TIMER__
#include "a8/util/Callback.h"

namespace a8::util {
class Timer {
public:
    Timer(){};
    ~Timer(){};
    virtual Timer *start() = 0;
};
} // namespace a8::util

#endif
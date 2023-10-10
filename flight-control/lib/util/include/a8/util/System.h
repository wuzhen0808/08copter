#pragma once
#include "a8/util/Output.h"

namespace a8::util {

class System {
protected:
    long long started;

public:
    Output *out;

    System() {
    }

    virtual long long getSysTime() = 0;

    virtual long long getSteadyTime() = 0;

    virtual void sleep(long ms) = 0;

    virtual void formatTime(long long ms, String *str) = 0;
};

} // namespace a8::util

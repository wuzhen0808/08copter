#pragma once
#include "a8/util/Result.h"
#include "a8/util/comp.h"

namespace a8::fc {
using namespace a8::util;
using namespace a8::util::comp;

class Propeller {
protected:
public:
    virtual void setThrottlePwm(long pwm) = 0;
};

} // namespace a8::fc

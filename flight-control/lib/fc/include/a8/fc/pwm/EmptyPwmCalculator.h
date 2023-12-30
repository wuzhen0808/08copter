#pragma once
#include "a8/fc/pwm/PwmCalculator.h"
#include "a8/util.h"
namespace a8::fc {
using namespace a8::util;

class EmptyPwmCalculator : public PwmCalculator {

public:
    EmptyPwmCalculator(PowerManage *pm, LoggerFactory *logFac) : PwmCalculator(pm, logFac) {
    }
    ~EmptyPwmCalculator() {
    }
    
};

} // namespace a8::fc
#pragma once
#include "a8/fc/ElevationEstimator.h"
#include "a8/fc/PowerManage.h"
#include "a8/util.h"
#define A8_PM_DEBUG (0)
namespace a8::fc {
using namespace a8::util;

class PwmCalculator {
public:
    virtual void setup() {
    }
    virtual int collectDataItems(Collector &collector, Result &res) {
        return 1;
    }
    virtual long calculate(int idx, float throttle) = 0;
};

} // namespace a8::fc
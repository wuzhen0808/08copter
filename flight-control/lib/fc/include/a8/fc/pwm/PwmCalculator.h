#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/util.h"
#define A8_PM_DEBUG (0)
namespace a8::fc {
using namespace a8::util;

class PwmCalculator : public FlyWeight {
    PowerManage *pm;
    int pwmFromThrottles[4];
    int pwmCompensate[4];

public:
    PwmCalculator(PowerManage *pm,
                  LoggerFactory *logFac) : FlyWeight(logFac) {
        this->pm = pm;
    }
    virtual void setup() {
    }
    virtual int collectDataItems(Collector *collector, Result &res) {
        int ret = 1;
        for (int i = 0; i < 4; i++) {
            ret = collector->add(String() << "prop" << i << "-pwmT", pwmFromThrottles[i], res);
            if (ret < 0) {
                break;
            }
            ret = collector->add(String() << "prop" << i << "-pwmC", pwmCompensate[i], res);
            if (ret < 0) {
                break;
            }
        }
        return ret;
    }

    long calculate(int idx, float throttle) {
        float voltage = pm->getVoltage();
        pwmCompensate[idx] = throttle * this->calculateCompensateFactor(voltage);
        pwmFromThrottles[idx] = throttle + pwmCompensate[idx];
        return pwmFromThrottles[idx];
    }

    virtual float calculateCompensateFactor(float voltage) {
        return 0;
    }
};

} // namespace a8::fc
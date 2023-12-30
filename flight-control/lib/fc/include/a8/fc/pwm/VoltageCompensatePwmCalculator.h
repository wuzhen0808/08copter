#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/pwm/PwmCalculator.h"
#include "a8/util.h"
#define A8_PM_DEBUG (0)
namespace a8::fc {
using namespace a8::util;

class VoltageCompensatePwmCalculator : public PwmCalculator {
private:
public:
    bool setup_ = false;
    float minVoltCompensate;
    float maxVoltCompensate;

    VoltageCompensatePwmCalculator(PowerManage *pm, float minVoltCompensate, float maxVoltCompensate,
                                   LoggerFactory *logFac) : PwmCalculator(pm, logFac) {
        this->minVoltCompensate = minVoltCompensate;
        this->maxVoltCompensate = maxVoltCompensate;
    }
    ~VoltageCompensatePwmCalculator() {
    }
    virtual void setup() {
        this->setup_ = true;
    }

    int isReady(Result &res) {
        return this->setup_;
    }

    float calculateCompensateFactor(float voltage) override {

        if (voltage > minVoltCompensate & voltage < maxVoltCompensate) {
            return (maxVoltCompensate - voltage) / 35.0f;
        }
        return 0;
    }
};
} // namespace a8::fc
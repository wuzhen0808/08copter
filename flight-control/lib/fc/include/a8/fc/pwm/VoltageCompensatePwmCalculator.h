#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/pwm/PwmCalculator.h"
#include "a8/util.h"
#define A8_PM_DEBUG (0)
namespace a8::fc {
using namespace a8::util;
/**
 * Based on the propeller test data with two different power/volt:
 *  propNum,pwm,g,v
 *  3,1600,230g,11.2
 *  3,2000,230g,7.32
 *
 * 230g@1600@11.2v = 230g@2000@7.32v ;
 * 1600=>1600+400=2000;
 * 400=(1600-1000)*x=600*x;
 * x=y*(11.2-7.32)=3.78y;
 * 400=600*3.78*y ;
 * y=400/600/3.78=0.176;
 *
 */

class VoltageCompensatePwmCalculator : public PwmCalculator {
private:
    static constexpr float coefficient = 0.0176f;

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
    /**
     * return the coefficient of the pwm to be added for voltage short.
     * for example, giving a 600 pwm(component from throttle).
     * If the voltage short 1V, the pwm will be corrected to:
     * 705 = 600+ 0.176*600(the final pwm is 1705).
     * 
     */
    float calculateCompensateFactor(float voltage) override {

        if (voltage > minVoltCompensate & voltage < maxVoltCompensate) {
            return (maxVoltCompensate - voltage) * coefficient;
        }
        return 0;
    }
};
} // namespace a8::fc
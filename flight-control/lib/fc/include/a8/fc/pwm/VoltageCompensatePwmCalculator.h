#pragma once
#include "a8/fc/ElevationEstimator.h"
#include "a8/fc/PowerManage.h"
#include "a8/fc/pwm/PwmCalculator.h"
#include "a8/util.h"
#define A8_PM_DEBUG (0)
namespace a8::fc {
using namespace a8::util;

class VoltageCompensatePwmCalculator : public PwmCalculator, public FlyWeight {
private:
    long maxPwm;
    long minPwm;

public:
    ElevationEstimator *elevationEstimator;
    float maxThrottle = 1000.0f;
    float maxElevation = 1000.0f;
    bool setup_ = false;

    float throttles[4];
    float elevations[4];
    long pwmFromThrottles[4];
    long totalPwmA[4];

    VoltageCompensatePwmCalculator(PowerManage *pm,
                                   LoggerFactory *logFac) : FlyWeight(logFac) {
        this->elevationEstimator = new ElevationEstimator(pm, logFac);
        this->minPwm = 1000;
        this->maxPwm = 2000;
    }
    ~VoltageCompensatePwmCalculator() {
        delete this->elevationEstimator;
    }
    virtual void setup() {
        this->elevationEstimator->setup();
        this->setup_ = true;
    }
    int collectDataItems(Collector *collector, Result &res) override {
        int ret = 1;
        for (int i = 0; i < 4; i++) {
            ret = collector->add(String() << "propeller" << i << "-srcThrottle", throttles[i], res);
            if (ret < 0) {
                break;
            }
            ret = collector->add(String() << "propeller" << i << "-srcElevation", elevations[i], res);
            if (ret < 0) {
                break;
            }
            ret = collector->add(String() << "propeller" << i << "-srcPwm", pwmFromThrottles[i], res);
            if (ret < 0) {
                break;
            }
            ret = collector->add(String() << "propeller" << i << "-srcPwm2", totalPwmA[i], res);
            if (ret < 0) {
                break;
            }
        }
        return ret;
    }
    int isReady(Result &res) {
        return this->setup_;
    }

    long calculate(int idx, float throttle) override {
        // map throttle to desired elevation.
        throttles[idx] = throttle;
        elevations[idx] = Math::map<float>(throttle, 0.0f, maxThrottle, 0.0f, maxElevation);
        // map elevation to pwm, note the max elevation is changing with the voltage.
        maxElevation = this->elevationEstimator->getMaxEstimated();
        pwmFromThrottles[idx] = Math::map<float>(elevations[idx], 0, maxElevation, 0, maxPwm - minPwm);
        totalPwmA[idx] = minPwm + pwmFromThrottles[idx];

        return totalPwmA[idx];
    }
};
} // namespace a8::fc
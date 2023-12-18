#pragma once
#include "a8/fc/ElevationEstimator.h"
#include "a8/fc/PowerManage.h"
#include "a8/fc/Propellers.h"
#include "a8/util.h"
#define A8_PM_DEBUG (0)
namespace a8::fc {
using namespace a8::util;

class PwmManage : public FlyWeight {
public:
    ElevationEstimator *elevationEstimator;
    float maxThrottle = 1000.0f;
    float maxElevation = 1000.0f;
    long minPwm = 1000;
    long maxPwm = 2000;

    bool setup_ = false;

    PwmManage(PowerManage *pm, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->elevationEstimator = new ElevationEstimator(pm, logFac);
    }
    ~PwmManage() {
        delete this->elevationEstimator;
    }
    virtual void setup() {
        this->elevationEstimator->setup();
        this->setup_ = true;
    }
    int isReady(Result &res) {
        return this->setup_;
    }
    long resolvePwmByThrottle(float throttle) {
        // map throttle to desired elevation.
        float elevation = Math::map<float>(throttle, 0.0f, maxThrottle, 0.0f, maxElevation);
        // map elevation to pwm, note the max elevation is changing with the voltage.
        float maxElevation = this->elevationEstimator->getMaxEstimated();
        float pwmFromThrottle = Math::map<float>(elevation, 0, maxElevation, 0, maxPwm - minPwm);
        long totalPwm = minPwm + pwmFromThrottle;
        A8_LOG_INFO(logger, String() << "elevation:" << elevation << ",maxElevation:" << maxElevation << ",pwmFromThrottle:" << pwmFromThrottle);
        A8_LOG_INFO(logger, String() << "throttle:" << throttle << ",minPwm:" << minPwm << ",totalPwm:" << totalPwm);
        return totalPwm;
    }
};
} // namespace a8::fc
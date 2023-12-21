#pragma once
#include "a8/fc/ElevationEstimator.h"
#include "a8/fc/PowerManage.h"
#include "a8/util.h"
#define A8_PM_DEBUG (0)
namespace a8::fc {
using namespace a8::util;

class PwmManage : public FlyWeight {
private:
    long basePwm;
    long maxPwm;

public:
    ElevationEstimator *elevationEstimator;
    float maxThrottle = 1000.0f;
    float maxElevation = 1000.0f;
    bool setup_ = false;

    float throttles[4];
    float elevations[4];
    long pwmFromThrottles[4];
    long totalPwms[4];

    PwmManage(PowerManage *pm, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->elevationEstimator = new ElevationEstimator(pm, logFac);
        this->basePwm = 1000;
        this->maxPwm = 2000;
        A8_LOG_WARN(logger, String() << "PwmManage.basePwm:" << basePwm << ",(double:" << ((double)basePwm) << "),max:" << maxPwm);
    }
    ~PwmManage() {
        delete this->elevationEstimator;
    }
    virtual void setup() {
        this->elevationEstimator->setup();
        this->setup_ = true;
    }
    void collectDataItems(Collector &collector) {
        collector.add<PwmManage *>("PwmM.basePwm", this, [](PwmManage *this_) { return (double)this_->basePwm; });                        
        for (int i = 0; i < 4; i++) {

            collector.add(String() << "propeller" << i << "-srcThrottle", throttles[i]);
            collector.add(String() << "propeller" << i << "-srcElevation", elevations[i]);
            collector.add(String() << "propeller" << i << "-srcPwm", pwmFromThrottles[i]);
            collector.add(String() << "propeller" << i << "-srcPwm2", totalPwms[i]);
        }
    }
    int isReady(Result &res) {
        return this->setup_;
    }

    long updateThrottle(int idx, float throttle) {
        // map throttle to desired elevation.
        throttles[idx] = throttle;
        elevations[idx] = Math::map<float>(throttle, 0.0f, maxThrottle, 0.0f, maxElevation);
        // map elevation to pwm, note the max elevation is changing with the voltage.
        maxElevation = this->elevationEstimator->getMaxEstimated();
        pwmFromThrottles[idx] = Math::map<float>(elevations[idx], 0, maxElevation, 0, maxPwm - basePwm);
        totalPwms[idx] = this->basePwm + pwmFromThrottles[idx];

        A8_LOG_DEBUG(logger, String()
                                << "pwm1:" << pwmFromThrottles[idx]
                                << "+basePwm:" << basePwm << "(double:" << ((double)basePwm) << ")=totalPwms:" << totalPwms[idx]);

        return totalPwms[idx];
    }
};
} // namespace a8::fc
#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Propellers.h"
#include "a8/util.h"
#define A8_PM_DEBUG (0)
namespace a8::fc {
using namespace a8::util;

class ElevationEstimator : public FlyWeight {
    PowerManage *pm;
    //
    float maxVoltage = 12.5f;
    //
    float maxElevationAtMaxVoltage = 1000.0f; // this should be got by testing the propeller at maxVoltage.
    // as a rough approach,
    // following fields used to estimate the impact on the elevation by voltage changing.
    float elevationLostEveryVoltage = 50.0f / (12.5f - 10.0f); // typical(for example under 1500 pwm) low elevation at min volt.
    // current voltage.
    float voltage;
    // current maxElevation estimated, this can be got by testing it at each voltage.
    float maxElevationAtCurrentVoltage;

public:
    ElevationEstimator(PowerManage *pm, LoggerFactory* logFac) :FlyWeight(logFac){
        this->pm = pm;
    }

    void setup() {
        // init value, avoid generate a wrong/dangers max elevation.
        this->update(maxVoltage);
    }

    float getMaxEstimated() {
        return this->maxElevationAtCurrentVoltage;
    }

    void update() {
        update(pm->getVoltage());
    }

    void update(float voltage) {
        this->voltage = voltage;
        // estimated value:
        this->maxElevationAtCurrentVoltage = maxElevationAtMaxVoltage - (maxVoltage - this->voltage) * elevationLostEveryVoltage;
    }
    void tick() {
        update();
    }

}; // end of class power compensate.

} // namespace a8::fc
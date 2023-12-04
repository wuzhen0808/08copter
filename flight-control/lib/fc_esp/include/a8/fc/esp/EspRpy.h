#pragma once
#include "a8/fc.h"
// lang-c-format off
#include <Arduino.h>
#include <MPU9250.h>
// lang-c-format on

namespace a8::fc::esp {
class EspRpy : public Rpy, public FlyWeight {
    MPU9250 mpu;

public:
    EspRpy(LoggerFactory *logFac) : FlyWeight(logFac, "EspRpy") {
    }

    int setup(Result &res) {
        if (!mpu.setup(0x68)) {
            res << "failed to setup mpu9256.";
            return -1;
        }
        mpu.selectFilter(QuatFilterSel::MADGWICK);
        mpu.setFilterIterations(1);
        return 1;
    }

    int getRollPitchYaw(float &roll, float &pitch, float &yaw) override {
        if (!mpu.update()) {
            return -1;
        }
        roll = mpu.getRoll();
        pitch = mpu.getPitch();
        yaw = mpu.getYaw();
        return 1;
    }
};

} // namespace a8::fc::esp

#pragma once
#include "a8/fc.h"
// lang-c-format off
#include <Arduino.h>
#include <MPU9250.h>
// lang-c-format on

namespace a8::fc::esp {
class EspRpy : public Rpy, public FlyWeight {
    MPU9250 *mpu;

public:
    EspRpy(MPU9250 *mpu, LoggerFactory *logFac) : FlyWeight(logFac, "EspRpy") {
        this->mpu = mpu;
    }
    bool update() override {
        return mpu->update();
    }
    float getRoll() override {
        return mpu->getRoll();
    }
    float getPitch() override {
        return mpu->getPitch();
    }
    float getYaw() override {
        return mpu->getYaw();
    }
    
};

} // namespace a8::fc::esp

#pragma once
#include "a8/fc/AttitudeSensor.h"
#include "a8/util.h"
#include <Arduino.h>
#include <MPU9250.h>

namespace a8::fc::esp {
using namespace a8::fc;
using namespace a8::util;

class EspAttitudeSensor : public AttitudeSensor, public FlyWeight {
private:
    MPU9250 *mpu;

public:
    EspAttitudeSensor(LoggerFactory *logFac) : AttitudeSensor(), FlyWeight(logFac) {
        mpu = new MPU9250();
        mpu->setup(0x68);
    }
    ~EspAttitudeSensor() {
        delete mpu;
    }

    int isReady(Result &rst) override {
        return 1;
    }
    int getAngVel(Vector3f &angVel, Result &rst) override {
        mpu->update();
        Vector3f v;
        v.x = mpu->getRoll();
        v.y = mpu->getPitch();
        v.z = mpu->getYaw();
        angVel = v;
        return 1;
    }
    int getAccVel(Vector3f &accVel, Result &rst) override {
        return 1;
    }
    int getAltitude(double &altitude, Result &rst) override {
        return 1;
    }

    int getLongitude(double &longitude, Result &rst) override {
        return 1;
    }
};
} // namespace a8::fc::uno
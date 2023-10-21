#pragma once
#include "a8/fc/AttitudeSensor.h"
#include "a8/hal/arduino/Hal.h"
#include "a8/util.h"

using namespace a8::fc;
using namespace a8::util;

namespace a8::fc::arduino_ {

class ArduinoAttitudeSensor : public AttitudeSensor, public FlyWeight {
private:
    Hal *hal;

public:
    ArduinoAttitudeSensor(Hal *hal, LoggerFactory *logFac) : AttitudeSensor(), FlyWeight(logFac) {
    }
    ~ArduinoAttitudeSensor() {
    }

    int isReady(Result &rst) override {
        return 1;
    }
    int getAngVel(Vector3f &angVel, Result &rst) override {
        hal->mpu9250->update();
        Vector3f v;
        v.x = hal->mpu9250->getRoll();
        v.y = hal->mpu9250->getPitch();
        v.z = hal->mpu9250->getYaw();
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
} // namespace a8::fc::arduino_
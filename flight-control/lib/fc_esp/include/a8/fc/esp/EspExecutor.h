#pragma once
#include "a8/fc.h"
#include "a8/fc/esp/EspPilot.h"
#include "a8/fc/esp/EspPropellers.h"
#include "a8/fc/esp/EspRpy.h"

namespace a8::fc::esp {

class EspExecutor : public Executor {
    MPU9250 *mpu;
    Rpy *rpy;
    Propellers *propellers;
    PowerManage *pm;

public:
    EspExecutor(PowerManage *pm, MPU9250 *mpu, System *sys, LoggerFactory *logFac) : Executor(sys, logFac) {
        this->pm = pm;
        this->mpu = mpu;
        this->rpy = new EspRpy(mpu, loggerFactory);
        this->propellers = new EspPropellers(17, 18, 19, 20, loggerFactory);
    }
    ~EspExecutor() {
        delete this->pm;
        delete this->propellers;
        delete this->rpy;
    }
    PowerManage *getPowerManage() {
        return pm;
    }
    Rpy *getRpy() override {
        return rpy;
    }

    Propellers *getPropellers() override {
        return propellers;
    }

    Pilot *createPilot(Config &config) override {
        return new EspPilot(config, rpy, propellers, loggerFactory);
    }

    void releasePilot(Pilot *pilot) override {
        delete pilot;
    }
};
} // namespace a8::fc::esp
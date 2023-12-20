#pragma once
#include "a8/fc.h"
#include "a8/fc/esp/EspMission.h"
#include "a8/fc/esp/EspPropellers.h"
#include "a8/fc/esp/EspRpy.h"

namespace a8::fc::esp {

class EspCommander : public Commander {
    MPU9250 *mpu;
    Rpy *rpy;
    Propellers *propellers;
    PowerManage *pm;

public:
    EspCommander(PowerManage *pm, Rpy *rpy, System *sys, Scheduler *sch, LoggerFactory *logFac) : Commander(sys, sch, logFac) {
        this->pm = pm;
        this->mpu = mpu;
        this->rpy = rpy;
        this->propellers = new EspPropellers(pm, 17, 18, 19, 20, loggerFactory);
    }
    ~EspCommander() {
        delete this->propellers;
    }
    void setup() override {
        this->propellers->setup();
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

    Mission *createMission(Config &config) override {
        return new EspMission(sys, config, rpy, propellers, loggerFactory);
    }

    void releaseMission(Mission *pilot) override {
        delete pilot;
    }
};
} // namespace a8::fc::esp
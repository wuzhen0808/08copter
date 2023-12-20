#pragma once
#include "a8/fc.h"
#include "a8/fc/esp/EspMission.h"
#include "a8/fc/esp/EspPropellers.h"
#include "a8/fc/esp/EspRpy.h"

namespace a8::fc::esp {

class EspCommander : public Commander {
    Propellers *propellers;

public:
    EspCommander(PowerManage *pm, Rpy *rpy, System *sys, Scheduler *sch, LoggerFactory *logFac) : Commander(pm, rpy, sys, sch, logFac) {
        this->propellers = new EspPropellers(pm, 17, 18, 19, 20, loggerFactory);
    }
    ~EspCommander() {
        delete this->propellers;
    }
    void setup() override {
        this->propellers->setup();
        Commander::setup();
    }
    
    Propellers *getPropellers() override {
        return propellers;
    }

    Mission *createMission(Config &config) override {
        return new EspMission(sys, config, rpy, propellers, loggerFactory);
    }
};
} // namespace a8::fc::esp
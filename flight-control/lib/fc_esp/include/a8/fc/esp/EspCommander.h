#pragma once
#include "a8/fc.h"
#include "a8/fc/esp/EspPropellers.h"
#include "a8/fc/esp/EspRpy.h"

namespace a8::fc::esp {

class EspCommander : public Commander {
    Propellers *propellers;

public:
    EspCommander(PowerManage *pm, Rpy *rpy, Scheduler *sch, System *sys, LoggerFactory *logFac) : Commander(pm, rpy, sys, sch, logFac) {
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
};
} // namespace a8::fc::esp
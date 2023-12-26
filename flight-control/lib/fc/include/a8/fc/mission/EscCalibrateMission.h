#pragma once
#include "a8/fc/mission/Mission.h"
#include "a8/util.h"

namespace a8::fc::mission {
using namespace a8::util;
using namespace a8::fc::throttle;

class EscCalibrateMission : public Mission {

protected:
    Propellers *propellers;

public:
    EscCalibrateMission(Propellers *propellers, System *sys, LoggerFactory *logFac) : Mission(sys, logFac, "EscCalibrateMission") {
        this->propellers = propellers;
    }

    ~EscCalibrateMission() {
    }

    int setup(Result &res) override {
        return 1;
    }
    int collectDataItems(Collector &collector, Result &res) override {
        return 1;
    }
    int run(Context &mc, Result &res) override {
        return 1;
    }
};

} // namespace a8::fc::mission
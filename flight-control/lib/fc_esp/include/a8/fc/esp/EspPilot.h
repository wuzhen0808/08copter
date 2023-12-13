#pragma once
#include "a8/fc/Pilot.h"
#include "a8/fc/esp/EspRpy.h"
#include <MPU9250.h>
namespace a8::fc::esp {
using namespace a8::fc;
class EspPilot : public Pilot {

public:
    EspPilot(Config &config, Rpy *rpy, Propellers *propellers, LoggerFactory *logFac) : Pilot(config, propellers, rpy, logFac) {
    }
    ~EspPilot() {
    }
};
} // namespace a8::fc::esp
#pragma once
#include "a8/fc/Mission.h"
#include "a8/fc/esp/EspRpy.h"
#include <MPU9250.h>
namespace a8::fc::esp {
using namespace a8::fc;
class EspMission : public Mission {

public:
    EspMission(Config &config, Rpy *rpy, Propellers *propellers, LoggerFactory *logFac) : Mission(config, propellers, rpy, logFac) {
    }
    ~EspMission() {
    }
};
} // namespace a8::fc::esp
#pragma once
#include "a8/fc/Balancer.h"
#include "a8/fc/esp/EspRpy.h"
#include <MPU9250.h>
namespace a8::fc::esp {
using namespace a8::fc;
class EspBalancer : public Balancer {
    static Rpy *buildRpy(EspBalancer *this_, MPU9250 *mpu, LoggerFactory *logFac) {
        this_->rpy = new EspRpy(mpu, logFac);

        return this_->rpy;
    }

    Rpy *rpy;

public:
    EspBalancer(Propeller *lh, Propeller *rh, Propeller *la, Propeller *ra, MPU9250 *mpu, System *sys, LoggerFactory *logFac) : Balancer(lh, rh, la, ra, buildRpy(this, mpu, logFac), sys, logFac) {
    }
};
} // namespace a8::fc::esp
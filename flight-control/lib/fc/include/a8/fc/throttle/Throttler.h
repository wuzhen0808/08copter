#pragma once
#include "a8/fc/throttle/Throttle.h"
#include "a8/util.h"
#define A8_THROTTLE_DEBUG 0
namespace a8::fc::throttle {
using namespace a8::util;

class Throttler : public FlyWeight {
protected:
    float maxThrottle = 1000.0f;

public:
    Throttler(LoggerFactory *logFac, String name) : FlyWeight(logFac, name) {
    }
    virtual void setup() = 0;

    virtual int collectDataItems(Collector *collector, Result& res) {
        return 1;
    }

    virtual int update(Throttle &ctx, Result &res) = 0;

};

} // namespace a8::fc::throttle
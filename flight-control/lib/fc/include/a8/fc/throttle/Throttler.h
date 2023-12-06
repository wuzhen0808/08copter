#pragma once
#include "a8/util.h"
#include "a8/fc/throttle/Context.h"

namespace a8::fc::throttle {
using namespace a8::util;

class Throttler : public FlyWeight {
public:
    Throttler(LoggerFactory *logFac, String name) : FlyWeight(logFac, name) {
    }

    virtual int update(Context &ctx, Result &res) = 0;
};

} // namespace a8::fc
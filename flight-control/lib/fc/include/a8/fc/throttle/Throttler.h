#pragma once
#include "a8/fc/Context.h"
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class Throttler : public FlyWeight {
public:
    Throttler(LoggerFactory *logFac, String name) : FlyWeight(logFac, name) {
    }

    virtual int update(Context &ctx, Result &res) = 0;

    virtual void printHistory(int depth, String &msg) = 0;

    virtual void getLimitInTheory(long &minBase, long &maxBase) = 0;
};

} // namespace a8::fc::throttle
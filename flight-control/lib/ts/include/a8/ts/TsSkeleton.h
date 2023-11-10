#pragma once

#include "a8/link.h"
#include "a8/util/comp.h"

namespace a8::ts {
using namespace a8::link;
using namespace a8::util::comp;

class TsSkeleton : public TsApi, public FlyWeight {

public:
    TsSkeleton(LoggerFactory *loggerFactory) : FlyWeight(loggerFactory) {
    }

    int ping(String msg, Result &rst) override {
        log(String() << "received ping msg:" << msg);
        return 1;
    }    
};
} // namespace a8::fc
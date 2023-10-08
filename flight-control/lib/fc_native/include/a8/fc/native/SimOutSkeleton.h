#pragma once
#include "a8/link/LineApi.h"
#include "a8/util.h"
#include "a8/util/comp.h"
#include <string>
using namespace a8::link;
using namespace a8::util::comp;

namespace a8::fc::native {

class SimOutSkeleton : public LineApi, public FlyWeight {
    int lines;

public:
    static void release(void *skeleton) {
        delete static_cast<SimOutSkeleton *>(skeleton);
    }
    SimOutSkeleton(LoggerFactory *logFac) : FlyWeight(logFac) {
        lines = 0;
    }
    void line(String line) override {
        lines++;
        log(line);
    }
};
} // namespace a8::fc::native
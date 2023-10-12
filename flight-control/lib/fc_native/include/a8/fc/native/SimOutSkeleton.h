#pragma once
#include "a8/link/LineApi.h"
#include "a8/util.h"
#include "a8/util/comp.h"
#include <string>
using namespace a8::link;
using namespace a8::util::comp;

namespace a8::fc::native {
/**
 * Accept the simulator's response when sending instructions to simulator.
 * Just logging the response and do nothing else.
 * TODO :
 * Provide such skelton with a more general way.
 *
 */
class SimOutSkeleton : public LineApi, public FlyWeight {
    int lines;

public:
    static void release(void *skeleton) {
        delete static_cast<SimOutSkeleton *>(skeleton);
    }
    SimOutSkeleton(LoggerFactory *logFac) : FlyWeight(logFac) {
        lines = 0;
    }
    int line(String line, Result &rst) override {
        lines++;
        log(line);
        return 1;
    }
};
} // namespace a8::fc::native
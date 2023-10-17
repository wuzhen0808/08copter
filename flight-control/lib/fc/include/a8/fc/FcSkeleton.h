#pragma once

#include "a8/link.h"
#include "a8/util/comp.h"

using namespace a8::link;
using namespace a8::util::comp;

namespace a8::fc {
class FcSkeleton : public FcApi, public FlyWeight {

public:
    FcSkeleton(LoggerFactory *loggerFactory) : FlyWeight(loggerFactory) {
    }

    int ping(String msg, Result &rst) override {
        log(String() << "received ping msg:" << msg);
        return 1;
    }
    int command(String cmd, Result &rst) override {
        // todo.
        log(String() << "received command:" << cmd);
        return 1;
    }
    int test(Result &rst) override {
        log(String() << "received test instruction");
        return 1;
    }
    int throttle(float throttle, Result &rst) override {
        return 1;
    }
    int roll(float roll, Result &rst) override {
        return 1;
    }
    int pitch(float pitch, Result &rst) override {
        return 1;
    }
    int yaw(float yaw, Result &rst) override {
        return 1;
    }
};
} // namespace a8::fc
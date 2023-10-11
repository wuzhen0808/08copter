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

    void ping(String msg) override {
        log(String() << "received ping msg:" << msg);
    }
    void command(String cmd) override {
        // todo.
        log(String() << "received command:" << cmd);
    }
    void test() override {
        log(String() << "received test instruction");
    }
};
} // namespace a8::fc
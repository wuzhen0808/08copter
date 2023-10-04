#pragma once

#include "a8/link.h"
#include "a8/util/comp.h"

using namespace a8::link;
using namespace a8::util::comp;

namespace a8::fc::native {
class FcSkeletonImpl : public FcSkeleton, public Component {
public:
    FcSkeletonImpl() : Component("skel") {
        
    }
    void ping(String msg) override {
        log(String() << "received ping msg:" << msg);
    }
    void command(String cmd) override {
        // todo.
        log(String() << "received command:" << cmd);
    }

    void setup(StagingContext *context) override {
    }
};
} // namespace a8::fc::native
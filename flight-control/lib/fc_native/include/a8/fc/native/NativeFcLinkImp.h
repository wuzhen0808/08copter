#pragma once
#include "a8/link.h"
using namespace a8::link;
namespace a8::fc::native {
class NativeFcLink : public FcSkeleton {
    void ping(String msg) override {
        // DO nothing.
    }
};
} // namespace a8::fc::native
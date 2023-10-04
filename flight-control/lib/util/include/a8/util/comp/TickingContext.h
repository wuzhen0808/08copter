#pragma once
#include "a8/util.h"
#include "a8/util/thread.h"
using namespace a8::util;
using namespace a8::util::thread;
namespace a8::util::comp {

class TickingContext {
public:
    Rate rate;
    long startTimestamp;
};

} // namespace a8::util::comp
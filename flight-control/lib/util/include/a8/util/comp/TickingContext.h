#pragma once
#include "a8/util.h"
#include "a8/util/comp/StagingContext.h"
#include "a8/util/thread.h"

using namespace a8::util;
using namespace a8::util::thread;
namespace a8::util::comp {

class TickingContext {
    StagingContext *staging;
    Rate rate;
    long startTimestamp;

public:
    TickingContext(StagingContext *staging, Rate rate) {
        this->staging = staging;
        this->rate = rate;
    }
    Rate getRate() {
        return this->rate;
    }
    StagingContext *getStaging() {
        return this->staging;
    }

    void beforTick() {
        this->startTimestamp = staging->getSys()->millis();
    }
};

} // namespace a8::util::comp
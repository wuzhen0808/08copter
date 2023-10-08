#pragma once
#include "a8/link.h"
using namespace a8::link;

namespace a8::fc::native {
class SimOutStub {
    Channel *channel;

public:
    static void *create(Channel *channel) {
        return new SimOutStub(channel);
    }

    static void release(void *stub) {
        delete static_cast<SimOutStub *>(stub);
    }

    SimOutStub(Channel *channel) {
        this->channel = channel;
    }

    void setThrottleNorm(int servoId, float throttle) {
        String sb = String::format("set fcs/throttle-cmd-norm[%i] %f\n", servoId, throttle);
        Result rst;
        channel->send(0, &sb, rst);
    }
};
} // namespace a8::fc::native
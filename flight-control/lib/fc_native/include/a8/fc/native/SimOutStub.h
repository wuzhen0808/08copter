#pragma once
#include "a8/link.h"
using namespace a8::link;

namespace a8::fc::native {
class SimOutStub : public LineStub {
public:
    SimOutStub(Channel *channel) : LineStub(channel) {
        
    }

    void setThrottleNorm(int servoId, float throttle){
        String sb = String::format("set fcs/throttle-cmd-norm[%i] %f\n", servoId, throttle);
        this->line(sb);
    }
};
} // namespace a8::fc::native
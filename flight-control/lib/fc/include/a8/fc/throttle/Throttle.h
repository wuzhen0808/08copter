#pragma once
#include "a8/fc/Propellers.h"
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class Throttle {
public:
    long startTimeMs = -1;
    long timeMs_;
    long lastTimeMs_;
    Propellers *propellers;
    bool active = true;

    Throttle(Propellers *propellers) {
        this->propellers = propellers;
    }

    ~Throttle() {
        A8_DEBUG("~Throttle()");
    }
    
    void reset(long timeMs) {
        this->startTimeMs = timeMs;
        this->timeMs_ = this->startTimeMs;
    }

    void preUpdate(long timeMs) {
        this->lastTimeMs_ = this->timeMs_;
        this->timeMs_ = timeMs;
        this->propellers->beforeUpdate();
    }
    Throttle &operator+=(long pwm) {
        this->propellers->addThrottle(pwm);
        return *this;
    }

    void commitUpdate() {
        propellers->commitUpdate();
    }

};

} // namespace a8::fc::throttle
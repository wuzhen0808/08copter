#pragma once
#include "a8/fc/Propellers.h"
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class Throttle {
public:
  
    TimeUs startTimeUs = -1;
    TimeUs timeUs_;
    TimeUs lastTimeUs_;
    Propellers *propellers;
    bool active = true;

    Throttle(Propellers *propellers) {
        this->propellers = propellers;
    }

    ~Throttle() {
        A8_DEBUG("~Throttle()");
    }
    
    void reset(TimeUs timeUs) {
        this->startTimeUs = timeUs;
        this->timeUs_ = this->startTimeUs;
    }

    void preUpdate(TimeUs timeUs) {
        this->lastTimeUs_ = this->timeUs_;
        this->timeUs_ = timeUs;
        this->propellers->beforeUpdate();
    }
    Throttle &operator+=(long pwm) {
        this->propellers->addThrottle(pwm);
        return *this;
    }

    void commitUpdate() {
        propellers->commitThrottle();
    }

};

} // namespace a8::fc::throttle
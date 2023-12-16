#pragma once
#include "a8/util/Result.h"
#include "a8/util/comp.h"

namespace a8::fc {
using namespace a8::util;
using namespace a8::util::comp;

class Propeller {
protected:
    long historyMaxPwm;
    long historyMinPwm;
    long pwm;
    String name;
    bool enable_ = true;
    long minInTheory = 0;
    long maxInTheory = 2000;

public:
    Propeller(String name) {
        this->name = name;
    }
    void setLimitInTheory(long min, long max) {
        this->minInTheory = min;
        this->maxInTheory = max;
    }

    bool isEnabled() {
        return this->enable_;
    }

    void enable(bool enable) {
        this->enable_ = enable;
    }
    String getName() {
        return this->name;
    }
    
    void printHistory(String intend, String &msg) {
        msg << intend << "historyMinPwm(inTheory:" << this->minInTheory << "):" << this->historyMinPwm
            << intend << "historyMaxPwm(inTheory:" << this->maxInTheory << "):" << this->historyMaxPwm;
    }

    void addPwm(long pwm) {
        this->setPwm(this->pwm + pwm);
    }

    long getPwm() {
        return this->pwm;
    }

    void setPwm(long pwm) {
        this->pwm = pwm;
    }

    void startUpdate() {
        this->pwm = 0;
    }
    void start() {
        this->historyMaxPwm = 0;
        this->historyMinPwm = 2000;
    }
    void updateHistory() {
        // update history
        if (pwm > historyMaxPwm) {
            historyMaxPwm = pwm;
        } else if (pwm < historyMinPwm) {
            historyMinPwm = pwm;
        }
    }
    void commitUpdate() {
        if (this->enable_) {
            this->doApplyPwm();
        } else {
            // do not apply pwm.
        }

        this->updateHistory();
    }

    virtual void doApplyPwm() = 0;
};

} // namespace a8::fc

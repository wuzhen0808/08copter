#pragma once
#include "a8/fc/pwm/PwmCalculator.h"
#include "a8/fc/collect/Collector.h"
#include "a8/util/Result.h"
#include "a8/util/comp.h"

namespace a8::fc {
using namespace a8::util;
using namespace a8::util::comp;
using namespace a8::fc::collect;

class Propeller {
protected:
    float throttle;
    String name;
    int idx;
    bool enable_ = true;
    float minInTheory = 0;
    float maxInTheory = 1000;
    long pwm;

public:
    Propeller(String name, int idx) {
        this->name = name;
        this->idx = idx;
    }

    virtual void setup() = 0;

    int collectDataItems(Collector &collector, Result &res) {

        int ret = collector.add(String("prop") << this->idx << "-throttle", this->throttle, res);
        if (ret > 0) {
            collector.add(String("prop") << this->idx << "-pwm", this->pwm, res);
        }
        return ret;
    }

    void open() {
    }
    void close() {
    }

    void setLimitInTheory(float min, float max) {
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

    void addThrottle(float pwm) {
        this->setThrottle(this->throttle + pwm);
    }

    float getThrottle() {
        return this->throttle;
    }

    void setThrottle(float throttle) {
        this->throttle = throttle;
    }

    void beforeUpdate() {
        this->throttle = 0;
    }

    long getPwm() {
        return pwm;
    }

    template <typename T>
    void updateMinMax(T value, T &min, T &max) {
        if (value < min) {
            min = value;
        }
        if (value > max) {
            max = value;
        }
    }
    
    void commitUpdate(PwmCalculator * pwmCalculator) {
        pwm = pwmCalculator->calculate(this->idx, this->throttle);
        if (this->enable_) {
            this->doApply(pwm);
        }

    }

    virtual void doApply(long pwm) = 0;
};

} // namespace a8::fc

#pragma once
#include "a8/fc/PwmManage.h"
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
    PwmManage *pwmManage;
    long pwm;

public:
    Propeller(String name, int idx, PwmManage *pwmManage) {
        this->name = name;
        this->idx = idx;
        this->pwmManage = pwmManage;
    }

    virtual void setup() = 0;

    void collectDataItems(Collector &collector) {
        
        collector.add(String("propeller") << this->idx << "-Throttle", this->throttle);
        collector.add(String("propeller") << this->idx << "-Pwm", this->pwm);
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
    void commitUpdate() {
        pwm = this->pwmManage->updateThrottle(this->idx, this->throttle);
        if (this->enable_) {
            this->doApply(pwm);
        }
    }

    virtual void doApply(long pwm) = 0;
};

} // namespace a8::fc

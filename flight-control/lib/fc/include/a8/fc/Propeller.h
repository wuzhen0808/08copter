#pragma once
#include "a8/fc/collect/Collector.h"
#include "a8/fc/pwm/PwmCalculator.h"
#include "a8/util/Result.h"
#include "a8/util/comp.h"
#include "a8/fc/GlobalVars.h"
namespace a8::fc {
using namespace a8::util;
using namespace a8::util::comp;
using namespace a8::fc::collect;

class Propeller {
protected:
    float throttle;
    String name;
    int idx;
    bool locked_ = true;
    bool open_ = false;
    long pwm;

public:
    Propeller(String name, int idx) {
        this->name = name;
        this->idx = idx;
        this->pwm = GlobalVars::MIN_PWM;
    }
    int getIndex() {
        return this->idx;
    }
    virtual void hz(int hz) = 0;

    virtual int attach(int pin) = 0;
    virtual void setup() = 0;

    int collectDataItems(Collector *collector, Result &res) {

        int ret = collector->add(String("prop") << this->idx << "-throttle", this->throttle, res);
        if (ret > 0) {
            collector->add(String("prop") << this->idx << "-pwm", this->pwm, res);
        }
        return ret;
    }

    void open() {
        this->open_ = true;
    }
    void close() {
        this->open_ = false;
    }

    bool isLocked() {
        return this->locked_;
    }
    bool isOpen() {
        return this->open_;
    }

    void lock() {
        this->locked_ = true;
    }

    void unLock() {
        this->locked_ = false;
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
    void setPwm(long pwm) {
        this->pwm = pwm;
    }
    void commit() {
        if (this->locked_) {
            return;
        }
        if (!this->open_) {
            return;
        }
        this->doApply(pwm);
    }
    void commit(long pwm) {
        this->setPwm(pwm);
        this->commit();
    }
    void commitThrottle(PwmCalculator *pwmCalculator) {
        commit(1000 + pwmCalculator->calculate(this->idx, this->throttle));
    }

    virtual void doApply(long pwm) = 0;
};

} // namespace a8::fc

#pragma once
#include "a8/fc/PwmManage.h"
#include "a8/util/Result.h"
#include "a8/util/comp.h"
namespace a8::fc {
using namespace a8::util;
using namespace a8::util::comp;

class Propeller {
protected:
    float historyMaxThrottle;
    float historyMinThrottle;
    long historyMaxPwm;
    long historyMinPwm;
    float throttle;
    String name;
    bool enable_ = true;
    float minInTheory = 0;
    float maxInTheory = 1000;
    PwmManage *pwmManage;
    long pwm;

public:
    Propeller(String name, PwmManage *pwmManage) {
        this->name = name;
        this->pwmManage = pwmManage;
    }

    virtual void setup() = 0;
    void open() {
        this->historyMaxThrottle = 0.0f;
        this->historyMinThrottle = 1000.0f;
        this->historyMaxPwm = 0;
        this->historyMinPwm = 1000;
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

    void printHistory(int depth, History &his) {
        
        String msg1 = String() << "historyMinThrottle(inTheory:" << this->minInTheory << "):" << this->historyMinThrottle;        
        his.add(depth, msg1);
        his.add(depth, String() << "historyMaxThrottle(inTheory:" << this->maxInTheory << "):" << this->historyMaxThrottle);
        his.add(depth, String() << "historyMinPwm:" << this->historyMinPwm);
        his.add(depth, String() << "historyMaxPwm:" << this->historyMaxPwm);
        
    }

    void addThrottle(float pwm) {
        this->setThrottle(this->throttle + pwm);
    }

    float getThrottle() {
        return this->throttle;
    }

    void setThrottle(long pwm) {
        this->throttle = pwm;
    }

    void beforeUpdate() {
        this->throttle = 0;
    }

    void updateHistory() {
        // update history
        updateMinMax<float>(throttle, historyMinThrottle, historyMaxThrottle);
        updateMinMax<long>(pwm, historyMinPwm, historyMaxPwm);
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
        pwm = this->pwmManage->resolvePwmByThrottle(this->throttle);
        if (this->enable_) {
            this->doApply(pwm);
        }

        this->updateHistory();
    }

    virtual void doApply(long pwm) = 0;
};

} // namespace a8::fc

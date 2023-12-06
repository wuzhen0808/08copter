#pragma once
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class Context {
public:
    long startTimeMs = -1;
    long timeMs_;
    long lastTimeMs_;
    long pwmLH_;
    long pwmRH_;
    long pwmLA_;
    long pwmRA_;
    String message;

    Context(long startTimeMs) {
        this->startTimeMs = startTimeMs;
        this->timeMs_ = this->startTimeMs;
    }
    long averagePwm() {
        return (pwmLH_ + pwmRH_ + pwmLA_ + pwmRA_) / 4;
    }

    Context &operator<<(String &str) {
        message << str;
        return *this;
    }

    void beforeUpdate(long timeMs) {
        this->lastTimeMs_ = this->timeMs_;
        this->timeMs_ = timeMs;
        pwmLH_ = 0;
        pwmRH_ = 0;
        pwmLA_ = 0;
        pwmRA_ = 0;
        this->message.clear();
    }

    void setPwm(long pwm0, long pwm1, long pwm2, long pwm3) {
        this->pwmLH_ = pwm0;
        this->pwmRH_ = pwm1;
        this->pwmLA_ = pwm2;
        this->pwmRA_ = pwm3;
        this->recordPwm();
    }
    void addPwm(long pwm) {
        addPwm(pwm, pwm, pwm, pwm);
    }
    void addPwm(long pwm0, long pwm1, long pwm2, long pwm3) {
        this->pwmLH_ += pwm0;
        this->pwmRH_ += pwm1;
        this->pwmLA_ += pwm2;
        this->pwmRA_ += pwm3;
        this->recordPwm();
    }

    Context &operator+=(long pwm) {
        addPwm(pwm, pwm, pwm, pwm);
        return *this;
    }

    void recordPwm() {
        message << ";pwm:(" << pwmLH_ << "," << pwmRH_ << "," << pwmLA_ << "," << pwmRA_ << ")";
    }

    friend String &operator<<(String &str, const Context *ctx) {
        return str << *ctx;
    }

    friend String operator<<(String str, const Context *ctx) {
        return str << *ctx;
    }

    friend String &operator<<(String &str, const Context &ctx) {
        str << "ctx;msg:" << ctx.message;
        return str;
    }
};

} // namespace a8::fc
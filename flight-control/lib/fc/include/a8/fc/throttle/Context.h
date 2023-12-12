#pragma once
#include "a8/util.h"
#include "a8/fc/Propellers.h"

namespace a8::fc::throttle {
using namespace a8::util;

class Context {
public:
    long startTimeMs = -1;
    long timeMs_;
    long lastTimeMs_;
    Propellers* propellers;
    String message;

    Context(long startTimeMs, Propellers * propellers) {
        this->startTimeMs = startTimeMs;
        this->timeMs_ = this->startTimeMs;
        this->propellers = propellers;
    }

    ~Context() {
    }

    Context &operator<<(String &str) {
        message << str;
        return *this;
    }
    
    void startUpdate(long timeMs) {
        this->lastTimeMs_ = this->timeMs_;
        this->timeMs_ = timeMs;    
        this->message.clear();
        this->propellers->startUpdate();        
    }
    Context &operator+=(long pwm) {
        this->propellers->addPwm(pwm);
        return *this;
    }

    void commitUpdate() {
        propellers->commitUpdate();
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

} // namespace a8::fc::throttle
#pragma once
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

class Throttler : public FlyWeight {
public:
    class Context {
    public:
        long startTimeMs = -1;
        long timeMs;
        long lastTimeMs;
        long pwmLH;
        long pwmRH;
        long pwmLA;
        long pwmRA;
        String message;

        Context(long startTimeMs) {
            this->startTimeMs = startTimeMs;
            this->timeMs = this->startTimeMs;
        }
        long averagePwm() {
            return (pwmLH + pwmRH + pwmLA + pwmRA) / 4;
        }

        Context &operator<<(String &str) {
            message << str;
            return *this;
        }

        void beforeUpdate(long timeMs) {
            this->lastTimeMs = timeMs;
            this->timeMs = timeMs;
            pwmLH = 0;
            pwmRH = 0;
            pwmLA = 0;
            pwmRA = 0;
            this->message.clear();
        }

        void afterUpdate() {
        }

        void setPwm(long pwm0, long pwm1, long pwm2, long pwm3) {
            this->pwmLH = pwm0;
            this->pwmRH = pwm1;
            this->pwmLA = pwm2;
            this->pwmRA = pwm3;
            this->recordPwm();
        }
        void addPwm(long pwm) {
            addPwm(pwm, pwm, pwm, pwm);
        }
        void addPwm(long pwm0, long pwm1, long pwm2, long pwm3) {
            this->pwmLH += pwm0;
            this->pwmRH += pwm1;
            this->pwmLA += pwm2;
            this->pwmRA += pwm3;
            this->recordPwm();
        }

        Context &operator+=(long pwm) {
            addPwm(pwm, pwm, pwm, pwm);
            return *this;
        }

        void recordPwm() {
            message << ";pwm:(" << pwmLH << "," << pwmRH << "," << pwmLA << "," << pwmRA << ")";
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

public:
    Throttler(LoggerFactory *logFac, String name) : FlyWeight(logFac, name) {
    }

    virtual int update(Context &ctx, Result &res) = 0;
};
} // namespace a8::fc
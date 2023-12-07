#pragma once
#include "a8/util.h"
namespace a8::fc::throttle {
using namespace a8::util;

class Pid {

    // pid
    float pwm;

    /////////////////PID CONSTANTS/////////////////
    double kp = 0; // 3.55
    double ki = 0; // 0.003
    double kd = 0; // 2.05
    float i = 0;
    float lastError = 0;
    long lastTimeMs = -1;

public:
    Pid(double kp, double ki, double kd) {
        this->config(kp, ki, kd);
    }
    
    void config(double kp, double ki, double kd) {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
    }
    void update(long timeMs, float actual, float desired, String &msg) {

        if (lastTimeMs < 0) {
            lastTimeMs = timeMs;
        }

        float error = desired - actual;
        float p = kp * error;
        long elapsedTimeSec = (timeMs - lastTimeMs) / 1000.0f;
        // if (-3 < error < 3) {
        i = i + (ki * error * elapsedTimeSec);
        // }
        float d = 0;
        if (elapsedTimeSec > 0) {
            d = kd * ((error - lastError) / elapsedTimeSec);
        }
        //
        pwm = p + i + d;
        // msg << "pid:(" << pwm << "=" << p << "+" << i << "+" << d << ")";
        if (pwm < -1000) {
            pwm = -1000;
        }
        if (pwm > 1000) {
            pwm = 1000;
        }
        lastError = error;
        lastTimeMs = timeMs;
    }
    float getLastError() {
        return this->lastError;
    }

    long getPwm() {
        return this->pwm;
    }
};
} // namespace a8::fc::throttle
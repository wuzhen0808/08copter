#pragma once
#include "a8/hal.h"
#include "a8/util/String.h"
// Reference:
// https://github.com/bitcraze/crazyflie-firmware.git
// https://www.youtube.com/watch?v=NVLXCwc8HzM

namespace a8::fc {
using namespace a8::hal;
using namespace a8::util;
class PidControl {

private:
    unsigned long lastUpdateTimeInMs = 0;
    float lastError;
    System *sys;

public:
    float kp;
    float ki;
    float kd;

    PidControl(System *sys, float kp, float ki, float kd) {
        (*this).kp = kp;
        (*this).ki = ki;
        (*this).kd = kd;
        this->sys = sys;
    }
    ~PidControl() {
    }

    float update(float desirePosition, float actualPosition) {

        float error = desirePosition - actualPosition;
        long now = sys->getSteadyTime();
        if (lastUpdateTimeInMs == 0) {
            lastUpdateTimeInMs = now;
        }
        float p = kp * error; // P
        //
        float deltaTime = now - lastUpdateTimeInMs;
        float i = ki * error * deltaTime; // I
        float d = 0.0f;
        if (deltaTime >= 1.0f) {
            d = kd * (error - lastError) / deltaTime; // D
        }

        lastUpdateTimeInMs = now;
        return p + i + d;
    }
    void setup(float kp, float ki, float kd) {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
    }

    friend String &operator<<(String &str, const PidControl *pid) {
        return operator<<(str, *pid);
    }

    friend String &operator<<(String &str, const PidControl &pid) {

        return str << "(kp:" << pid.kp << ",ki:" << pid.ki << ",kd:" << pid.kd << ")";
    }
};
} // namespace a8::fc

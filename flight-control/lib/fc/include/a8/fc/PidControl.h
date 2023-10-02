#pragma once
#include "a8/hal.h"
#include "a8/util/String.h"
// Reference:
// https://github.com/bitcraze/crazyflie-firmware.git
// https://www.youtube.com/watch?v=NVLXCwc8HzM
using namespace a8::hal;
using namespace a8::util;

namespace a8::fc {
class PidControl {

private:
    unsigned long lastUpdateTimeInMs = 0;
    float lastError;

public:
    float kp;
    float ki;
    float kd;

    PidControl(float kp, float ki, float kd) {
        (*this).kp = kp;
        (*this).ki = ki;
        (*this).kd = kd;
    }
    ~PidControl() {

    }

    float update(float desirePosition, float actualPosition) {

        float error = desirePosition - actualPosition;
        unsigned long now = S->millis();
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

    String toString() {
        return String::format("(pk:%e,pi:%e,pd:%e)", kp, ki, kd);
    }
};
} // namespace a8::fc

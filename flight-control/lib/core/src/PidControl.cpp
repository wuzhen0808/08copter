#include "a8/core/PidControl.h"
#include "a8/hal/Hal.h"
using namespace a8::util;
namespace a8::core {
using namespace a8::hal;

// https://www.youtube.com/watch?v=NVLXCwc8HzM

PidControl::PidControl(float kp, float ki, float kd) {
    // init as the default arguments.
    (*this).kp = kp;
    (*this).ki = ki;
    (*this).kd = kd;
}

PidControl::~PidControl() {
}

float PidControl::update(float desirePosition, float actualPosition) {

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
} // namespace a8::core
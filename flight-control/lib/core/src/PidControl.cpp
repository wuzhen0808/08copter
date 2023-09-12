#include "a8/hal/Hal.hpp"
#include "a8/core/PidControl.hpp"

namespace a8 {
namespace core {
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
    float deltaTime = now - lastUpdateTimeInMs;
    float p = kp * error;             // speed
    float i = ki * error * deltaTime; //
    float d = 0.0f;
    if (deltaTime >= 1.0f) {
        d = kd * (error - lastError) / deltaTime; // speed plus
    }

    lastUpdateTimeInMs = now;
    return p + i + d;
}
} // namespace core
} // namespace a8
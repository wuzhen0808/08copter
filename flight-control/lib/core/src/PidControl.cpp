#include "a8/core/PidControl.hpp"
#include "a8/rtos/Thread.hpp"
#include <Arduino.h>
namespace a8 {
namespace core {
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
    float deltaTime = millis() - lastUpdateTimeInMs;
    float p = kp * error;                           // speed
    float i = ki * error * deltaTime;               //
    float d = kd * (error - lastError) / deltaTime; // speed plus
    return p + i + d;
}
} // namespace core
} // namespace a8
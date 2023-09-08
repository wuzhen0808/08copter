#include "PidControl.h"
#include "thread.h"
#include <Arduino.h>
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
    float p = kp * error;//speed
    float i = ki * error * deltaTime;//
    float d = kd * (error - lastError) / deltaTime;//speed plus
    return p + i + d;
}

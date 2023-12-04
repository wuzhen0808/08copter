#pragma once
namespace a8::fc {
class Pid {

    // pid
    float pwm;
    
    /////////////////PID CONSTANTS/////////////////
    double kp = 3.55;  // 3.55
    double ki = 0.003; // 0.003
    double kd = 2.05;  // 2.05
    float previous_error;

public:
    void update(float error, float elapsedTimeSec) {
        float p = kp * error;
        // if (-3 < error < 3) {
        //     i = i + (ki * error);
        // }
        float i = 0;
        float d = kd * ((error - previous_error) / elapsedTimeSec);
        //
        pwm = p + i + d;
        if (pwm < -1000) {
            pwm = -1000;
        }
        if (pwm > 1000) {
            pwm = 1000;
        }
        previous_error = error;
    }

    long getPwm() {
        return this->pwm;
    }
};
} // namespace a8::fc
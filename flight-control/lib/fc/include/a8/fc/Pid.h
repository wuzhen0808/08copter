#pragma once
namespace a8::fc {
class Pid {

    // pid
    float pwm;

    /////////////////PID CONSTANTS/////////////////
    double kp = 3.55;  // 3.55
    double ki = 0.003; // 0.003
    double kd = 2.05;  // 2.05
    float i = 0;
    float lastError = 0;
    long lastTimeMs = -1;
    long &timeMs;

public:
    Pid(long &timeMs) : timeMs(timeMs) {
    }
    void update(float actual, float desired, String &msg) {

        if (lastTimeMs < 0) {
            lastTimeMs = timeMs;
        }

        float error = desired - actual;
        float p = kp * error;
        if (-3 < error < 3) {
            i = i + (ki * error);
        }
        float d = 0;
        long elapsedTime = timeMs - lastTimeMs;
        if (elapsedTime > 0) {
            d = kd * ((error - lastError) / (elapsedTime / 1000.0f));
        }
        //
        pwm = p + i + d;
        //msg << "pid:(" << pwm << "=" << p << "+" << i << "+" << d << ")";
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
} // namespace a8::fc
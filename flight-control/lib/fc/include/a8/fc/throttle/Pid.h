#pragma once
#include "a8/util.h"
namespace a8::fc::throttle {
using namespace a8::util;

class Pid {

    float outputLimit = 0;
    float integralOutputLimit = 0;

    // pid
    float output;

    double kp = 0; // 3.55
    double ki = 0; // 0.003
    double kd = 0; // 2.05
    //
    float p = 0;
    float i = 0;
    float d = 0;
    float lastError = 0;
    long lastTimeMs = -1;
    //
public:
    float historyMinOutputP;
    float historyMinOutputI;
    float historyMinOutputD;
    float historyMinOutput;
    float historyMaxOutputP;
    float historyMaxOutputI;
    float historyMaxOutputD;
    float historyMaxOutput;

public:
    Pid() {
    }

    void config(double kp, double ki, double kd, float outputLimit, float integralOutputLimit) {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
        this->outputLimit = outputLimit;
        this->integralOutputLimit = integralOutputLimit;
        this->resetHistory();
    }
    void resetHistory() {
        this->historyMinOutputP = outputLimit;
        this->historyMinOutputI = integralOutputLimit;
        this->historyMinOutputD = outputLimit;
        this->historyMinOutput = outputLimit;
        this->historyMaxOutputP = -outputLimit;
        this->historyMaxOutputI = -integralOutputLimit;
        this->historyMaxOutputD = -outputLimit;
        this->historyMaxOutput = -outputLimit;
    }

    void printHistory(int depth, String &msg) {
        msg << StringUtil::space(depth) << "historyMaxOutputP:" << this->historyMaxOutputP << "\n";
        msg << StringUtil::space(depth) << "historyMaxOutputI(limit:" << this->integralOutputLimit << "):" << this->historyMaxOutputI << "\n";
        msg << StringUtil::space(depth) << "historyMaxOutput(limit:" << this->outputLimit << "):" << this->historyMaxOutput << "\n";
    }

    void updateHistoryMinMax(float fValue, float &min, float &max) {
        if (fValue < min) {
            min = fValue;
        }
        if (fValue > max) {
            max = fValue;
        }
    }
    void updateHistory() {
        this->updateHistoryMinMax(this->output, this->historyMinOutput, this->historyMaxOutput);
        this->updateHistoryMinMax(this->p, this->historyMinOutputP, this->historyMaxOutputP);
        this->updateHistoryMinMax(this->i, this->historyMinOutputI, this->historyMaxOutputI);
        this->updateHistoryMinMax(this->d, this->historyMinOutputD, this->historyMaxOutputD);
    }

    void update(long timeMs, float actual, float desired, String &msg) {

        if (lastTimeMs < 0) {
            lastTimeMs = timeMs;
        }

        float error = desired - actual;
        p = kp * error;
        float elapsedTimeSec = (timeMs - lastTimeMs) / 1000.0f;
        // if (-3 < error < 3) {
        i = i + (ki * error * elapsedTimeSec);
        limit(i, -integralOutputLimit, integralOutputLimit);
        // }
        d = 0;
        if (elapsedTimeSec > 0) {
            d = kd * ((error - lastError) / elapsedTimeSec);
        }
        //
        output = p + i + d;
        limit(output, -outputLimit, outputLimit);

        updateHistory();

        lastError = error;
        lastTimeMs = timeMs;
    }

    void limit(float &output, float min, float max) {
        if (output > max) {
            output = max;
        } else if (output < min) {
            output = min;
        }
    }
    float getLastError() {
        return this->lastError;
    }

    long getPwm() {
        return this->output;
    }
};
} // namespace a8::fc::throttle
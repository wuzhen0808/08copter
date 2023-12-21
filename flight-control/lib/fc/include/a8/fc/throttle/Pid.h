#pragma once
#include "a8/fc/collect/Collector.h"
#include "a8/util.h"
namespace a8::fc::throttle {
using namespace a8::util;
using namespace a8::fc::collect;

class Pid : public FlyWeight {

    float outputLimit = 0;
    float outputLimitI = 0;

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
    void limit(float &output, float min, float max) {
        if (output > max) {
            output = max;
        } else if (output < min) {
            output = min;
        }
    }

public:
    Pid(LoggerFactory *logFac, String name) : FlyWeight(logFac, name) {
    }

    void config(double kp, double ki, double kd, float outputLimit, float integralOutputLimit) {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
        this->outputLimit = outputLimit;
        this->outputLimitI = integralOutputLimit;
    }

    void setup() {
    }

    void collectDataItems(Collector &collector) {
        collector.add<float>(String(this->name)<<"-lmt", this->outputLimit);
        collector.add<float>(String(this->name)<<"-lmtI", this->outputLimitI);
        collector.add<float>(String(this->name) << "-p", this->p);
        collector.add<float>(String(this->name) << "-i", this->i);
        collector.add<float>(String(this->name) << "-d", this->d);
        collector.add<float>(String(this->name), this->output);
    }

    float getOutputLimit() {
        return outputLimit;
    }

    void update(long timeMs, float actual, float desired) {
        A8_LOG_DEBUG(logger, String() << ">>update," << actual << "," << desired);
        if (lastTimeMs < 0) {
            lastTimeMs = timeMs;
        }

        float error = desired - actual;
        p = kp * error;
        float elapsedTimeSec = (timeMs - lastTimeMs) / 1000.0f;
        // if (-3 < error < 3) {
        i = i + (ki * error * elapsedTimeSec);
        limit(i, -outputLimitI, outputLimitI);
        // }
        d = 0;
        if (elapsedTimeSec > 0) {
            d = kd * ((error - lastError) / elapsedTimeSec);
        }
        //
        output = p + i + d;
        limit(output, -outputLimit, outputLimit);

        lastError = error;
        lastTimeMs = timeMs;
        A8_LOG_DEBUG(logger, String() << "<<update," << p << "," << i << "," << d << ","
                                      << "output:" << output);
    }

    float getLastError() {
        return this->lastError;
    }
    float getP() {
        return p;
    }
    float getI() {
        return i;
    }
    float getD() {
        return d;
    }

    float getOutput() {
        return this->output;
    }
};
} // namespace a8::fc::throttle
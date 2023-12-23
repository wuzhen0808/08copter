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
    float error = 0;
    float elapsedTimeSec = 0;
    float errorDiff = 0;
    //
public:
    void limit(float &output, float min, float max) {
        if (output > max) {
            output = max;
        }

        if (output < min) {
            output = min;
        }
    }

public:
    Pid(LoggerFactory *logFac, String name) : FlyWeight(logFac, name) {
    }

    void config(double kp, double ki, double kd, float outputLimit, float limitI) {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
        this->outputLimit = outputLimit;
        this->outputLimitI = limitI;
    }

    void setup() {
    }

    void collectDataItems(Collector &collector) {
        collector.add<float>(String(this->name) << "-err", error);
        collector.add<float>(String(this->name) << "-errDiff", errorDiff);
        collector.add<float>(String(this->name) << "-ets", this->elapsedTimeSec);
        collector.add<float>(String(this->name) << "-lmt", this->outputLimit);
        collector.add<float>(String(this->name) << "-lmtI", this->outputLimitI);
        collector.add<float>(String(this->name) << "-p", this->p);
        collector.add<float>(String(this->name) << "-i", this->i);
        collector.add<float>(String(this->name) << "-d", this->d);
        collector.add<float>(String(this->name << "-output"), this->output);
    }

    float getOutputLimit() {
        return outputLimit;
    }

    void update(long timeMs, float actual, float desired) {
        A8_LOG_DEBUG(logger, String() << ">>update," << actual << "," << desired);
        if (lastTimeMs < 0) {
            lastTimeMs = timeMs;
        }

        error = desired - actual;
        p = kp * error;
        elapsedTimeSec = (timeMs - lastTimeMs) / 1000.0f;
        // if (-3 < error < 3) {
        i = i + (ki * error * elapsedTimeSec);
        limit(i, -outputLimitI, outputLimitI);
        // }
        d = 0;
        errorDiff = error - lastError;
        if (elapsedTimeSec > 0) {

            d = kd * (errorDiff / elapsedTimeSec);
        }
        //
        output = p + i + d;
        limit(output, -outputLimit, outputLimit);

        lastError = error;
        lastTimeMs = timeMs;
        A8_LOG_DEBUG(logger, String() << "<<update(float):" << p << "," << i << "," << d << ","
                                      << "output:" << output);
        A8_LOG_DEBUG(logger, String() << "<<update(double):" << (double)p << "," << (double)i << "," << (double)d << ","
                                      << "output:" << (double)output);
        Format::AutoOffsetFloat format(16, 3);
        String msg;
        msg.setFloatFormat(&format);
        msg << "<<update(format,float):" << p << "," << i << "," << d << ","
            << "output:" << output << "\n";
        msg << "<<update(format,double):" << (double)p << "," << (double)i << "," << (double)d << ","
            << "output:" << (double)output;
        A8_LOG_DEBUG(logger, msg);
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
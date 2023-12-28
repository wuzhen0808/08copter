#pragma once
#include "a8/fc/collect/Collector.h"
#include "a8/util.h"
// More reference:
//  https://github.com/bitcraze/crazyflie-firmware.git
//  https://www.youtube.com/watch?v=NVLXCwc8HzM

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
    long ticks = 0;

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

    int collectDataItems(Collector *collector, Result &res) {
        int ret = 1;
        if (ret > 0)
            ret = collector->add(String(this->name) << "-err", error, res);
        if (ret > 0)
            ret = collector->add(String(this->name) << "-errDiff", errorDiff, res);
        String etsName = String(this->name) << "-ets";
        if (ret > 0)
            ret = collector->add(etsName, this->elapsedTimeSec, res);
        if (ret > 0)
            ret = collector->add(String(this->name) << "-lmt", this->outputLimit, res);
        if (ret > 0)
            ret = collector->add(String(this->name) << "-lmtI", this->outputLimitI, res);
        if (ret > 0)
            ret = collector->add(String(this->name) << "-p", this->p, res);
        if (ret > 0)
            ret = collector->add(String(this->name) << "-i", this->i, res);
        if (ret > 0)
            ret = collector->add(String(this->name) << "-d", this->d, res);
        if (ret > 0)
            ret = collector->add(String(this->name << "-o"), this->output, res);
        return ret;
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
        if (ticks == 0) {
            lastError = error;
        }
        errorDiff = error - lastError;
        if (elapsedTimeSec > 0) {

            d = kd * (errorDiff / elapsedTimeSec);
        }
        //
        output = p + i + d;
        limit(output, -outputLimit, outputLimit);

        A8_LOG_DEBUG(logger, String(name) << "timeMs(" << timeMs << ")-lastTimeMs(" << lastTimeMs << ")/1000=elapsedTimeSec?(" << elapsedTimeSec << ")");
        A8_LOG_DEBUG(logger, String(name) << "error(" << error << ")-lastError(" << lastError << ")=errorDiff(" << errorDiff << ")/elapsedTimeSec(" << elapsedTimeSec << ")=?" << d);
        A8_LOG_DEBUG(logger, String(name) << ">>update,p:" << p << ",i:" << i << ",d:" << d << ","
                                          << "output:" << output);
        A8_DEBUG7("timeMs(", timeMs, ")-lastTimeMs(", lastTimeMs, ")/1000=elapsedTimeSec?(", elapsedTimeSec, ")");
        A8_DEBUG10("error(", error, ")-lastError(", lastError, ")=errorDiff(", errorDiff, ")/elapsedTimeSec(", elapsedTimeSec, ")=?", d);
        A8_DEBUG8(">>update,p:", p, ",i:", i, ",d:", d, ",output:", output);

        lastTimeMs = timeMs;
        lastError = error;
        ticks++;
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
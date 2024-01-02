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
    long lastTimeMs = -1;
    //
    float elapsedTimeSec = 0;
    float error = 0;
    float dError = 0;
    float dLastError = 0;
    float dErrorDiff = 0;
    //
    long ticks = 0;
    Filter *filter_;
    void (*releaseFilter_)(Filter*);

public:
    void limit(float &output, float min, float max) {
        if (output > max) {
            output = max;
        }

        if (output < min) {
            output = min;
        }
    }
    void releaseErrorDiffFilter(){
        if(this->filter_ != 0){
            this->releaseFilter_(this->filter_);
            this->filter_ = 0;
        }
    }

public:
    Pid(LoggerFactory *logFac, String name) : FlyWeight(logFac, name) {
        this->filter_ = 0;
    }
    ~Pid() {
        releaseErrorDiffFilter();
    }
    
    void config(double kp, double ki, double kd, float outputLimit, float limitI) {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
        this->outputLimit = outputLimit;
        this->outputLimitI = limitI;
    }

    void setErrorDiffFilter(Filter *lpf, void (*release)(Filter*)) {
        this->releaseErrorDiffFilter();
        this->filter_ = lpf;
    }

    void setup() {
    }

    int collectDataItems(Collector *collector, Result &res) {
        int ret = 1;
        if (ret > 0)
            ret = collector->add(String(this->name) << "-err", error, res);
        if (ret > 0)
            ret = collector->add(String(this->name) << "-dErr", dError, res);    
        if (ret > 0)
            ret = collector->add(String(this->name) << "-dErrDiff", dErrorDiff, res);        
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

    void update(long timeMs, float actual, float dTermActual, float desired) {
        A8_LOG_DEBUG(logger, String() << ">>update," << actual << "," << desired);
        if (lastTimeMs < 0) {
            lastTimeMs = timeMs;
        }
        error = actual - desired;        
        dError = dTermActual - desired;
        p = kp * error;
        elapsedTimeSec = (timeMs - lastTimeMs) / 1000.0f;
        // if (-3 < error < 3) {
        i = i + (ki * error * elapsedTimeSec);
        limit(i, -outputLimitI, outputLimitI);
        // }
        d = 0;
        if (ticks == 0) {
            dLastError = dError;
        }
        dErrorDiff = dError - dLastError;
        if (elapsedTimeSec > 0) {

            d = kd * (dErrorDiff / elapsedTimeSec);
        }
        //
        output = p + i + d;
        limit(output, -outputLimit, outputLimit);

        A8_LOG_DEBUG(logger, String(name) << "timeMs(" << timeMs << ")-lastTimeMs(" << lastTimeMs << ")/1000=elapsedTimeSec?(" << elapsedTimeSec << ")");
        A8_LOG_DEBUG(logger, String(name) << "error(" << error << ")-lastError(" << lastError << ")=rawErrorDiff(" << rawErrorDiff << ")/elapsedTimeSec(" << elapsedTimeSec << ")=?" << d);
        A8_LOG_DEBUG(logger, String(name) << ">>update,p:" << p << ",i:" << i << ",d:" << d << ","
                                          << "output:" << output);
        A8_DEBUG7("timeMs(", timeMs, ")-lastTimeMs(", lastTimeMs, ")/1000=elapsedTimeSec?(", elapsedTimeSec, ")");
        A8_DEBUG10("error(", error, ")-lastError(", lastError, ")=rawErrorDiff(", rawErrorDiff, ")/elapsedTimeSec(", elapsedTimeSec, ")=?", d);
        A8_DEBUG8(">>update,p:", p, ",i:", i, ",d:", d, ",output:", output);

        lastTimeMs = timeMs;
        dLastError = dError;

        ticks++;
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
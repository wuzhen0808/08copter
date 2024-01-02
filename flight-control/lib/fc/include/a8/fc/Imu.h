#pragma once
#include "a8/fc/collect/Collector.h"
#include "a8/util.h"
namespace a8::fc {
using namespace a8::util;
using namespace a8::fc::collect;
class Imu {
public:
    virtual void setup() = 0;
    virtual bool update() = 0;
    int collectDataItems(Collector *collector, Result &res) {
        return 1;
    }
    virtual void get(float *rpy) = 0;
    virtual float getRoll() = 0;
    virtual float getPitch() = 0;
    virtual float getYaw() = 0;
    virtual void getGyro(float *xyz) = 0;

    virtual int checkReady(float limit, Result &res) = 0;
    virtual int checkStable(Result &res) = 0;
    int checkStable(int retries, Result &res) {
        int ret = -1;
        for (int i = 0; i < retries && ret < 0; i++) {
            ret = this->checkStable(res);
        }
        return ret;
    }
    virtual int checkBalance(bool silence, float limit, float &deg, Result &res) = 0;
    virtual bool isBalance(float limit, bool silence) = 0;
    virtual String getLastError() = 0;
    virtual void setFilter(int filter) = 0;
};
} // namespace a8::fc
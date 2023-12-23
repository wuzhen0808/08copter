#pragma once
#include "a8/fc/collect/Collector.h"
#include "a8/util.h"
namespace a8::fc {
using namespace a8::util;
using namespace a8::fc::collect;
class Rpy {
public:
    virtual void setup() = 0;
    virtual bool update() = 0;
    void collectDataItems(Collector &collector) {
        collector.add<Rpy *>("Roll", this, [](Rpy *this_) { return (double)this_->getRoll(); });
        collector.add<Rpy *>("Pitch", this, [](Rpy *this_) { return (double)this_->getPitch(); });
        collector.add<Rpy *>("Yaw", this, [](Rpy *this_) { return (double)this_->getYaw(); });
    }
    virtual void get(float &roll, float &pitch, float &yaw) = 0;
    virtual float getRoll() = 0;
    virtual float getPitch() = 0;
    virtual float getYaw() = 0;
    virtual int checkReady(Result &res) = 0;
    virtual int checkStable(Result &res) = 0;
    int checkStable(int retries, Result &res) {
        int ret = -1;
        for (int i = 0; i < retries && ret < 0; i++) {
            ret = this->checkStable(res);
        }
        return ret;
    }
    virtual int checkBalance(Result &res) = 0;
    virtual bool isBalance() = 0;
    virtual String getLastError() = 0;
};
} // namespace a8::fc
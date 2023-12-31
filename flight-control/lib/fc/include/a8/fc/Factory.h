#pragma once
#include "a8/fc/Commander.h"
#include "a8/fc/PowerManage.h"
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"

namespace a8::fc {
class Factory {
protected:
    System *sys;
    Scheduler *sch;
    LoggerFactory *logFac;

public:
    void populate(StagingContext *sc) {
        this->sys = sc->getSys();
        this->sch = sc->scheduler;
        this->logFac = sc->loggerFactory;
    }
    virtual Imu *newImu() = 0;
    virtual void setupWire() = 0;
    virtual Propeller *newPropeller(String name, int idx) = 0;
    virtual Filter *newLowPassFilter(Rate cutOffRate, Rate samplingRate, int order);
};
} // namespace a8::fc
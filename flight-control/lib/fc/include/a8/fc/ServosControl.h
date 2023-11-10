#pragma once
#include "a8/util/Result.h"
#include "a8/util/comp.h"


namespace a8::fc {
using namespace a8::util;
using namespace a8::util::comp;

class ServosControl : public FlyWeight {
protected:
    int totalServos;

public:
    ServosControl(int totalServos, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->totalServos = totalServos;
    }
    ~ServosControl() {
    }
    virtual void setThrottleNorm(int servoId, float velocity) = 0;
    void setThrottleNorm(int id1, float vel1, int id2, float vel2, int id3, float vel3, int id4, float vel4) {
        setThrottleNorm(id1, vel1);
        setThrottleNorm(id2, vel2);
        setThrottleNorm(id3, vel3);
        setThrottleNorm(id4, vel4);
    }
};

} // namespace a8::fc

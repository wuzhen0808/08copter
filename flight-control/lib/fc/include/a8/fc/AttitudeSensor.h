#pragma once
#include "a8/util/Vector3f.h"
#include "a8/util/comp.h"


namespace a8::fc {
using namespace a8::util;
using namespace a8::util::comp;

class AttitudeSensor {
public:
    AttitudeSensor() {}
    ~AttitudeSensor() {}

    virtual int isReady(Result &rst) = 0;
    virtual int getAngVel(Vector3f &angVel, Result &rst) = 0;
    virtual int getAccVel(Vector3f &accVel, Result &rst) = 0;
    virtual int getAltitude(double &altitude, Result &rst) = 0;
    virtual int getLongitude(double &longitude, Result &rst) = 0;
};
} // namespace a8::fc
#pragma once
#include "a8/fc/AttitudeSensor.h"
#include "a8/fc/FlightControl.h"
#include "a8/util/comp/Component.h"
namespace a8::fc::arduino {
using namespace a8::fc;

class Mpu9250AttitudeSensor : public AttitudeSensor {
public:
    Mpu9250AttitudeSensor();
    ~Mpu9250AttitudeSensor();
    int isReady(Result &rst);
    int getAngVel(Vector3f &angVel, Result &rst);
    int getAccVel(Vector3f &accVel, Result &rst);
    int getAltitude(double &altitude, Result &rst);
    int getLongitude(double &longitude, Result &rst);

private:
    void* mpu;
};
} // namespace a8::fc::arduino
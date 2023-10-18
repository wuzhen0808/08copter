#pragma once
#include "a8/fc/AttitudeSensor.h"
namespace a8::fc::arduino {
using namespace a8::fc;

class Mpu9250AttitudeSensor : public AttitudeSensor, public FlyWeight {
public:
    Mpu9250AttitudeSensor(LoggerFactory *logFac);
    ~Mpu9250AttitudeSensor();
    int isReady(Result &rst) override;
    int getAngVel(Vector3f &angVel, Result &rst) override;
    int getAccVel(Vector3f &accVel, Result &rst) override;
    int getAltitude(double &altitude, Result &rst) override;
    int getLongitude(double &longitude, Result &rst) override;

private:
    void *mpu;
};
} // namespace a8::fc::arduino
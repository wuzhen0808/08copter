#ifndef MPU9250_ATTITUDE_SENSOR__
#define MPU9250_ATTITUDE_SENSOR__
#include "a8/fc/AttitudeSensor.h"
#include "a8/util/comp/Component.h"
#include "a8/fc/FlightControl.h"
#include <MPU9250.h>
namespace a8 {
namespace mpu9250 {
using namespace a8::fc;

class Mpu9250AttitudeSensor : public AttitudeSensor {
public:
    Mpu9250AttitudeSensor(Copter *copter);
    ~Mpu9250AttitudeSensor();
    virtual void update();
    virtual float getRoll();
    virtual float getPitch();
    virtual float getYaw();

private:
    MPU9250 mpu;
};
} // namespace mpu9250
} // namespace a8
#endif
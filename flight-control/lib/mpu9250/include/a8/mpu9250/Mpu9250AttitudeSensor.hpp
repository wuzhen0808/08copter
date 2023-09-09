#ifndef MPU9250_ATTITUDE_SENSOR__
#define MPU9250_ATTITUDE_SENSOR__
#include "a8/AttitudeSensor.hpp"
#include "a8/Component.hpp"
#include "a8/Copter.hpp"
#include <MPU9250.h>
namespace a8 {
namespace mpu9250 {

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
#ifndef MPU9250_ATTITUDE_SENSOR__
#define MPU9250_ATTITUDE_SENSOR__
#include "Component.h"
#include "Copter.h"
#include "AttitudeSensor.h"
#include "MPU9250.h"
class Mpu9250AttitudeSensor : public AttitudeSensor,public Component {
public:
    Mpu9250AttitudeSensor(Copter* copter);
    ~Mpu9250AttitudeSensor();
    virtual void update();
    virtual float getRoll();
    virtual float getPitch();
    virtual float getYaw();
private:
    MPU9250 mpu;
};
#endif
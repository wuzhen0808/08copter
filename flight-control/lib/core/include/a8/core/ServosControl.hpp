#ifndef SERVOS_CONTROL__
#define SERVOS_CONTROL__
#include "a8/core/Component.hpp"
#include <Arduino.h>
#include <Servo.h>
#include <stdlib.h>
namespace a8 {
namespace core {
class ServosControl : public Component {

public:
    ServosControl(Copter *copter);
    ~ServosControl();
    ServosControl *attachAll();
    void setVelocity(int servoId, float velocity);
    void setVelocities(int id1, float vel1,int id2, float vel2,int id3, float vel3,int id4, float vel4);

private:
    int totalServos;
    Servo **servos;
    float lastVelocities[4] = {};
};
} // namespace core
} // namespace a8
#endif
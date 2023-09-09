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

private:
    int totalServos;
    Servo **servos;
};
} // namespace core
} // namespace a8
#endif
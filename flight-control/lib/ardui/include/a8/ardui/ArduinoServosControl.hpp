#ifndef ARDUINO_SERVOS_CONTROL_H__
#define ARDUINO_SERVOS_CONTROL_H__
#include "a8/core/ServosControl.hpp"
#include <Servo.h>
namespace a8 {
namespace ardui {
using namespace a8::core;

class ArduinoServosControl : public ServosControl {
public:
    ArduinoServosControl(Copter *copter);
    ~ArduinoServosControl();
    virtual void attachAll();
    virtual void setVelocity(int servoId, float velocity);
    virtual void setVelocities(int id1, float vel1, int id2, float vel2, int id3, float vel3, int id4, float vel4);

private:
    Servo **servos;
    int totalServos;
    float lastVelocities[4] = {};
};

} // namespace ardui

} // namespace a8
#endif
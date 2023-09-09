#ifndef SERVOS_CONTROL__
#define SERVOS_CONTROL__
#include <Arduino.h>
#include <Servo.h>
#include "a8/Component.hpp"
#include <stdlib.h>
namespace a8 {
class ServosControl: public Component {

public:    
    ServosControl(Copter* copter);
    ~ServosControl();    
    ServosControl* attachAll();
    void setVelocity(int servoId, float velocity);

private:
    int totalServos;
    Servo** servos;    
};
}
#endif
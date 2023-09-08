#ifndef SERVOS_CONTROL__
#define SERVOS_CONTROL__
#include <Arduino.h>
#include <Servo.h>
#include "Component.h"
#include <stdlib.h>
class ServosControl: public Component {

public:    
    ServosControl(Copter* copter);
    ~ServosControl();    
    void active();
    void setVelocity(int servoId, float velocity);

private:
    int totalServos;
    Servo** servos;    
};

#endif
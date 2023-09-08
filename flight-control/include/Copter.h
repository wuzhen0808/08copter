#ifndef FLIGHT_CONFIG__
#define FLIGHT_CONFIG__

#include <Arduino.h>

#define SERVO_TOP_LEFT 0
#define SERVO_TOP_RIGHT 1
#define SERVO_BOTTOM_RIGHT 2
#define SERVO_BOTTOM_LEFT 3

class Copter {
public:
    Copter();
    ~Copter();
    int getServoCount();
    int getServoAttachPin(int servoId);
    void active();
    void destroy();
    void log(String message);
private:    
    int servoCount = 4;
    int *servoAttachPins = new int[4]{3, 5, 9, 11};
    
};
#endif
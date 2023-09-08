#include "ServosControl.h"
#include <Servo.h>

#define MIN_PULSE 1000
#define MAX_PULSE 2000

ServosControl::ServosControl(Copter *copter) : Component(copter) {
    (*this).totalServos = (*copter).getServoCount();
    (*this).servos = new Servo *[totalServos];
    for (int i = 0; i < (*this).totalServos; i++) {
        (*this).servos[i] = new Servo();
    }
}
void ServosControl::active() {
    log("ServosControl::active");
    for (int i = 0; i < (*this).totalServos; i++) {
        int pin = (*copter).getServoAttachPin(i);
        (*(*this).servos[i]).attach(pin);
        log(String("Servo[") + String(i) + String("] is attached to:") + String(pin));

        (*(*this).servos[i]).writeMicroseconds(MIN_PULSE); // initialize
    }
}

ServosControl::~ServosControl() {
}

void ServosControl::setVelocity(int servoId, float velocity) {
    log("ServosControl::setVelocity");
    long pulseWidth = map(velocity, 0, velocity * 1000, MIN_PULSE, MAX_PULSE);
    pulseWidth = 1300;
    log("pw:" + String(pulseWidth));

    (*((*this).servos)[servoId]).writeMicroseconds(pulseWidth);
}
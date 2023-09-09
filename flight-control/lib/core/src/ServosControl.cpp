#include "a8/core/ServosControl.hpp"
#include <Servo.h>

#define MIN_PULSE 1000
#define MAX_PULSE 2000
namespace a8 {
namespace core {
ServosControl::ServosControl(Copter *copter) : Component(copter) {
    (*this).totalServos = (*copter).getServoCount();
    (*this).servos = new Servo *[totalServos];
    for (int i = 0; i < (*this).totalServos; i++) {
        (*this).servos[i] = new Servo();
    }
}
ServosControl *ServosControl::attachAll() {
    log("ServosControl::active");
    for (int i = 0; i < (*this).totalServos; i++) {
        int pin = (*copter).getServoAttachPin(i);
        uint8_t isOk = (*(*this).servos[i]).attach(pin);
        if (isOk) {
            log(String("Servo[") + String(i) + String("] is Successfully attached to:") + String(pin));
        } else {
            log(String("Failed for Servo[") + String(i) + String("] to be attached to:") + String(pin));
        }

        (*(*this).servos[i]).writeMicroseconds(MIN_PULSE); // initialize
    }
    return this;
}

ServosControl::~ServosControl() {
}

void ServosControl::setVelocity(int servoId, float velocity) {
    log("ServosControl::setVelocity");
    long pulseWidth = map(velocity, 0, velocity * 1000, MIN_PULSE, MAX_PULSE);
    // pulseWidth = 1300;
    log("pw:" + String(pulseWidth));

    (*((*this).servos)[servoId]).writeMicroseconds(pulseWidth);
}
} // namespace core
} // namespace a8
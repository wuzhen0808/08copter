#include "a8/arduino/ArduinoServosControl.h"
#include "a8/hal/Hal.h"
#include "a8/util/Chars.h"
#include "a8/util/Math.h"

namespace a8 {
namespace arduino {
using namespace a8::util;
using namespace a8::core;
using namespace a8::hal;
using a8::util::Math;

static int SERVO_MIN_PULSE = 1000;
static int SERVO_MAX_PULSE = 2000;

long convertToPulseWidth(float velocity) {

    long pulseWidth = Math::map(velocity * 1000, 0, 1000, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
    return pulseWidth;
}
ArduinoServosControl::ArduinoServosControl(Copter *copter) : ServosControl(copter) {
    this->totalServos = copter->getServoCount();
    this->servos = new Servo *[totalServos];
    for (int i = 0; i < (*this).totalServos; i++) {
        this->servos[i] = new Servo();
    }
}

void ArduinoServosControl::attachAll() {
    //log("ServosControl::active");
    for (int i = 0; i < this->totalServos; i++) {
        int pin = copter->getServoAttachPin(i);
        uint8_t isOk = this->servos[i]->attach(pin);
        if (isOk) {

            //log("Servo[%i] is Successfully attached to:%i");
        } else {
           // log("Failed for Servo[%i] to be attached to:%i");
        }

        this->servos[i]->writeMicroseconds(SERVO_MIN_PULSE); // initialize
    }
}

ArduinoServosControl::~ArduinoServosControl() {
}

void ArduinoServosControl::setVelocity(int servoId, float velocity) {
    long pulseWidth = convertToPulseWidth(velocity);
    // long pulseWidth = 1200;
    this->servos[servoId]->writeMicroseconds(pulseWidth);
    lastVelocities[servoId] = velocity;
    char buf[100];
    // sprintf(buf, "%f",velocity);
    // log("pulseWidth:" );
}

void ArduinoServosControl::setVelocities(int id1, float vel1, int id2, float vel2, int id3, float vel3, int id4, float vel4) {
    // log("serVelocities:[" +
    // String(id1) + ":" + String(vel1) + "," +
    // String(id2) + ":" + String(vel2) + "," +
    // String(id3) + ":" + String(vel3) + "," +
    // String(id4) + ":" + String(vel4) + "]");
    setVelocity(id1, vel1);
    setVelocity(id2, vel2);
    setVelocity(id3, vel3);
    setVelocity(id4, vel4);
}

} // namespace ardui
} // namespace a8
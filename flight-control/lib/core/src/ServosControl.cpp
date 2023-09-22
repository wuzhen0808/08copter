#include "a8/core/ServosControl.h"
#include "a8/util/Math.h"

namespace a8::core {

using namespace a8::util;
using namespace a8::core;
using namespace a8::hal;
using a8::util::Math;

ServosControl::ServosControl(int totalServos) {

    this->totalServos = totalServos;
    this->lastVelocities = new float[totalServos]{0};
}
ServosControl::~ServosControl() {
    delete[] this->lastVelocities;
}

Result ServosControl::setup() {
    log(String::format(">>setup, totalServos:%i", totalServos));

    this->servos = new Servo *[totalServos];
    for (int i = 0; i < (*this).totalServos; i++) {
        this->servos[i] = setupServo(i);
    }
    // log("ServosControl::active");
    for (int i = 0; i < this->totalServos; i++) {
        this->servos[i]->setThrottleNorm(-1.0f); // initial velocity
    }
    return true;
}

void ServosControl::setThrottleNorm(int servoId, float velocity) {
    // long pulseWidth = 1200;
    this->servos[servoId]->setThrottleNorm(velocity);
    lastVelocities[servoId] = velocity;    
}

void ServosControl::setThrottleNorm(int id1, float vel1, int id2, float vel2, int id3, float vel3, int id4, float vel4) {
    // log("serVelocities:[" +
    // String(id1) + ":" + String(vel1) + "," +
    // String(id2) + ":" + String(vel2) + "," +
    // String(id3) + ":" + String(vel3) + "," +
    // String(id4) + ":" + String(vel4) + "]");
    setThrottleNorm(id1, vel1);
    setThrottleNorm(id2, vel2);
    setThrottleNorm(id3, vel3);
    setThrottleNorm(id4, vel4);
}

} // namespace a8::core

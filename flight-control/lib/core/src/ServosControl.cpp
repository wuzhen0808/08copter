#include "a8/core/ServosControl.h"
#include "a8/util/Math.h"

namespace a8::core {

using namespace a8::util;
using namespace a8::core;
using namespace a8::hal;
using a8::util::Math;

ServosControl::ServosControl(int totalServos) : Component("svc") {

    this->totalServos = totalServos;
    this->lastVelocities = new float[totalServos]{0};
}
ServosControl::~ServosControl() {
    delete[] this->lastVelocities;
}

void ServosControl::setThrottleNorm(int servoId, float velNorm) {
    //log(String::format(">>setThrottleNorm,%i:%f", servoId, velNorm));
    this->servos[servoId]->setThrottleNorm(velNorm);
    lastVelocities[servoId] = velNorm;
    //log(String::format("<<setThrottleNorm"));
}

void ServosControl::setThrottleNorm(int id1, float vel1, int id2, float vel2, int id3, float vel3, int id4, float vel4) {
    setThrottleNorm(id1, vel1);
    setThrottleNorm(id2, vel2);
    setThrottleNorm(id3, vel3);
    setThrottleNorm(id4, vel4);
}

} // namespace a8::core

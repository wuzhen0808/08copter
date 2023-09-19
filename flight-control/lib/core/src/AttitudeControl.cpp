#include "a8/core/AttitudeControl.h"
#include "a8/core/ServosControl.h"
#include "a8/core/Thread.h"
#include "a8/util/string.h"
#include "a8/core/Copter.h"
/**
 * (M1) (M2)
 *   \   /
 *    [ ]
 *    /  \
 * (M4) (M3)
 */
namespace a8 {
namespace core {
using namespace a8::util;
AttitudeControl::AttitudeControl(void *context,
                                 ServosControl *servosControl,
                                 AttitudeSensor *attitudeSensor) : Component() {
    rollControl = new PidControl(2.0f, 1.0f, 1.0f);
    pitchControl = new PidControl(.0f, .0f, .0f);
    yawControl = new PidControl(.0f, .0f, .0f);
    this->attitudeSensor = attitudeSensor;
    this->servosControl = servosControl;
}

AttitudeControl::~AttitudeControl() {
}

void AttitudeControl::call() {
    //string msg;

    log(">>AttitudeControl::call()");

    attitudeSensor->update();

    float actualRoll = attitudeSensor->getRoll();
    float actualPitch = attitudeSensor->getPitch();
    float actualYaw = attitudeSensor->getYaw();

    float desiredRoll = 0.0f;
    float desiredPitch = 0.0f;
    float desiredYaw = 0.0f;

    float cmdRoll = rollControl->update(desiredRoll, actualRoll);
    float cmdPitch = pitchControl->update(desiredPitch, actualPitch);
    float cmdYaw = yawControl->update(desiredYaw, actualYaw);

    //log(string("actualRoll:") + string(actualRoll) + string(",rollGain:") + string(rollGain));

    float heave = 0.0f;
    float motors[4] = {};

    // float m2 = throttle + cmdRoll - cmdPitch + cmdYaw; // FR: Front right
    // float m4 = throttle - cmdRoll + cmdPitch + cmdYaw; // AL: After left
    // float m1 = throttle - cmdRoll - cmdPitch - cmdYaw; // FL: Front left
    // float m3 = throttle + cmdRoll + cmdPitch - cmdYaw; // AR: After right

    float fr = heave - cmdRoll + cmdPitch + cmdYaw; // FR: Front right
    float al = heave + cmdRoll - cmdPitch + cmdYaw; // AL: After left
    float fl = heave + cmdRoll + cmdPitch - cmdYaw; // FL: Front left
    float ar = heave - cmdRoll - cmdPitch - cmdYaw; // AR: After right



    servosControl->setVelocities(SERVO_FRONT_LEFT, fl, SERVO_FRONT_RIGHT, fr, SERVO_AFTER_RIGHT, ar, SERVO_AFTER_LEFT, al);
    log("<<AttitudeControl::call()");
}
} // namespace core
} // namespace a8
#include "a8/core/AttitudeControl.h"
#include "a8/core/ServosControl.h"
#include "a8/core/Thread.h"
#include "a8/util/string.h"
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
AttitudeControl::AttitudeControl(Copter *copter,
                                 ServosControl *servosControl,
                                 AttitudeSensor *attitudeSensor) : Component(copter) {
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

    //log(msg);

    attitudeSensor->update();

    float actualRoll = attitudeSensor->getRoll();
    float actualPitch = attitudeSensor->getPitch();
    float actualYaw = attitudeSensor->getYaw();

    float desiredRoll = 0.0f;
    float desiredPitch = 0.0f;
    float desiredYaw = 0.0f;

    float rollGain = rollControl->update(desiredRoll, actualRoll);
    float pitchGain = pitchControl->update(desiredPitch, actualPitch);
    float yawGain = yawControl->update(desiredYaw, actualYaw);

    //log(string("actualRoll:") + string(actualRoll) + string(",rollGain:") + string(rollGain));

    float throttle = 0.0f;
    float motors[4] = {};

    float m1 = throttle - rollGain - pitchGain - yawGain; // top left
    float m2 = throttle + rollGain - pitchGain + yawGain; // top right
    float m3 = throttle + rollGain + pitchGain - yawGain; // bottom right
    float m4 = throttle - rollGain + pitchGain + yawGain; // bottom left

    servosControl->setVelocities(SERVO_TOP_LEFT, m1, SERVO_TOP_RIGHT, m2, SERVO_BOTTOM_RIGHT, m3, SERVO_BOTTOM_LEFT, m4);
}
} // namespace core
} // namespace a8
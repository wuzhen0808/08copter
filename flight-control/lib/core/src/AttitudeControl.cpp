#include "a8/core/AttitudeControl.hpp"
#include "a8/core/ServosControl.hpp"
#include "a8/rtos/Thread.hpp"
#include <FreeRTOSConfig.h>
/**
 * (M1) (M2)
 *   \   /
 *    [ ]
 *    /  \
 * (M4) (M3)
 */
namespace a8 {
namespace core {
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
    log("AttitudeControl::call");

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

    //log("actualRoll:" + String(actualRoll) + ",rollGain:" + String(rollGain));

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
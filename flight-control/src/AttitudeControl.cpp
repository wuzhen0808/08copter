#include "AttitudeControl.h"
#include "FreeRTOSConfig.h"
#include "Mpu9250AttitudeSensor.h"
#include "ServosControl.h"
#include "Thread.h"
/**
 * (M1) (M2)
 *   \   /
 *    [ ]
 *    /  \
 * (M4) (M3)
 */

AttitudeControl::AttitudeControl(Copter *copter) : Component(copter) {
    rollControl = new PidControl(2.0f, 1.0f, 1.0f);
    pitchControl = new PidControl(2.0f, 1.0f, 1.0f);
    yawControl = new PidControl(2.0f, 1.0f, 1.0f);
    attitudeSensor = new Mpu9250AttitudeSensor(copter);
    servosControl = new ServosControl(copter);
}

AttitudeControl::~AttitudeControl() {
}

void AttitudeControl::run() {
    log("AttitudeControl::run");
    servosControl->active();
    while (true) {
        (*attitudeSensor).update();

        float actualRoll = (*attitudeSensor).getRoll();
        float actualPitch = (*attitudeSensor).getPitch();
        float actualYaw = (*attitudeSensor).getYaw();

        float desiredRoll = 0.0f;
        float desiredPitch = 0.0f;
        float desiredYaw = 0.0f;

        float rollGain = (*rollControl).update(desiredRoll, actualRoll);
        float pitchGain = (*pitchControl).update(desiredPitch, actualPitch);
        float yawGain = (*yawControl).update(desiredYaw, actualYaw);

        float throttle = 0.0f;
        float motors[4] = {};

        float m1 = throttle - rollGain - pitchGain - yawGain;
        float m2 = throttle + rollGain - pitchGain + yawGain;
        float m3 = throttle + rollGain + pitchGain - yawGain;
        float m4 = throttle - rollGain + pitchGain + yawGain;

        servosControl->setVelocity(SERVO_TOP_LEFT, m1);
        servosControl->setVelocity(SERVO_TOP_RIGHT, m2);
        servosControl->setVelocity(SERVO_BOTTOM_RIGHT, m3);
        servosControl->setVelocity(SERVO_BOTTOM_LEFT, m4);
        log("going to delayUtil");
        Thread::getCurrentThread()->delayUtil(1);
    }
}
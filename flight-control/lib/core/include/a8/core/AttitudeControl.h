#ifndef ATTITUDE_CONTROL__
#define ATTITUDE_CONTROL__
#include "a8/core/AttitudeSensor.h"
#include "a8/core/PidControl.h"
#include "a8/core/ServosControl.h"
#include "a8/core/defines.h"
#include "a8/util/Callback.h"
#include "a8/util/Component.h"
#include "a8/util/Writer.h"

#define FR_SPIN_DIRECTION LEFT_HAND

using namespace a8::util;

namespace a8::core {

class AttitudeControl : public Callback, public Component {

private:
    PidControl *rollControl;
    PidControl *pitchControl;
    PidControl *yawControl;
    AttitudeSensor *attitudeSensor;
    ServosControl *servosControl;
    Writer *dataLog;
    int servoIdxFR;
    int servoIdxFL;
    int servoIdxAR;
    int servoIdxAL;
    // how the Front Right propeller spin, the default direction is clockwise(left hand mode NEG frame).
    // In left hand mode(NEG frame), the quad copter frame inversely have a willing to rotate
    // right hand. Which is the positive direction of angular velocity.

    bool reverseYaw = false;

public:
    AttitudeControl(ServosControl *servosControl,
                    AttitudeSensor *attitudeSensor) : Component() {
        rollControl = new PidControl(.0f, 0.0f, 0.0f);
        pitchControl = new PidControl(2.0f, .0f, .0f);
        yawControl = new PidControl(.0f, .0f, .0f);
        this->attitudeSensor = attitudeSensor;
        this->servosControl = servosControl;
        this->dataLog = 0;
    }
    ~AttitudeControl() {
    }

    virtual void boot(Context &context) override {
        Component::boot(context);
        servoIdxAR = context.properties.getInt(P_fcs_servo_idx_ar, 0);
        servoIdxFL = context.properties.getInt(P_fcs_servo_idx_fl, 1);
        servoIdxAL = context.properties.getInt(P_fcs_servo_idx_al, 2);
        servoIdxFR = context.properties.getInt(P_fcs_servo_idx_fr, 3);
        log(String::format("ar:%i,fl:%i,al:%i,fr:%i,", servoIdxAR, servoIdxFL, servoIdxAL, servoIdxFR));
    }
    virtual void setup(Context &context) override {
        Component::setup(context);
    }
    virtual void call() {
        // string msg;

        log(">>AttitudeControl::call()");

        Vector3f aVel1 = attitudeSensor->getAngVel();
        Vector3f aVel2 = Vector3f(0.0, 0.5, 0.0);

        float cmdRoll = rollControl->update(aVel2.x, aVel1.x);
        float cmdPitch = pitchControl->update(aVel2.y, aVel1.y);
        float cmdYaw = yawControl->update(aVel2.z, aVel1.z);

        // log(string("actualRoll:") + string(actualRoll) + string(",rollGain:") + string(rollGain));

        float heave = .0f;

        // float m2 = throttle + cmdRoll - cmdPitch + cmdYaw; // FR: Front right
        // float m4 = throttle - cmdRoll + cmdPitch + cmdYaw; // AL: After left
        // float m1 = throttle - cmdRoll - cmdPitch - cmdYaw; // FL: Front left
        // float m3 = throttle + cmdRoll + cmdPitch - cmdYaw; // AR: After right

        float yawSign = this->reverseYaw ? -1.0f : 1.0f;

        float fr = heave - cmdRoll + cmdPitch - yawSign * cmdYaw; // FR: Front right
        float al = heave + cmdRoll - cmdPitch - yawSign * cmdYaw; // AL: After left
        float fl = heave + cmdRoll + cmdPitch + yawSign * cmdYaw; // FL: Front left
        float ar = heave - cmdRoll - cmdPitch + yawSign * cmdYaw; // AR: After right
        fr = al = fl = ar = 0.6;
        fr = al = 0.6f + yawSign * 0.00005f;

        servosControl->setThrottleNorm(servoIdxFL, fl, servoIdxFR, fr, servoIdxAR, ar, servoIdxAL, al);

        log("<<AttitudeControl::call()");
    }

    void setDataLog(Writer *dataLog) {
        this->dataLog = dataLog;
    }

    void logData(const String &msg) {
        if (this->dataLog == 0) {
            return;
        }
    }
};

} // namespace a8::core
#endif

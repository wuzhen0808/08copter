#pragma once

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

class AttitudeControl : public Component {

private:
    PidControl *altitudeControl_;
    PidControl *rollControl_;
    PidControl *pitchControl_;
    PidControl *yawControl_;
    AttitudeSensor *attitudeSensor;
    ServosControl *servosControl;
    Writer *dataLog;
    int idxFR;
    int idxFL;
    int idxAR;
    int idxAL;
    // how the Front Right propeller spin, the default direction is clockwise(left hand mode NEG frame).
    // In left hand mode(NEG frame), the quad copter frame inversely have a willing to rotate
    // right hand. Which is the positive direction of angular velocity.

    bool reverseYaw = false;

public:
    AttitudeControl(ServosControl *servosControl,
                    AttitudeSensor *attitudeSensor) : Component("atc") {
        altitudeControl_ = new PidControl(1.0f, .0f, .0f);
        rollControl_ = new PidControl(.0f, 0.0f, 0.0f);
        pitchControl_ = new PidControl(.0f, .0f, .0f);
        yawControl_ = new PidControl(.0f, .0f, .0f);
        this->attitudeSensor = attitudeSensor;
        this->servosControl = servosControl;
        this->dataLog = 0;
        this->rate = 1; // hz
    }
    ~AttitudeControl() {
        delete altitudeControl_;
        delete rollControl_;
        delete pitchControl_;
        delete yawControl_;
    }

    virtual void boot(Context &context) override {
        Component::boot(context);
        idxAR = context.properties.getInt(P_fcs_servo_idx_ar, 0);
        idxFL = context.properties.getInt(P_fcs_servo_idx_fl, 1);
        idxAL = context.properties.getInt(P_fcs_servo_idx_al, 2);
        idxFR = context.properties.getInt(P_fcs_servo_idx_fr, 3);
        log(String::format("ar:%i,fl:%i,al:%i,fr:%i,", idxAR, idxFL, idxAL, idxFR));
    }
    virtual void setup(Context &context) override {
        Component::setup(context);
    }

    virtual void tick(const long tickTime) override {

        double altitude1 = attitudeSensor->getAlt();

        Vector3f aVel1 = attitudeSensor->getAngVel();
        //
        double altitude2 = 20; // Meter
        float cmdThrottle = altitudeControl_->update(altitude2, altitude1);

        Vector3f aVel2 = Vector3f(0.0, 0.5, 0.0);
        float cmdRoll = rollControl_->update(aVel2.x, aVel1.x);
        float cmdPitch = pitchControl_->update(aVel2.y, aVel1.y);
        float cmdYaw = yawControl_->update(aVel2.z, aVel1.z);

        float yawSign = this->reverseYaw ? -1.0f : 1.0f;

        float fr = cmdThrottle - cmdRoll + cmdPitch - yawSign * cmdYaw; // FR: Front right
        float al = cmdThrottle + cmdRoll - cmdPitch - yawSign * cmdYaw; // AL: After left
        float fl = cmdThrottle + cmdRoll + cmdPitch + yawSign * cmdYaw; // FL: Front left
        float ar = cmdThrottle - cmdRoll - cmdPitch + yawSign * cmdYaw; // AR: After right
        // fr = al = fl = ar = 0.6;
        // fr = al = 0.6f + yawSign * 0.00005f;
        trim(fr);
        trim(al);
        trim(fl);
        trim(ar);

        log(String::format("\tagl:%e=>%e", altitude1, altitude2)                                                                    //
            + String::format(",cmdThrottles:%i:%.2f,%i:%.2f,%i:%.2f,%i:%.2f", idxFL, fl, idxFR, fr, idxAR, ar, idxAL, al) //
        );
        servosControl->setThrottleNorm(idxFL, fl, idxFR, fr, idxAR, ar, idxAL, al);
    }

    void trim(float &throttle) {
        if (throttle > 1) {
            throttle = 1;
        } else if (throttle < -1) {
            throttle = -1;
        }
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

#pragma once

#include "a8/fc/AttitudeSensor.h"
#include "a8/fc/PidControl.h"
#include "a8/fc/ServosControl.h"
#include "a8/fc/defines.h"
#include "a8/util/Callback.h"
#include "a8/util/Component.h"
#include "a8/util/Writer.h"

#define FR_SPIN_DIRECTION LEFT_HAND

using namespace a8::util;

namespace a8::fc {

class AttitudeControl : public Component {

private:
    PidControl *altitudePid;
    PidControl *rollPid;
    PidControl *pitchPid;
    PidControl *yawPid;
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

        altitudePid = new PidControl(.0f, .0f, .0f);
        rollPid = new PidControl(.0f, 0.0f, 0.0f);
        pitchPid = new PidControl(.0f, .0f, .0f);
        yawPid = new PidControl(.0f, .0f, .0f);
        this->attitudeSensor = attitudeSensor;
        this->servosControl = servosControl;
        this->dataLog = 0;
        this->rate = 1; // hz
    }
    ~AttitudeControl() {
        delete altitudePid;
        delete rollPid;
        delete pitchPid;
        delete yawPid;
    }

    virtual void boot(Context *context) override {
        Component::boot(context);
        idxAR = context->properties->getInt(P_fcs_servo_idx_ar, 0);
        idxFL = context->properties->getInt(P_fcs_servo_idx_fl, 1);
        idxAL = context->properties->getInt(P_fcs_servo_idx_al, 2);
        idxFR = context->properties->getInt(P_fcs_servo_idx_fr, 3);
        //
        Buffer<float> aPid3 = context->properties->getFloatArray(P_fcs_altitude_pid_k3);
        Buffer<float> rPid3 = context->properties->getFloatArray(P_fcs_roll_pid_k3);
        Buffer<float> pPid3 = context->properties->getFloatArray(P_fcs_pitch_pid_k3);
        Buffer<float> yPid3 = context->properties->getFloatArray(P_fcs_yaw_pid_k3);
        setPidKs(altitudePid, aPid3);
        setPidKs(rollPid, rPid3);
        setPidKs(pitchPid, pPid3);
        setPidKs(yawPid, yPid3);

        log(String::format("ar:%i,fl:%i,al:%i,fr:%i,", idxAR, idxFL, idxAL, idxFR));
        log(String("pid-Ks:")
            << "altitude:" << altitudePid->toString() << "," //
            << "roll:" << rollPid->toString() << ","         //
            << "pitch:" << pitchPid->toString() << ","       //
            << "yaw:" << yawPid->toString() << ","           //
        );
    }

    void setPidKs(PidControl *pid, Buffer<float> pidKs) {

        float kp = 0.0f;
        float ki = 0.0f;
        float kd = 0.0f;

        if (pidKs.getLength() > 0) {
            kp = pidKs.get(0);
        }
        if (pidKs.getLength() > 1) {
            ki = pidKs.get(1);
        }
        if (pidKs.getLength() > 2) {
            kd = pidKs.get(2);
        }
        pid->setup(kp, ki, kd);
    }
    virtual void setup(Context *context) override {
        Component::setup(context);
    }

    virtual void tick(const long tickTime) override {

        double altitude1 = attitudeSensor->getAlt();

        Vector3f aVel1 = attitudeSensor->getAngVel();
        //
        double altitude2 = 200; // Meter
        float cmdThrottle = altitudePid->update(altitude2, altitude1);

        Vector3f aVel2 = Vector3f(0.0, 0.0, 0.0);
        float cmdRoll = rollPid->update(aVel2.x, aVel1.x);
        float cmdPitch = pitchPid->update(aVel2.y, aVel1.y);
        float cmdYaw = yawPid->update(aVel2.z, aVel1.z);

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

        log(String::format("\tagl:%e=>%e", altitude1, altitude2)                                                          //
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

} // namespace a8::fc
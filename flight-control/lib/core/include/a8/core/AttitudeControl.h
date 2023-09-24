#ifndef ATTITUDE_CONTROL__
#define ATTITUDE_CONTROL__
#include "a8/core/AttitudeSensor.h"
#include "a8/core/PidControl.h"
#include "a8/core/ServosControl.h"
#include "a8/util/Callback.h"
#include "a8/util/Component.h"
#include "a8/util/Writer.h"

//TODO configurable
#define SERVO_AFTER_RIGHT 0
#define SERVO_FRONT_LEFT 1
#define SERVO_AFTER_LEFT 2
#define SERVO_FRONT_RIGHT 3

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

public:
    AttitudeControl(void *context,
                    ServosControl *servosControl,
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
    virtual void call() {
        // string msg;

        log(">>AttitudeControl::call()");

        Vector3f sensorAngVelDegSec = attitudeSensor->getAngVel();
        Vector3f desireAngVelDegSec = Vector3f(0.0, 0.5, 0.0);

        float cmdRoll = rollControl->update(desireAngVelDegSec.a, sensorAngVelDegSec.a);
        float cmdPitch = pitchControl->update(desireAngVelDegSec.b, sensorAngVelDegSec.b);
        float cmdYaw = yawControl->update(desireAngVelDegSec.c, sensorAngVelDegSec.c);

        // log(string("actualRoll:") + string(actualRoll) + string(",rollGain:") + string(rollGain));

        float heave = .0f;

        // float m2 = throttle + cmdRoll - cmdPitch + cmdYaw; // FR: Front right
        // float m4 = throttle - cmdRoll + cmdPitch + cmdYaw; // AL: After left
        // float m1 = throttle - cmdRoll - cmdPitch - cmdYaw; // FL: Front left
        // float m3 = throttle + cmdRoll + cmdPitch - cmdYaw; // AR: After right

        float fr = heave - cmdRoll + cmdPitch + cmdYaw; // FR: Front right
        float al = heave + cmdRoll - cmdPitch + cmdYaw; // AL: After left
        float fl = heave + cmdRoll + cmdPitch - cmdYaw; // FL: Front left
        float ar = heave - cmdRoll - cmdPitch - cmdYaw; // AR: After right
        
        servosControl->setThrottleNorm(SERVO_FRONT_LEFT, fl, SERVO_FRONT_RIGHT, fr, SERVO_AFTER_RIGHT, ar, SERVO_AFTER_LEFT, al);
        log("<<AttitudeControl::call()");
    }
    void setDataLog(Writer *dataLog) {
        this->dataLog = dataLog;
    }

    void logData(const String& msg){
        if(this->dataLog ==0){
            return;
        }

    }
};

} // namespace a8::core
#endif

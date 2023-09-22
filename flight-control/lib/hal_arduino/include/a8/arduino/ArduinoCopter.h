#pragma once
#include "a8/core/Copter.h"
#include "a8/util/Scheduler.h"
#include "a8/core/ServosControl.h"
#include "a8/core/AttitudeSensor.h"

using a8::core::Copter;
using a8::core::Scheduler;
using a8::core::AttitudeControl;
using a8::core::ServosControl;
using a8::core::AttitudeSensor;

namespace a8::arduino {

class ArduinoCopter : public Copter {
public:
    ArduinoCopter(Scheduler* scheduler);
    ~ArduinoCopter();
    virtual void start();
    virtual void stop();
    virtual int getServoAttachPin(int servoId);
    virtual Scheduler *getScheduler();
    virtual ServosControl *newServosControl();
    virtual AttitudeSensor *newAttitudeSensor();
    virtual AttitudeControl *newAttitudeControl(ServosControl* sc, AttitudeSensor* as);

private:
    int *servoAttachPins;    
};

} // namespace a8::arduino

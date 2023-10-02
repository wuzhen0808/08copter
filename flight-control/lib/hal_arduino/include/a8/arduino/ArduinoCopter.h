#pragma once
#include "a8/fc/FlightControl.h"
#include "a8/util/Scheduler.h"
#include "a8/fc/ServosControl.h"
#include "a8/fc/AttitudeSensor.h"

using a8::fc::Copter;
using a8::fc::Scheduler;
using a8::fc::AttitudeControl;
using a8::fc::ServosControl;
using a8::fc::AttitudeSensor;

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

#pragma once
#include "a8/fcs/FlightControl.h"
#include "a8/util/Scheduler.h"
#include "a8/fcs/ServosControl.h"
#include "a8/fcs/AttitudeSensor.h"

using a8::fcs::Copter;
using a8::fcs::Scheduler;
using a8::fcs::AttitudeControl;
using a8::fcs::ServosControl;
using a8::fcs::AttitudeSensor;

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

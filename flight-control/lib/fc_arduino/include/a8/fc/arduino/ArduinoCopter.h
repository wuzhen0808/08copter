#pragma once
#include "a8/fc.h"
#include "a8/util/thread.h"

using namespace a8::fc;

namespace a8::fc::arduino {

class ArduinoCopter : public FlightControl {
public:
    ArduinoCopter(Scheduler *scheduler);
    ~ArduinoCopter();
    virtual void start();
    virtual void stop();
    virtual int getServoAttachPin(int servoId);
    virtual Scheduler *getScheduler();
    virtual ServosControl *newServosControl();
    virtual AttitudeSensor *newAttitudeSensor();
    virtual AttitudeControl *newAttitudeControl(ServosControl *sc, AttitudeSensor *as);

private:
    int *servoAttachPins;
};

} // namespace a8::fc::arduino

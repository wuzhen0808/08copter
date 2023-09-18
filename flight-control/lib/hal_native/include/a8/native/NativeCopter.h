
#pragma once

#include "a8/core/Copter.h"
#include <iostream>

using namespace std;
using namespace a8::util;
using namespace a8::core;
namespace a8::native {

class NativeCopter : public Copter {
protected:
    virtual ServosControl *newServosControl(int totalServos, int *servoAttachPins);
    virtual AttitudeSensor *newAttitudeSensor();
    virtual AttitudeControl *newAttitudeControl(ServosControl *sc, AttitudeSensor *as);

public:
    NativeCopter(Scheduler *scheduler);
    ~NativeCopter();
    //virtual void setup();
    virtual void start();
    virtual void stop();
    virtual int getServoAttachPin(int servoId);
};

} // namespace a8::native

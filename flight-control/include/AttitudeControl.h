#ifndef ATTITUDE_CONTROL__
#define ATTITUDE_CONTROL__
#include "PidControl.h"
#include "Runnable.h"
#include "AttitudeSensor.h"
#include "Copter.h"
#include "Component.h"
#include "ServosControl.h"

class AttitudeControl : public Runnable, public Component {
public:
    AttitudeControl(Copter* copter);
    ~AttitudeControl(); 
    virtual void run();
private:
    PidControl* rollControl;
    PidControl* pitchControl;
    PidControl* yawControl;
    AttitudeSensor* attitudeSensor;
    ServosControl* servosControl;

};
#endif
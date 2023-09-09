#ifndef ATTITUDE_CONTROL__
#define ATTITUDE_CONTROL__
#include "a8/AttitudeSensor.hpp"
#include "a8/Component.hpp"
#include "a8/Copter.hpp"
#include "a8/PidControl.hpp"
#include "a8/Runnable.hpp"
#include "a8/ServosControl.hpp"
namespace a8 {

class AttitudeControl : public Runnable, public Component {
public:
    AttitudeControl(Copter *copter,
                    ServosControl *servosControl,
                    AttitudeSensor *attitudeSensor);
    ~AttitudeControl();
    virtual void run();

private:
    PidControl *rollControl;
    PidControl *pitchControl;
    PidControl *yawControl;
    AttitudeSensor *attitudeSensor;
    ServosControl *servosControl;
};
} // namespace a8
#endif

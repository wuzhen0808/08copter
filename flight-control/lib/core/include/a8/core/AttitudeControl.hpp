#ifndef ATTITUDE_CONTROL__
#define ATTITUDE_CONTROL__
#include "a8/core/AttitudeSensor.hpp"
#include "a8/core/Component.hpp"
#include "a8/core/Copter.hpp"
#include "a8/core/PidControl.hpp"
#include "a8/rtos/Runnable.hpp"
#include "a8/core/ServosControl.hpp"
namespace a8 {
namespace  core {
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
}
} // namespace a8
#endif
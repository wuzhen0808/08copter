#ifndef ATTITUDE_CONTROL__
#define ATTITUDE_CONTROL__
#include "a8/core/AttitudeSensor.h"
#include "a8/core/Component.h"
#include "a8/core/Copter.h"
#include "a8/core/PidControl.h"
#include "a8/core/Callback.h"
#include "a8/core/ServosControl.h"
namespace a8 {
namespace  core {
class AttitudeControl : public Callback, public Component {
public:
    AttitudeControl(Copter *copter,
                    ServosControl *servosControl,
                    AttitudeSensor *attitudeSensor);
    ~AttitudeControl();
    virtual void call();

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

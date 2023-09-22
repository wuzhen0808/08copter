#ifndef ATTITUDE_CONTROL__
#define ATTITUDE_CONTROL__
#include "a8/core/AttitudeSensor.h"
#include "a8/core/PidControl.h"
#include "a8/core/ServosControl.h"
#include "a8/util/Callback.h"
#include "a8/util/Component.h"
using namespace a8::util;

namespace a8::core {
class AttitudeControl : public Callback, public Component {
public:
    AttitudeControl(void *copter,
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

} // namespace a8::core
#endif

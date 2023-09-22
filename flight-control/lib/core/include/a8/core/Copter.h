#ifndef FLIGHT_CONFIG__
#define FLIGHT_CONFIG__

#include "a8/core/AttitudeControl.h"
#include "a8/core/AttitudeSensor.h"
#include "a8/core/ServosControl.h"
#include "a8/hal/Hal.h"
#include "a8/util/Result.h"
#include "a8/util/Scheduler.h"
#include "a8/util/String.h"

#define SERVO_FRONT_LEFT 0
#define SERVO_FRONT_RIGHT 1
#define SERVO_AFTER_RIGHT 2
#define SERVO_AFTER_LEFT 3

using a8::hal::S;
using namespace a8::util;
namespace a8::core {

class Copter : public Component {
private:
protected:
    // members
    int totalServos_;
    AttitudeSensor *attitudeSensor_;
    ServosControl *servosControl_;
    AttitudeControl *attitudeControl_;
    Timer *attitudeTimer_;
    // member functions
    virtual ServosControl *setupServosControl(Context * context) = 0;
    virtual AttitudeSensor *setupAttitudeSensor(Context * context) = 0;

public:
    Copter();
    Copter(int totalServos);
    ~Copter();
    virtual void setup(Context * context);
    virtual void start(Context * context);
    virtual void stop();
    int getServoCount();
    void log(a8::util::String message);
};

} // namespace a8::core
#endif
#ifndef FLIGHT_CONFIG__
#define FLIGHT_CONFIG__

#include "a8/util/String.h"
#include "a8/util/Result.h"
#include "a8/core/AttitudeControl.h"
#include "a8/core/AttitudeSensor.h"
#include "a8/core/Scheduler.h"
#include "a8/core/ServosControl.h"
#include "a8/hal/Hal.h"

#define SERVO_FRONT_LEFT 0
#define SERVO_FRONT_RIGHT 1
#define SERVO_AFTER_RIGHT 2
#define SERVO_AFTER_LEFT 3

using a8::hal::S;

namespace a8 {
namespace core {
using a8::util::String;
using a8::util::Result;

class Copter {
private:
protected:
    // members
    int totalServos_;
    Scheduler *scheduler_;
    AttitudeSensor *attitudeSensor_;
    ServosControl *servosControl_;
    AttitudeControl *attitudeControl_;
    Timer *attitudeTimer_;
    // member functions
    virtual ServosControl *setupServosControl() = 0;
    virtual AttitudeSensor *setupAttitudeSensor() = 0;
    AttitudeControl *setupAttitudeControl(ServosControl *sc, AttitudeSensor *as);

public:
    Copter(Scheduler *scheduler);
    Copter(int totalServos, Scheduler *scheduler);
    ~Copter();
    virtual Result setup();
    virtual Result start();
    virtual void stop();
    int getServoCount();
    void log(a8::util::String message);
    virtual Scheduler *getScheduler();
};
} // namespace core
} // namespace a8
#endif
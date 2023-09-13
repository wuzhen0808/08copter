#ifndef FLIGHT_CONFIG__
#define FLIGHT_CONFIG__
#include "a8/core/AttitudeControl.h"
#include "a8/core/AttitudeSensor.h"
#include "a8/core/Scheduler.h"
#include "a8/core/ServosControl.h"
#include "a8/core/AttitudeSensor.h"
#include "a8/hal/Hal.h"
#include "a8/util/String.h"

#define SERVO_TOP_LEFT 0
#define SERVO_TOP_RIGHT 1
#define SERVO_BOTTOM_RIGHT 2
#define SERVO_BOTTOM_LEFT 3

using a8::hal::S;

namespace a8 {
namespace core {
using a8::util::String;
class Copter {
private:
    int servoCount;

protected:
    Scheduler *scheduler;
    AttitudeSensor * attitudeSensor;
    ServosControl * servosControl;
    AttitudeControl * attitudeControl;
    Timer * attitudeTimer;
public:
    Copter(int servoCount, Scheduler* scheduler);
    ~Copter();
    virtual void setup();
    virtual void start();
    virtual void stop() ;
    int getServoCount();
    virtual int getServoAttachPin(int servoId) = 0;
    void log(a8::util::String message);
    virtual Scheduler *getScheduler() ;

protected:
    virtual ServosControl *newServosControl() = 0;
    virtual AttitudeSensor *newAttitudeSensor() = 0;
    virtual AttitudeControl *newAttitudeControl(ServosControl *sc, AttitudeSensor *as) = 0;
};
} // namespace core
} // namespace a8
#endif
#ifndef FLIGHT_CONFIG__
#define FLIGHT_CONFIG__
#include "a8/core/AttitudeControl.h"
#include "a8/core/AttitudeSensor.h"
#include "a8/core/Scheduler.h"
#include "a8/core/ServosControl.h"
#include "a8/hal/Hal.h"
#include "a8/util/String.h"

#define SERVO_FRONT_LEFT 0
#define SERVO_FRONT_RIGHT 1
#define SERVO_AFTER_RIGHT 2
#define SERVO_AFTER_LEFT 3

using a8::hal::S;

namespace a8 {
namespace core {
using a8::util::String;
class Copter {
private:
protected:
    // members
    int totalServos_;
    int *servoAttachPins_;
    Scheduler *scheduler_;
    AttitudeSensor *attitudeSensor_;
    ServosControl *servosControl_;
    AttitudeControl *attitudeControl_;
    Timer *attitudeTimer_;
    // member functions
    virtual ServosControl *newServosControl(int totalServos, int *servoAttachPins) = 0;
    virtual AttitudeSensor *newAttitudeSensor() = 0;
    AttitudeControl *newAttitudeControl(ServosControl *sc, AttitudeSensor *as);
    void configServos(int totalServos, int *servoAttachPins);

public:
    Copter(Scheduler *scheduler);
    ~Copter();
    virtual void setup();
    virtual void start();
    virtual void stop();
    int getServoCount();
    virtual int getServoAttachPin(int servoId) = 0;
    void log(a8::util::String message);
    virtual Scheduler *getScheduler();
};
} // namespace core
} // namespace a8
#endif
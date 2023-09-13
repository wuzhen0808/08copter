#ifndef FLIGHT_CONFIG__
#define FLIGHT_CONFIG__
#include "a8/core/Scheduler.h"
#include "a8/hal/Hal.h"
#include "a8/util/String.h"

#define SERVO_TOP_LEFT 0
#define SERVO_TOP_RIGHT 1
#define SERVO_BOTTOM_RIGHT 2
#define SERVO_BOTTOM_LEFT 3

using a8::hal::S;

using namespace a8::util;
namespace a8 {
namespace core {
class Copter {
public:
    Copter(int servoCount);
    ~Copter();
    virtual void start() = 0;
    virtual void stop() = 0;
    int getServoCount();
    virtual int getServoAttachPin(int servoId) = 0;
    void log(String message) ;
    virtual Scheduler *getScheduler() = 0;

private:
    int servoCount;
};
} // namespace core
} // namespace a8
#endif
#ifndef FLIGHT_CONFIG__
#define FLIGHT_CONFIG__

#include "a8/core/Scheduler.hpp"
#include "a8/util/String.hpp"

#define SERVO_TOP_LEFT 0
#define SERVO_TOP_RIGHT 1
#define SERVO_BOTTOM_RIGHT 2
#define SERVO_BOTTOM_LEFT 3

using namespace a8::util;
namespace a8 {
namespace core {
class Copter {
public:
    Copter(int servoCount, Scheduler* scheduler);
    ~Copter();    
    virtual void start();
    virtual void stop();
    int getServoCount();
    int getServoAttachPin(int servoId);    
    virtual void log(String message);
    Scheduler *getScheduler();

private:
    int servoCount ;    
    Scheduler *scheduler;
};
} // namespace core
} // namespace a8
#endif
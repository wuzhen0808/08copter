#ifndef FLIGHT_CONFIG__
#define FLIGHT_CONFIG__

#include "a8/core/Scheduler.hpp"
#include "a8/util/string.hpp"

#define SERVO_TOP_LEFT 0
#define SERVO_TOP_RIGHT 1
#define SERVO_BOTTOM_RIGHT 2
#define SERVO_BOTTOM_LEFT 3

namespace a8 {
namespace core {
using namespace a8::util;
class Copter {
public:
    Copter(Scheduler *scheduler);
    ~Copter();
    int getServoCount();
    int getServoAttachPin(int servoId);
    void destroy();
    void log(char* message);
    void log(string message);
    Scheduler *getScheduler();

private:
    int servoCount = 4;
    int *servoAttachPins = new int[4]{3, 5, 9, 11};
    Scheduler *scheduler;
};
} // namespace core
} // namespace a8
#endif
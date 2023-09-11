#ifndef ARDUINO_SCHEDULER_H__
#define ARDUINO_SCHEDULER_H__
#include "a8/core/Scheduler.hpp"
namespace a8 {
namespace ardui {
using namespace a8::core;

class ArduinoScheduler : public Scheduler {
public:
    ArduinoScheduler();
    ~ArduinoScheduler();
    virtual void startSchedule();

    virtual void endSchedule();

    virtual Thread *schedule(Runnable *runnable);

    virtual Timer *scheduleTimer(Callback *callback, ulong ticks);

    virtual Thread *getCurrentThread();
};
} // namespace ardui
} // namespace a8
#endif
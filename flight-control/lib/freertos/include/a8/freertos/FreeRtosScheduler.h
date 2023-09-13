#ifndef ARDUINO_SCHEDULER_H__
#define ARDUINO_SCHEDULER_H__
#include "a8/core/Scheduler.h"
namespace a8::freertos {
using namespace a8::core;

class FreeRtosScheduler : public Scheduler {
public:
    FreeRtosScheduler();
    ~FreeRtosScheduler();
    virtual void startSchedule();

    virtual void endSchedule();

    virtual Thread *schedule(Runnable *runnable);

    virtual Timer *scheduleTimer(Callback *callback, ulong ticks);

    virtual Thread *getCurrentThread();
};

} // namespace a8::arduino
#endif
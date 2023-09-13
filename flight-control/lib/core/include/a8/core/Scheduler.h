#ifndef SCHEDULER__
#define SCHEDULER__
#include "a8/core/Runnable.h"
#include "a8/core/Thread.h"
#include "a8/core/Timer.h"
#include "a8/core/core.h"

namespace a8 {
namespace core {

class Scheduler {
public:
    Scheduler();
    ~Scheduler();
    virtual void startSchedule() = 0;
    virtual void endSchedule() = 0;
    virtual Thread *schedule(Runnable *runnable) = 0;

    virtual Timer *scheduleTimer(Callback *callback, ulong ticks) = 0;

    virtual Thread *getCurrentThread() = 0;
};
} // namespace core
} // namespace a8
#endif
#ifndef SCHEDULER__
#define SCHEDULER__
#include "a8/core/Runnable.hpp"
#include "a8/core/Thread.hpp"
#include "a8/core/Timer.hpp"
#include "a8/core/core.hpp"

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
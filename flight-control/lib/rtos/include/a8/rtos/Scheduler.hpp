#ifndef SCHEDULER__
#define SCHEDULER__
#include "a8/rtos/RtOs.hpp"
#include "a8/rtos/Thread.hpp"
#include "a8/rtos/Timer.hpp"

namespace a8 {
namespace rtos {

class Scheduler {
public:
    Scheduler();

    void startSchedule();

    Thread *schedule(Runnable *runnable);

    Timer *scheduleTimer(Callback *callback, ulong ticks);

    Thread *getCurrentThread();
};
} // namespace rtos
} // namespace a8
#endif
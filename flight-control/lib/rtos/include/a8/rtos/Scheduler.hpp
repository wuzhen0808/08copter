#ifndef SCHEDULER__
#define SCHEDULER__
#include "a8/rtos/Thread.hpp"
namespace a8 {
namespace rtos {

class Scheduler {
public:
    Scheduler();

    void startSchedule();

    Thread *schedule(Runnable *runnable);

    Thread *getCurrentThread();
};
} // namespace rtos
} // namespace a8
#endif
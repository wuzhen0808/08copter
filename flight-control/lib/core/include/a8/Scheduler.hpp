#ifndef SCHEDULER__
#define SCHEDULER__
#include "a8/Thread.hpp"
namespace a8 {
class Scheduler {
public:
    Scheduler();

    void startSchedule();

    Thread* schedule(Runnable * runnable);

    Thread* getCurrentThread();

};
}
#endif
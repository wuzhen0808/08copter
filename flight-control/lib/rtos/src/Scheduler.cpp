#include "a8/rtos/Scheduler.hpp"

namespace a8 {
namespace rtos {
Scheduler::Scheduler() {
}
void Scheduler::startSchedule() {

    Thread::startScheduler();
}

Thread *Scheduler::schedule(Runnable *runnable) {
    return Thread::start(runnable);
}

Thread *Scheduler::getCurrentThread() {
    return Thread::getCurrentThread();
}
} // namespace rtos
} // namespace a8
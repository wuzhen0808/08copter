#include "a8/Scheduler.hpp"

namespace a8 {
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
} // namespace a8
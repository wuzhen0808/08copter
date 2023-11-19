#pragma once
#include "a8/util/System.h"
#include "a8/util/sched/Scheduler.h"

namespace a8::util::sched {
using namespace a8::util;

class TaskSystem : public System, public Scheduler, public LoggerFactory {

};

} // namespace a8::util::sched
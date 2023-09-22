#pragma once
#include "a8/util/Scheduler.h"

using namespace a8::util;

namespace a8::freertos {

class FreeRtosScheduler : public Scheduler {
public:
    FreeRtosScheduler();
    ~FreeRtosScheduler();
    virtual Timer* tmpTimer() override {
        return 0;
    }
    virtual void startSchedule() override;

    virtual void endSchedule() override;

    virtual Thread *schedule(Runnable *runnable) override;

    virtual Timer *scheduleTimer(Callback *callback, long ticks) override;

    virtual Thread *getCurrentThread() override;
};

} // namespace a8::freertos
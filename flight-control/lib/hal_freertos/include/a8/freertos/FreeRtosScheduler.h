#pragma once
#include "a8/core/Scheduler.h"
namespace a8::freertos {
using namespace a8::core;

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
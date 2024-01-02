#pragma once
#include "a8/fc/Rpy.h"
#include "a8/util/sched.h"
namespace a8::fc {
using namespace a8::util::sched;
class ImuMonitor {
    Imu *imu;
    Output *out;
    Scheduler *sch;
    Timer *timer;
    Rate rate = 2.0f;

public:
    ImuMonitor(Imu *imu, Output *out, Scheduler *sch) {
        this->imu = imu;
        this->out = out;
        this->sch = sch;
        this->timer = 0;
    }
    Rate getRate() {
        return this->rate;
    }
    void setRate(Rate rate) {
        this->rate = rate;
    }

    bool isRunning() {
        return this->timer != 0;
    }

    void open() {
        if (this->timer != 0) {
            return;
        }
        this->timer = this->sch->createTimer<ImuMonitor *>("RpyMonitor", rate, this, [](ImuMonitor *this_) {
            this_->tick();
        });
    }

    int close() {
        if (this->timer == 0) {
            return 1;
        }
        int ret = this->timer->close();
        if (ret < 0) {
            return ret;
        }
        this->timer = 0;
        return 1;
    }

    void tick() {        
        bool ok = imu->update();
        if (!ok) {
            this->out->println("update failed.");
            return;
        }
        this->out->println(String("(") << imu->getRoll() << "," << imu->getPitch() << "," << imu->getYaw() << ")");
    }
};
} // namespace a8::fc
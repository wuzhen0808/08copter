#pragma once
#include "a8/fc.h"
#include "a8/fc/esp/EspExecutor.h"
#include "a8/hal/esp.h"
#include "a8/util.h"
#include "a8/util/sched.h"
#include <MPU9250.h>
namespace a8::fc::esp {
using namespace a8::util;
using namespace a8::hal::esp;
class EspApplication : public BaseEspApplication {
protected:
    Executor *executor;
    MPU9250 *mpu;
    PowerManage *pm;

public:
    EspApplication(MPU9250 *mpu) : BaseEspApplication("EspApplication") {
        this->mpu = mpu;
        this->pm = new PowerManage(sys, 5, 21, loggerFactory);
        this->executor = new EspExecutor(pm, mpu, sys, loggerFactory);
    }

    ~EspApplication() {
    }

    int setupMpu9250() {
        mpu->setup(0x68);
        mpu->selectFilter(QuatFilterSel::MADGWICK);
        mpu->setFilterIterations(1);
        log("success of setupMpu9250.");
        return 1;
    }
    int start(Result &res) override {
        this->setupSerial();
        this->setupWire();
        this->setupMpu9250();
        this->pm->setup();
        sch->createTimer<EspApplication *>("100Hz", 100.0f, this, [](EspApplication *this_) {
            this_->hz100();
        });
        sch->createTask<EspApplication *>("EspApplication", this, [](EspApplication *this_) {
            this_->run();
        });
        return 1;
    }
    void hz100() {
        long timeMs = sys->getSteadyTime();
        this->pm->tick(timeMs);
    }

    void run() {
        Result res;
        int ret = this->executor->run(res);
        if (ret < 0) {
            log(res.errorMessage);
        }
        log("done of application.");
    }
};

} // namespace a8::fc::esp

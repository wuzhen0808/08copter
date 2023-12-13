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
class EspApplication : public BaseEspExample {
protected:
    Executor *executor;
    MPU9250 *mpu;

public:
    EspApplication(MPU9250 *mpu) : BaseEspExample("EspApplication") {
        this->mpu = mpu;
        this->executor = new EspExecutor(mpu, sys, loggerFactory);
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

        sch->createTask<EspApplication *>("EspPilotExample", this, [](EspApplication *this_) {
            this_->run();
        });
        return 1;
    }
    void run() {
        this->executor->run();
    }
};

} // namespace a8::fc::esp

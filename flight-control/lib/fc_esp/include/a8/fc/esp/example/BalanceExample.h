#pragma once
#include "a8/fc.h"
#include "a8/fc/esp.h"
#include "a8/hal/esp/example.h"
#include "a8/util.h"
#include "a8/util/sched.h"

#include <Wire.h>
namespace a8::fc::esp::example {
using namespace a8::util;
using namespace a8::util::sched;
using namespace a8::fc;
using namespace a8::hal::esp::example;

class EspBalanceExample : public BaseEspExample {
    Balancer *balancer;
    bool setup = false;
    MPU9250 *mpu;

public:
    EspBalanceExample(MPU9250 *mpu) : BaseEspExample("BalanceExample") {
        this->mpu = mpu;
        Propeller *p1 = new EspPropeller();
        Propeller *p2 = new EspPropeller();
        Propeller *p3 = new EspPropeller();
        Propeller *p4 = new EspPropeller();

        this->balancer = new EspBalancer(p1, p2, p3, p4, mpu, sys, loggerFactory);
    }

    ~EspBalanceExample() {
    }

    Balancer *getBalancer() {
        return this->balancer;
    }
    bool update() {
        Result res;
        return this->balancer->update(res) > 0;
    }
    float getRoll() {
        return this->balancer->getRoll();
    }

    float getPitch() {
        return this->balancer->getPitch();
    }
    float getYaw() {
        return this->balancer->getYaw();
    }
    int setupMpu9250() {
        mpu->setup(0x68);
        mpu->selectFilter(QuatFilterSel::MADGWICK);
        mpu->setFilterIterations(1);
        study::log("success of setupMpu9250.", true);
        return 1;
    }
    int start(Result &res) override {
        this->setupSerial();
        this->setupWire();
        this->setupMpu9250();
        this->balancer->start();
        sch->createTimer<EspBalanceExample *>("EspBalanceExample", 100.0f, this, [](EspBalanceExample *this_) {
            this_->tick();
        });
        return 1;
    }
    void tick() {
        Result res;
        int ret = this->balancer->update(res);
        if (ret < 0) {
            log(String() << "failed to update balancer, detail:" << res);
        }
    }
};
} // namespace a8::fc::esp::example
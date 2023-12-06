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

class EspPilotExample : public BaseEspExample {
    Pilot *pilot;
    bool setup = false;
    MPU9250 *mpu;

public:
    EspPilotExample(MPU9250 *mpu) : BaseEspExample("EspPilotExample") {
        this->mpu = mpu;
        Propeller *p0 = setupPropeller(17, 50);
        Propeller *p1 = setupPropeller(18, 50);
        Propeller *p2 = setupPropeller(19, 100);
        Propeller *p3 = setupPropeller(20, 100);

        this->pilot = new EspPilot(p0, p1, p2, p3, mpu, sys, loggerFactory);
    }

    Propeller *setupPropeller(int pin, int hz) {
        EspPropeller *prop = new EspPropeller();
        prop->hz(hz);
        int channel = prop->attach(pin);
        prop->setup();
        log(String() << "propeller setup,pin:" << pin << ",hz:" << hz << ",channel:" << channel);
        return prop;
    }

    ~EspPilotExample() {
    }

    Pilot *getPilot() {
        return this->pilot;
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
        this->pilot->start();
        
        sch->createTask<EspPilotExample *>("EspPilotExample", this, [](EspPilotExample *this_) {
            this_->run();
        });
        return 1;
    }
    void run() {
        while (true) {
            Result res;
            int ret = this->pilot->update(res);
            if (ret < 0) {
                log(String() << "failed to update pilot, detail:" << res);
            }
        }
    }    
};
} // namespace a8::fc::esp::example
#pragma once
#include "a8/fc.h"
#include "a8/fc/Propeller.h"
#include "a8/fc/esp/EspPropeller.h"
namespace a8::fc::esp {

class EspPropellers : public Propellers {
    PwmManage *pwmManage;

public:
    EspPropellers(PowerManage *pm, int pinLH, int pinRH, int pinLA, int pinRA, LoggerFactory *logFac) : Propellers(logFac) {
        this->pwmManage = new PwmManage(pm, logFac);
        addPropeller("LH", pinLH, 50);
        addPropeller("RH", pinRH, 50);
        addPropeller("LA", pinLA, 100);
        addPropeller("RA", pinRA, 100);
    }
    ~EspPropellers() {
        this->propellers.clear([](Propeller *prop) {
            delete prop;
        });
        delete this->pwmManage;
    }

    void setup() override {
        this->pwmManage->setup();
    }
    int isReady(Result &res) override {
        int ret = Propellers::isReady(res);
        if (ret < 0) {
            return ret;
        }
        return this->pwmManage->isReady(res);
    }

    void addPropeller(String name, int pin, int hz) {
        EspPropeller *prop = new EspPropeller(name, pwmManage);
        prop->hz(hz);
        int channel = prop->attach(pin);
        prop->setup();
        log(String() << "propeller setup,pin:" << pin << ",hz:" << hz << ",channel:" << channel);
        this->propellers.append(prop);
    }
};
} // namespace a8::fc::esp
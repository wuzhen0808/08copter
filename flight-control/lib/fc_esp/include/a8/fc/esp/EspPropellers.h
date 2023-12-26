#pragma once
#include "a8/fc.h"
#include "a8/fc/Propeller.h"
#include "a8/fc/esp/EspPropeller.h"
namespace a8::fc::esp {

class EspPropellers : public Propellers {

    int pins[4];

public:
    EspPropellers(PowerManage *pm, int pinLH, int pinRH, int pinLA, int pinRA, LoggerFactory *logFac) : Propellers(pm, logFac) {
        this->pins[0] = pinLH;
        this->pins[1] = pinRH;
        this->pins[2] = pinLA;
        this->pins[3] = pinRA;
    }
    ~EspPropellers() {
        this->propellers.clear([](Propeller *prop) {
            delete prop;
        });
    }

    void setup() override {
        Propellers::setup();
        addPropeller("LH", pins[0], 50);
        addPropeller("RH", pins[1], 50);
        addPropeller("LA", pins[2], 100);
        addPropeller("RA", pins[3], 100);
    }
    
    void addPropeller(String name, int pin, int hz) {

        EspPropeller *prop = new EspPropeller(name, this->propellers.len());
        prop->hz(hz);
        int channel = prop->attach(pin);
        prop->setup();
        log(String() << "propeller setup,pin:" << pin << ",hz:" << hz << ",channel:" << channel);
        this->propellers.append(prop);
    }
};
} // namespace a8::fc::esp
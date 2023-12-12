#pragma once
#include "a8/fc/Propeller.h"
#include "a8/util/Result.h"
#include "a8/util/comp.h"
#define A8_PROPELLER_COUNT (4)
#define A8_PROPELLER_LH = 0;
#define A8_PROPELLER_RH = 1;
#define A8_PROPELLER_LA = 2;
#define A8_PROPELLER_RA = 3;

namespace a8::fc {
using namespace a8::util;
using namespace a8::util::comp;

class Propellers : public FlyWeight {
protected:
    Buffer<Propeller *> propellers;

public:
    Propellers(LoggerFactory *logFac) : FlyWeight(logFac, "Propellers") {
    }

    Propeller *get(int idx) {
        return propellers.get(idx);
    }
    void startUpdate() {
        propellers.forEach<int>(0, [](int c, Propeller *propeller) {
            propeller->startUpdate();
        });
    }
    void commitUpdate() {
        propellers.forEach<int>(0, [](int c, Propeller *propeller) {
            propeller->commitUpdate();
        });
    }

    void start() {
        propellers.forEach<int>(0, [](int c, Propeller *propeller) {
            propeller->start();
        });
    }

    void addPwm(long pwmLH, long pwmRH, long pwmLA, long pwmRA) {
        propellers[0]->addPwm(pwmLH);
        propellers[1]->addPwm(pwmRH);
        propellers[2]->addPwm(pwmLA);
        propellers[3]->addPwm(pwmRA);
    }

    void setPwm(long pwmLH, long pwmRH, long pwmLA, long pwmRA) {
        propellers[0]->setPwm(pwmLH);
        propellers[1]->setPwm(pwmRH);
        propellers[2]->setPwm(pwmLA);
        propellers[3]->setPwm(pwmRA);
    }

    long getPwm(int idx) {
        return propellers[idx]->getPwm();
    }

    void addPwm(long pwm) {
        propellers.forEach<long>(pwm, [](long pwm, Propeller *propeller) {
            propeller->addPwm(pwm);
        });
    }
    void printHistory(String &msg) {

        propellers.forEach<String &>(msg, [](String &msg, Propeller *propeller) {
            msg << "propeller[" << propeller->getName() << "]:";
            propeller->printHistory("\n ", msg);
            msg << "\n";
        });
    }

    void enableAll(bool enable) {
        propellers.forEach<bool>(enable, [](bool enable, Propeller *propeller) {
            propeller->enable(enable);
        });
    }

    int getTotalPropellers() {
        return this->propellers.len();
    }

    Buffer<Propeller *> getAll() {
        return this->propellers;
    }
};

} // namespace a8::fc

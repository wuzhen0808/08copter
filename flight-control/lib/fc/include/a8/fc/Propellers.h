#pragma once
#include "a8/fc/Propeller.h"
#include "a8/fc/collect/Collector.h"
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
using namespace a8::fc::collect;

class Propellers : public FlyWeight {
protected:
    Buffer<Propeller *> propellers;
    PwmManage *pwmManage;

public:
    Propellers(PowerManage *pm, LoggerFactory *logFac) : FlyWeight(logFac, "Propellers") {
        this->pwmManage = new PwmManage(pm, logFac);
    }
    virtual void setup() {
        this->pwmManage->setup();
    }

    int collectDataItems(Collector &collector, Result &res) {
        int ret = this->pwmManage->collectDataItems(collector, res);
        for (int i = 0; ret > 0 && i < this->propellers.len(); i++) {
            Propeller *p = propellers.get(i, 0);
            ret = p->collectDataItems(collector, res);
        }
        return ret;
    }

    virtual int isReady(Result &res) {
        return this->pwmManage->isReady(res);
    }
    Propeller *get(int idx) {
        return propellers.get(idx, 0);
    }

    void open(bool enable) {
        this->enableAll(enable);
        propellers.forEach<int>(0, [](int c, Propeller *propeller) {
            propeller->open();
        });
    }
    void beforeUpdate() {
        propellers.forEach<int>(0, [](int c, Propeller *propeller) {
            propeller->beforeUpdate();
        });
    }
    void commitUpdate() {
        propellers.forEach<int>(0, [](int c, Propeller *propeller) {
            propeller->commitUpdate();
        });
    }

    void close() {
        propellers.forEach<int>(0, [](int, Propeller *propeller) {
            propeller->close();
        });
    }

    void setLimitInTheory(long minInTheory, long maxInTheory) {
        for (int i = 0; i < propellers.len(); i++) {
            propellers.get(i, 0)->setLimitInTheory(minInTheory, maxInTheory);
        }
    }

    void addThrottle(float th0, float th1, float th2, float th3) {
        propellers[0]->addThrottle(th0);
        propellers[1]->addThrottle(th1);
        propellers[2]->addThrottle(th2);
        propellers[3]->addThrottle(th3);
    }

    void setThrottle(float th0, float th1, float th2, float th3) {
        propellers[0]->setThrottle(th0);
        propellers[1]->setThrottle(th1);
        propellers[2]->setThrottle(th2);
        propellers[3]->setThrottle(th3);
    }

    float getThrottle(int idx) {
        return propellers[idx]->getThrottle();
    }

    void addThrottle(long pwm) {
        propellers.forEach<long>(pwm, [](long pwm, Propeller *propeller) {
            propeller->addThrottle(pwm);
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

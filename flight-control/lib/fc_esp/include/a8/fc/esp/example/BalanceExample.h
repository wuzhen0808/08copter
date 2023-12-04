#pragma once
#include "a8/fc.h"
#include "a8/fc/esp.h"
#include "a8/util.h"
#include "a8/util/sched.h"

namespace a8::fc::esp::example {
using namespace a8::util;
using namespace a8::util::sched;
using namespace a8::fc;

class BalanceExample : public FlyWeight {
    Balancer *balancer;
    Scheduler *sch;

public:
    BalanceExample(Scheduler *sch, System *sys, LoggerFactory *logFac) : FlyWeight(logFac, "BalanceExample") {
        this->sch = sch;
        Rpy *rpy = new EspRpy(logFac);
        Propeller *p1 = new EspPropeller();
        Propeller *p2 = new EspPropeller();
        Propeller *p3 = new EspPropeller();
        Propeller *p4 = new EspPropeller();
        this->balancer = new Balancer(p1, p2, p3, p4, rpy, sys, logFac);

    }

    int start(Result &res) {
        this->balancer->start();
        this->sch->createTimer<BalanceExample *>("BalanceExampleTimer", 100.0f, this, [](BalanceExample *this_) { this_->tick(); });
        return 1;
    }
    void tick() {
        Result res;
        int ret = balancer->update(res);
        if (ret < 0) {
            log(String() << "failed to update balancer,detail:" << res);
        }
        }
};
} // namespace a8::fc::esp::example
#pragma once
#include "a8/fc/Propellers.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/throttle/Throttle.h"
#include "a8/fc/collect/Collector.h"
#include "a8/util.h"
namespace a8::fc::mission {
using namespace a8::util;
using namespace a8::fc::throttle;

class Mission : public FlyWeight {
private:
    Mission(const Mission &);
    Mission &operator=(const Mission &);

protected:
    System *sys;
    ConfigContext &configContext;
    Throttle &throttle;

public:
    Mission(ConfigContext &configContext, Throttle &throttle, System *sys, LoggerFactory *logFac, String name)
        : FlyWeight(logFac, name), configContext(configContext), throttle(throttle) {
        this->sys = sys;
    }

    ~Mission() {
    }
    virtual int setup(Result &res) {
        return 0;
    }
    virtual int collectDataItems(Collector *collector, Result &res) {
        return 1;
    }
    virtual ConfigItem *getForeground() {
        return 0;
    }
    virtual int run(Result &res) = 0;
};

} // namespace a8::fc::mission
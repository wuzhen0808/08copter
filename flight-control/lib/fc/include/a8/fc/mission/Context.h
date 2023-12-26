#pragma once
#include "a8/util.h"
#include "a8/fc/throttle/Throttle.h"
#include "a8/fc/collect/Collector.h"
namespace a8::fc::mission {
using namespace a8::util;
using namespace a8::fc::throttle;

class Context {
private:
    Context(const Context &);
    Context &operator=(const Context &);

public:
    ConfigContext &configContext;
    Throttle &throttle;
    Collector &collector;
    bool landing;
    long startTimeMs;
    long tickCostTimeMs;
    long timeMs;
    Context(Collector &collector, ConfigContext &configContext, Throttle &throttle) : configContext(configContext), throttle(throttle), collector(collector) {
        this->landing = false;
    }

    ~Context() {
    }
    int collectDataItems(Collector &collector, Result &res) {
        int ret = collector.add("timeMs", this->timeMs, res);
        if (ret < 0) {
            return ret;
        }
        ret = collector.add("tickCostTimeMs", this->tickCostTimeMs, res);
        if (ret < 0) {
            return ret;
        }
        ret = collector.add("Landing", this->landing, res);
        if (ret < 0) {
            return ret;
        }
        return ret;
    }

    void preUpdate(long timeMs) {
        this->timeMs = timeMs;
        throttle.preUpdate(timeMs);
    }
};

} // namespace a8::fc::mission
#pragma once
#include "a8/fc/throttle/MainThrottler.h"
#include "a8/fc/History.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

class Pilot : FlyWeight {
    Propellers *propellers;
    throttle::MainThrottler *throttler;
    long startTimeMs;

protected:
    Rpy *rpy;

public:
    Pilot(Config &config, Propellers *propellers,
          Rpy *rpy, LoggerFactory *logFac) : FlyWeight(logFac, "Pilot") {
        this->rpy = rpy;
        this->propellers = propellers;
        this->throttler = new throttle::MainThrottler(config, rpy, logFac);
    }
    ~Pilot() {
        delete this->throttler;
    }

    int checkIfReady(Result res) {
        return 1;
    }

    int start(Context &context, Result &res) {
        int ret = this->checkIfReady(res);
        if (ret < 0) {
            return ret;
        }
        float minInTheory = 0;
        float maxInTheory = 0;
        throttler->getLimitInTheory(minInTheory, maxInTheory);
        this->propellers->setLimitInTheory(minInTheory, maxInTheory);
        return 1;
    }

    int landing(long timeMs) {
        return this->throttler->startLanding(timeMs);
    }

    bool isLanded() {
        return this->throttler->isLanded();
    }

    int update(Context *context, long timeMs, Result &res) {
        
        A8_LOG_DEBUG(logger, String() << ">>update,timeMs:" << timeMs);

        context->startUpdate(timeMs);
        
        int ret = this->throttler->update(*context, res);
        context->message << ",Propellers:[";
        this->propellers->getAll().forEach<Context *>(context, [](Context *c, Propeller *propeller) {
            c->message << (propeller->isEnabled() ? "Enabled" : "Disabled") << ",";
        });
        context->message << "]";

        if (ret > 0) {
            context->commitUpdate();
            context->message << ",Commited.";
        } else {
            context->message << ",Give-up.";
        }
        log(String() << context);
        A8_LOG_DEBUG(logger, String() << "<<update,timeMs:" << timeMs);
        return ret;
    }
    void printHistory(int depth, History&his) {
        his.add(depth, "Pilot-history:");
        this->throttler->printHistory(depth + 1, his);
    }
};

} // namespace a8::fc
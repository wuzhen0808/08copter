#pragma once
#include "a8/fc/throttle/MainThrottler.h"

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
        long minInTheory = 0;
        long maxInTheory = 0;
        throttler->getLimitInTheory(minInTheory, maxInTheory);
        this->propellers->setLimitInTheory(minInTheory, maxInTheory);
        this->propellers->start();
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
        
        A8_LOG_DEBUG(logger, String() << ".update,timeMs:" << timeMs<<",.1");
        int ret = this->throttler->update(*context, res);
        A8_LOG_DEBUG(logger, String() << ".update,timeMs:" << timeMs<<",.2");
        context->message << ",Propellers:[";
        A8_LOG_DEBUG(logger, String() << ".update,timeMs:" << timeMs<<",.4");
        this->propellers->getAll().forEach<Context *>(context, [](Context *c, Propeller *propeller) {
            c->message << (propeller->isEnabled() ? "Enabled" : "Disabled") << ",";
        });
        A8_LOG_DEBUG(logger, String() << ".update,timeMs:" << timeMs<<",.5");
        context->message << "]";

        if (ret > 0) {
            A8_LOG_DEBUG(logger, String() << ".update,timeMs:" << timeMs<<",.6");
            context->commitUpdate();
            A8_LOG_DEBUG(logger, String() << ".update,timeMs:" << timeMs<<",.7");
            context->message << ",Commited.";
        } else {
            A8_LOG_DEBUG(logger, String() << ".update,timeMs:" << timeMs<<",.8");
            context->message << ",Give-up.";
        }
        log(String() << context);
        A8_LOG_DEBUG(logger, String() << "<<update,timeMs:" << timeMs);
        return ret;
    }
    void printHistory(int depth, String &msg) {
        msg << StringUtil::space(depth) << "Pilot-history:\n";
        this->throttler->printHistory(depth + 1, msg);
    }
};

} // namespace a8::fc
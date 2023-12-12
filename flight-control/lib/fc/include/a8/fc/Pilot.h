#pragma once
#include "a8/fc/throttle/MainThrottler.h"

#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

class Pilot : FlyWeight {
    Propellers *propellers;
    throttle::MainThrottler *throttler;
    Config config;

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

    int start(throttle::Context &context, Result &res) {
        int ret = this->checkIfReady(res);
        if (ret < 0) {
            return ret;
        }
        this->propellers->start();
        return 1;
    }

    int landing(long timeMs) {
        return this->throttler->startLanding(timeMs);
    }

    bool isLanded() {
        return this->throttler->isLanded();
    }

    int update(throttle::Context *context, long timeMs, Result &res) {
        context->startUpdate(timeMs);
        int ret = this->throttler->update(*context, res);
        context->message << ",Propellers:[";
        this->propellers->getAll().forEach<throttle::Context *>(context, [](throttle::Context *c, Propeller *propeller) {
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
        return ret;
    }
    void printHistory(int depth, String &msg) {
        msg << StringUtil::space(depth) << "Pilot-history:\n";
        this->throttler->printHistory(depth + 1, msg);
    }
};

} // namespace a8::fc
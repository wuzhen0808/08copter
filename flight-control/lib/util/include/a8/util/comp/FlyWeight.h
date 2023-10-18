#pragma once
#include "a8/util.h"
#include "a8/util/comp/StagingContext.h"
#include "a8/util/comp/TickingContext.h"
#include "a8/util/comp/defines.h"
#include "a8/util/thread.h"
using namespace a8::util;
using namespace a8::util::thread;
namespace a8::util::comp {
class FlyWeight {
protected:
    LoggerFactory *loggerFactory;
    Logger *logger;

public:
    FlyWeight(LoggerFactory *logFac) {
        this->loggerFactory = logFac;
        this->logger = logFac->getLogger("default");
    }

    virtual ~FlyWeight() {
    }

    void log(String msg) {
        logger->info(msg);
    }
};

} // namespace a8::util::comp
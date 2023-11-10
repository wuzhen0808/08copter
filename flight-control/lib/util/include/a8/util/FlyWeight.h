#pragma once
#include "a8/util/Logger.h"
#include "a8/util/LoggerFactory.h"

namespace a8::util {
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

} // namespace a8::util
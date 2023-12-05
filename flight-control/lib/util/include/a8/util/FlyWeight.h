#pragma once
#include "a8/util/Logger.h"
#include "a8/util/LoggerFactory.h"

namespace a8::util {
class FlyWeight {
protected:
    LoggerFactory *loggerFactory;
    Logger *logger;

    void init(LoggerFactory *logFac, String name) {
        this->loggerFactory = logFac;
        this->logger = logFac->getLogger(name);
    }

public:
    FlyWeight() {
    }
    FlyWeight(LoggerFactory *logFac) {
        this->init(logFac, "Unknown");
    }

    FlyWeight(LoggerFactory *logFac, String name) {
        this->init(logFac, name);
    }

    virtual ~FlyWeight() {
    }

    void log(const String &msg) {
        logger->info(msg);
    }
};

} // namespace a8::util
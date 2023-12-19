#pragma once
#include "a8/util/Logger.h"
#include "a8/util/LoggerFactory.h"

namespace a8::util {
class FlyWeight {
protected:
    LoggerFactory *loggerFactory;
    Logger *logger;
    String name;

    void init(LoggerFactory *logFac, String name) {
        this->name = name;
        this->loggerFactory = logFac;
        this->logger = logFac->getLogger(name);
    }

public:
    FlyWeight(LoggerFactory *logFac) {
        this->init(logFac, "Unknown");
    }

    FlyWeight(LoggerFactory *logFac, String name) {
        this->init(logFac, name);
    }

    ~FlyWeight() {
    }
    String getName() {
        return name;
    }

    void log(const String &msg) {
        logger->info(msg);
    }
};

} // namespace a8::util
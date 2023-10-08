#pragma once
#include "a8/link.h"
using namespace a8::link;

namespace a8::gs {

class GsSkeleton : public GsApi {
    LoggerFactory *loggerFactory;
    Logger *logger;

public:
    static void release(void *skeleton) {
        delete static_cast<GsApi *>(skeleton);
    }
    GsSkeleton(LoggerFactory *logFac) {
        this->loggerFactory = logFac;
        this->logger = logFac->getLogger("gsNetImpl");
    }
    ~GsSkeleton() {
    }
    void ping(String msg) override {
        log(String() << "on ping message:" << msg);
    }

    void log(String msg) override {
        logger->info(String() << "on log message:" << msg);
    }

    void unknownMessageReceived(int type, void *data) override {
        logger->info(String() << "unknownMessageReceived, type:" << type);
    }
};
} // namespace a8::gs
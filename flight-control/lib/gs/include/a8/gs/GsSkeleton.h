#pragma once
#include "a8/link.h"
#include "a8/gs/defines.h"
using namespace a8::link;

namespace a8::gs {

class GsSkeleton : public GsApi {
    LoggerFactory *loggerFactory;
    Logger *logger;
    EventCenter *ec;

public:
    static void release(void *skeleton) {
        delete static_cast<GsApi *>(skeleton);
    }

    GsSkeleton(EventCenter *ec, LoggerFactory *logFac) {
        this->ec = ec;
        this->loggerFactory = logFac;
        this->logger = logFac->getLogger("gsNetImpl");
    }
    ~GsSkeleton() {
    }
    int ping(String msg, Result &rst) override {
        logger->info(String() << "on ping message:" << msg);
        return 1;
    }

    int log(String msg, Result &rst) override {
        logger->info(String() << "on log message:" << msg);
        return 1;
    }

    int sensors(SensorsData ssd, Result &rst) override {
        String str;
        logger->info(str << ssd);
        ec->notifyEvent(EventTypes::ON_SENSORS_DATA, &ssd);
        return 1;
    }
};
} // namespace a8::gs
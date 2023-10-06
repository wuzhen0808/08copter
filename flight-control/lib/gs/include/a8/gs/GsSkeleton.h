#pragma once
#include "a8/gs/Dashboard.h"
#include "a8/link.h"
using namespace a8::link;

namespace a8::gs {

class GsSkeleton : public GsApi {
    Dashboard *dashboard;
    LoggerFactory * loggerFactory;
    Logger * logger;
public:
    GsSkeleton(Dashboard *db, LoggerFactory * logFac) {
        this->dashboard = db;
        this->loggerFactory = logFac;
        this->logger = logFac->getLogger("gsNetImpl");
    }
    ~GsSkeleton() {
    }
    void ping(String msg) override {
        log(String() << "on ping message:" << msg);
        dashboard->print(msg);
    }

    void log(String msg) override {
        logger->info(String() << "on log message:" << msg);
        dashboard->print(msg);
    }
};
} // namespace a8::gs
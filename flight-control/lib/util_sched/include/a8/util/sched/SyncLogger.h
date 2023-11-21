#pragma once
#include "a8/util/Logger.h"
#include "a8/util/sched/Lock.h"
#include "a8/util/sched/Scheduler.h"
namespace a8::util::sched {
using namespace a8::util;

class SyncLogger : public Logger {
protected:
    Logger *logger;
    Lock *lock;

public:
    SyncLogger(String name, Logger *logger, Scheduler *sch) : Logger(name) {
        this->logger = logger;
        this->lock = sch->createLock();
    }

    ~SyncLogger() {
        delete this->logger;
    }

    void log(Logger::Level level, const String &msg) override {
        lock->lock();
        logger->log(level, msg);
        lock->unLock();
    };
};

} // namespace a8::util::sched
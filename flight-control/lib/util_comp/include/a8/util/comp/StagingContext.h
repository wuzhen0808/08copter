#pragma once

#include "a8/util.h"
#include "a8/util/schedule.h"

namespace a8::util::comp {
using namespace a8::util;
using namespace a8::util::schedule;

class StagingContext {

private:
    int lastError;
    String message_;
    bool stop_ = false;
    Buffer<String> *path;
    System *sys;

public:
    Properties *properties;
    Scheduler *scheduler;
    LoggerFactory *loggerFactory;

    StagingContext(Scheduler *scheduler, LoggerFactory *loggerFactory, System *sys) {
        this->properties = new Properties();
        this->loggerFactory = loggerFactory;
        this->scheduler = scheduler;
        this->path = new Buffer<String>();
        this->sys = sys;
    }

    System *getSys() {
        return sys;
    }

    Buffer<String> *getPath() {
        return this->path;
    }

    ~StagingContext() {}

    bool isStop() {
        return stop_;
    }

    void stop() {
        stop_ = true;
    }
    void stop(Result rst) {
        stop(rst.errorMessage);
    }
    void stop(String errorMessage) {
        message_ << "\n"
                 << errorMessage;
        stop();
    }

    String &message() {
        return this->message_;
    }
    String getMessage() {
        return message_;
    }
};

} // namespace a8::util::comp
#pragma once

#include "a8/util.h"
#include "a8/util/thread.h"
#include "a8/util/sys.h"
using namespace a8::util;
using namespace a8::util::thread;
using namespace a8::util::sys;

namespace a8::util::comp {

class StagingContext {

private:
    int lastError;
    String message_;
    bool stop_ = false;
    Buffer<String> *path;

public:
    System * sys;
    Properties *properties;
    Scheduler *scheduler;
    LoggerFactory *loggerFactory;

    StagingContext(Scheduler *scheduler, LoggerFactory *loggerFactory, System * sys) {
        this->properties = new Properties();
        this->loggerFactory = loggerFactory;
        this->scheduler = scheduler;
        this->path = new Buffer<String>();
        this->sys = sys;
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

} // namespace a8::util
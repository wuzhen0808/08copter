#pragma once

#include "a8/util.h"
#include "a8/util/thread.h"
using namespace a8::util;
using namespace a8::util::thread;

namespace a8::util::comp {

class Context {

private:
    int lastError;
    String message_;
    bool stop_ = false;
    Buffer<String> *path;

public:
    Properties *properties;
    Scheduler *scheduler;
    LoggerFactory *loggerFactory;

    Context(Scheduler *scheduler, LoggerFactory *loggerFactory) {
        this->properties = new Properties();
        this->loggerFactory = loggerFactory;
        this->scheduler = scheduler;
        this->path = new Buffer<String>();
    }
    Buffer<String> *getPath() {
        return this->path;
    }

    ~Context() {}

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
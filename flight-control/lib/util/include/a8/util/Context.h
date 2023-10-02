#pragma once

#include "a8/util/Buffer.h"
#include "a8/util/Callback2.h"
#include "a8/util/LoggerFactory.h"
#include "a8/util/Properties.h"
#include "a8/util/Runnable.h"
#include "a8/util/Scheduler.h"
#include "a8/util/String.h"
namespace a8::util {

class Context {

private:
    int lastError;
    String message_;
    bool stop_ = false;

public:
    Properties *properties;
    Scheduler *scheduler;
    LoggerFactory *loggerFactory;

    Context(Scheduler *scheduler, LoggerFactory *loggerFactory) {
        this->properties = new Properties();
        this->loggerFactory = loggerFactory;
        this->scheduler = scheduler;
    }

    ~Context() {}

    bool isStop() {
        return stop_;
    }

    void stop() {
        stop_ = true;
    }
    void stop(String errorMessage) {
        message_ << "\n"
                << errorMessage;
        stop();
    }

    void schedule(Runnable *task) {
        scheduler->schedule(task);
    }
    String &message() {
        return this->message_;
    }
    String getMessage() {
        return message_;
    }
};

} // namespace a8::util
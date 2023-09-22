#pragma once

#include "a8/util/Buffer.h"
#include "a8/util/Callback2.h"
#include "a8/util/Runnable.h"
#include "a8/util/Scheduler.h"
#include "a8/util/String.h"
namespace a8::util {

class Context {

private:
    int lastError;
    String message;
    bool stop_ = false;

public:
    Scheduler *scheduler;

    Context(Scheduler *scheduler) {
        this->scheduler = scheduler;
    }

    ~Context() {}

    bool isStop() {
        return stop_;
    }

    void stop(String errorMessage) {
        this->message = errorMessage;
        this->stop_ = true;
    }

    void schedule(Runnable *task) {
        this->scheduler->schedule(task);
    }

    String getMessage() {
        return message;
    }
};

} // namespace a8::util
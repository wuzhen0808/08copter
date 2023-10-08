#pragma once
#include "a8/util.h"
#include "a8/util/comp/StagingContext.h"
#include "a8/util/thread.h"

using namespace a8::util;
using namespace a8::util::thread;
namespace a8::util::comp {

class TickingContext {
    StagingContext *staging;
    Rate rate;
    int group;
    long startTimestamp;
    // for thead safe, attributes is used to store thread local variables.
    Buffer<void *> variables;

public:
    TickingContext(StagingContext *staging, Rate rate, int group) {
        this->staging = staging;
        this->rate = rate;
        this->group = group;
    }

    void setVar(int key, void *var) {
        this->variables.set(key, 0, var);
    }

    void *removeVar(int key) {
        void *vPtr = this->variables.get(key, 0);
        this->variables.set(key, 0);
        return vPtr;
    }

    template <typename T>
    T *getVar(int key) {
        void *vPtr = variables.get(key, 0);
        return static_cast<T *>(vPtr);
    }

    int getGroup() {
        return group;
    }
    Rate getRate() {
        return this->rate;
    }
    StagingContext *getStaging() {
        return this->staging;
    }

    void beforTick() {
        this->startTimestamp = staging->getSys()->millis();
    }
};

} // namespace a8::util::comp
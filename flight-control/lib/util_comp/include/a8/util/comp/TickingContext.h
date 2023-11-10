#pragma once
#include "a8/util.h"
#include "a8/util/comp/StagingContext.h"
#include "a8/util/schedule.h"

namespace a8::util::comp {
using namespace a8::util;
using namespace a8::util::schedule;

class TickingContext {
    StagingContext *staging;
    Rate rate;
    int group;
    long startTimestamp_;
    // for thead safe, attributes is used to store thread local variables.
    Buffer<void *> variables;
    //    
public:
    int ret;

    Result rst;

    TickingContext(StagingContext *staging, Rate rate, int group) {
        this->staging = staging;
        this->rate = rate;
        this->group = group;
        this->ret = 0;
    }
    long getStartTimestamp(){
        return startTimestamp_;
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
        this->startTimestamp_ = staging->getSys()->getSteadyTime();
    }
    void afterTick() {
        this->ret = 0;
        this->rst.reset();
    }
};

} // namespace a8::util::comp
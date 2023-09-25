#pragma once
#include "a8/util/Component.h"

namespace a8::util {
class RunnerComponent : public Component {
    Runnable *runner;

public:
    RunnerComponent(const char *name, Runnable *runner) : Component(name) {
        this->runner = runner;
        this->rate = Rate::ForEver;
    }
    virtual void run() {
        this->runner->run();
    }
};

} // namespace a8::util

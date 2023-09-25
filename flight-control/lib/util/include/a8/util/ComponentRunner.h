#pragma once
#include "a8/util/Component.h"

namespace a8::util {
class ComponentRunner : public Runnable {
    Component *component;

public:
    ComponentRunner(Component *com) {
        this->component = com;
    }
    virtual void run() {
        this->component->run();
    }
};

} // namespace a8::util

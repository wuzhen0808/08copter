#pragma once
#include "a8/util/comp/Component.h"
using namespace a8::util;
using namespace a8::util::thread;
namespace a8::util::comp {
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

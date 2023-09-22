#ifndef COPTER_COM__
#define COPTER_COM__
#include "a8/core/Runnable.h"
#include "a8/hal/Hal.h"
#include "a8/util/Attributes.h"
#include "a8/util/Buffer.h"
#include "a8/util/String.h"

using a8::util::Attributes;
using a8::util::Buffer;
namespace a8::core {

class Component {

protected:
    Attributes *attributes;
    Buffer<Runnable *> *tasks;

public:
    Component();
    ~Component();
    void add(Runnable *task);

    int getTotalTasks() {
        return this->tasks->getLength();
    }

    Runnable *getTask(int idx) {
        return this->tasks->get(idx);
    }

    void log(a8::util::String msg);
    Attributes *getAttributes();
};
} // namespace a8::core
#endif

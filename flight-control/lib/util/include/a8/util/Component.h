#ifndef COPTER_COM__
#define COPTER_COM__
#include "a8/util/Attributes.h"
#include "a8/util/Buffer.h"
#include "a8/util/Context.h"
#include "a8/util/Runnable.h"
#include "a8/util/String.h"

using namespace a8::util;

namespace a8::util {

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

    virtual void populate(Context* context) {

    }

    virtual void setup(Context* context) {

    }
    
    virtual void start(Context* context) {

    }
};
} // namespace a8::util
#endif

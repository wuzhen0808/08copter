#include "a8/core/Component.h"

namespace a8::core {

Component::Component() {
    this->tasks = new Buffer<Runnable *>();
}

Component::~Component() {
    delete[] attributes;
    attributes = 0;
}

void Component::add(Runnable *task) {
    this->tasks->append(task);
}

Attributes *Component::getAttributes() {
    if (attributes == 0) {
        attributes = new util::Attributes();
    }
    return attributes;
}

void Component::log(a8::util::String msg) {
    a8::hal::S->out->println(msg);
}

} // namespace a8::core
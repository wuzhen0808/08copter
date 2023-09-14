#include "a8/core/Component.h"
namespace a8 {
namespace core {
Component::Component() {
}

Component::~Component() {
    delete[] attributes;
    attributes = 0;
}

util::Attributes *Component::getAttributes() {
    if (attributes == 0) {
        attributes = new util::Attributes();
    }
    return attributes;
}

void Component::log(a8::util::String msg) {
    a8::hal::S->out->println(msg);
}

} // namespace core
} // namespace a8
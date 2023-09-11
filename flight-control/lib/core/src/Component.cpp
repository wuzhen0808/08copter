#include "a8/core/Component.hpp"
namespace a8 {
namespace core {
Component::Component(Copter *copter) {
    this->copter = copter;
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

void Component::log(string msg) {
    copter->log(msg);
}

} // namespace core
} // namespace a8
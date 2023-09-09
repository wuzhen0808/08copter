#include "a8/Component.hpp"
namespace a8 {

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

void Component::log(String msg) {
    copter->log(msg);
}

} // namespace a8
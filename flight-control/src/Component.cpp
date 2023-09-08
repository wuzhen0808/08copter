#include "Component.h"

Component::Component(Copter *copter) {
    this->copter = copter;
}

void Component::log(String msg) {
    copter->log(msg);
}

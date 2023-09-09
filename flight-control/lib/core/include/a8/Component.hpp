#ifndef COPTER_COM__
#define COPTER_COM__

#include "a8/Copter.hpp"
#include "a8/util/Attributes.hpp"
namespace a8 {

class Component {
public:
    Component(Copter *copter);
    ~Component();
    void log(String msg);
    util::Attributes *getAttributes();

protected:
    Copter *copter;
    util::Attributes *attributes;
};
} // namespace a8
#endif

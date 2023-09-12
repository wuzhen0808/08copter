#ifndef COPTER_COM__
#define COPTER_COM__

#include "a8/core/Copter.hpp"
#include <a8/util/Attributes.hpp>

using namespace a8::util;

namespace a8 {
namespace core {
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
} // namespace core
} // namespace a8
#endif

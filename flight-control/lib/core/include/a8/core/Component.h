#ifndef COPTER_COM__
#define COPTER_COM__
#include "a8/util/String.h"
#include "a8/util/Attributes.h"
#include "a8/hal/Hal.h"

using a8::util::Attributes;

namespace a8 {
namespace core {
class Component {
public:
    Component();
    ~Component();
    void log(a8::util::String msg);
    Attributes *getAttributes();

protected:    
    Attributes *attributes;
};
} // namespace core
} // namespace a8
#endif

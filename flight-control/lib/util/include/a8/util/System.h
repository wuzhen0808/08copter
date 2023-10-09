#pragma once
#include "a8/util/Output.h"

namespace a8::util {
    
class System {
public:
    Output *out;
    virtual long millis() = 0;
    virtual void sleep(long ms) = 0;
};

} // namespace a8::util::sys

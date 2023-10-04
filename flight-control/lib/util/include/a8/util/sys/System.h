#pragma once
#include "a8/util.h"


using namespace a8::util;

namespace a8::util::sys {
    
class System {
public:
    Output *out;
    virtual long millis() = 0;
    virtual void sleep(long ms) = 0;
};

} // namespace a8::util::sys

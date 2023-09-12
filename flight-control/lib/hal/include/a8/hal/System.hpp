#pragma once
#include "a8/util/Output.h"
#include "a8/util/String.hpp"

namespace a8 {
namespace hal {

class System {
public:
    Output *out;
    virtual long millis() = 0;
    virtual void memcpy(void *to, const void *from, int size) = 0;
};

} // namespace hal
} // namespace a8

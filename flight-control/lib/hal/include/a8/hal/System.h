#pragma once
#include "a8/util/Output.h"
#include "a8/util/String.h"

namespace a8 {
namespace hal {

class System {
public:
    Output *out;
    virtual long millis() = 0;
};

} // namespace hal
} // namespace a8

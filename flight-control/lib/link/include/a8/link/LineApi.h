#pragma once

#include "a8/util/String.h"

namespace a8::link {
using namespace a8::util;

class LineApi {
public:
    virtual int line(String line, Result &rst) = 0;
};

} // namespace a8::link

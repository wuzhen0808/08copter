#pragma once

#include "a8/util/String.h"
using namespace a8::util;

namespace a8::link {

class LineApi {
public:
    virtual int line(String line, Result &rst) = 0;
};

} // namespace a8::link

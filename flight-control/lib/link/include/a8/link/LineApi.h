#pragma once

#include "a8/util/String.h"
using namespace a8::util;

namespace a8::link {

class LineApi {
public:
    virtual void line(String line) = 0;
};

} // namespace a8::link

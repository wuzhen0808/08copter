#pragma once

#include "a8/util/String.h"
using namespace a8::util;

namespace a8::link {

class FcApi {
public:
    // send a message to see if the FCS is responsible or not.
    virtual void ping(String msg) = 0;
    virtual void command(String cmd) = 0;
};

} // namespace a8::link

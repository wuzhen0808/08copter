#pragma once

#include "a8/util.h"
using namespace a8::util;

namespace a8::link {

class TsApi {
public:
    // send a message to see if the FCS is responsible or not.
    virtual int ping(String msg, Result &rst) = 0;

};

} // namespace a8::link

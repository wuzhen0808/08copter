#pragma once

#include "a8/util/String.h"
using namespace a8::util;

namespace a8::net {

// Stub to flight control system.
class FcsStub {
    // send a message to see if the FCS is responsible or not.
    virtual String ping(String msg) = 0;

    // connect to the fcs
    virtual void connect() = 0;

    // send start command to FCS.
    virtual void start() = 0;

    // send stop command to FCS.
    virtual void start() = 0;
};

} // namespace  a8::api

#pragma once

#include "a8/util.h"

using namespace a8::util;

namespace a8::link {

// Stub to ground station interface.
class GsApi {

public:
    // check if the GS responsible or not.
    virtual void ping(String msg) = 0;

    // send log message to GS.
    virtual void log(String msg) = 0;

    virtual void unknownMessageReceived(int type, void *data) {
        // do nothing.
    }
};
} // namespace a8::link

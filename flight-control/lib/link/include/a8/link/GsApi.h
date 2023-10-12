#pragma once

#include "a8/link/SensorsData.h"
#include "a8/util.h"

using namespace a8::util;

namespace a8::link {

// Stub to ground station interface.
class GsApi {

public:
    // check if the GS responsible or not.
    virtual int ping(String msg, Result &rst) = 0;

    // send log message to GS.
    virtual int log(String msg, Result &rst) = 0;

    virtual int sensors(SensorsData ssd, Result &rst) = 0;

    virtual int unknownMessageReceived(int type, void *data, Result &rst) {
        rst << "unknown message received, type:" << type;
        return 1;
    }
};
} // namespace a8::link

#pragma once

#include "a8/util/String.h"
namespace a8::link {
using namespace a8::util;


class FcApi {
public:
    // send a message to see if the FCS is responsible or not.
    virtual int ping(String msg, Result &rst) = 0;
    virtual int command(String cmd, Result &rst) = 0;
    virtual int test(Result &rst) = 0;
    virtual int throttle(float throttle, Result &rst) = 0;
    virtual int roll(float roll, Result &rst) = 0;
    virtual int pitch(float pitch, Result &rst) = 0;
    virtual int yaw(float yaw, Result &rst) = 0;
};

} // namespace a8::link

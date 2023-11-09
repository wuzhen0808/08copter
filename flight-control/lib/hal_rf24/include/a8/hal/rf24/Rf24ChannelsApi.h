#pragma once
#include "a8/util/net.h"

namespace a8::hal::nrf24 {

class Rf24ChannelsApi {
public:
    virtual int readToQueue(int channelId) = 0;
    virtual int create() = 0;
};

} // namespace a8::hal::nrf24
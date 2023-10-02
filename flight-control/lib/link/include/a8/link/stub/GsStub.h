#pragma once

#include "a8/link/GsApi.h"
#include "a8/util.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::link {

// Stub to ground station interface.
class GsStub : public GsApi {

protected:
    Channel* channel;

public:
    
    GsStub(Channel* channel) {
        this->channel = channel;
    }

    // check if the GS responsible or not.
    void ping(String msg) override {
        this->channel->send(CommonMessageType::PING, &msg);
    }

    // send log message to GS.
    void log(String msg) override {
        this->channel->send(CommonMessageType::LOG, &msg);
    }
};

} // namespace a8::net

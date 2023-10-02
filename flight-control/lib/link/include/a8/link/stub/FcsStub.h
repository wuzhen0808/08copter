#pragma once
#include "a8/link/FcsApi.h"
#include "a8/util/net.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::link {

// Stub to flight control system.
class FcsStub : public FcsApi {
    Channel *channel;

public:
    FcsStub(Channel *channel) {
        this->channel = channel;
    }
    void ping(String msg) override {
        this->channel->send(CommonMessageType::PING, &msg);
    }
};

} // namespace a8::net

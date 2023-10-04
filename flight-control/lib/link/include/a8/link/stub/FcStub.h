#pragma once
#include "a8/link/FcApi.h"
#include "a8/util/net.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::link {

// Stub to flight control system.
class FcStub : public FcApi {
    Channel *channel;

public:
    FcStub(Channel *channel) {
        this->channel = channel;
    }
    void ping(String msg) override {
        this->channel->send(CommonMessageType::PING, &msg);
    }
    void command(String cmd) override {
        this->channel->send(CommonMessageType::CMD, &cmd);
    }
};

} // namespace a8::link

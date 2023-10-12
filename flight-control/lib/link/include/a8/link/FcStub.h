#pragma once
#include "a8/link/FcApi.h"
#include "a8/link/GsApi.h"
#include "a8/link/defines.h"
#include "a8/util/net.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::link {

// Stub to flight control system.
class FcStub : public FcApi {
    Channel *channel;

public:
    static FcStub *create(Channel *channel) {
        return new FcStub(channel);
    }

    static void release(void *stub) {
        delete static_cast<FcStub *>(stub);
    }

    FcStub(Channel *channel) {
        this->channel = channel;
    }
    int ping(String msg, Result &rst) override {

        return this->channel->send(CommonMessageType::PING, &msg, rst);
    }
    int command(String cmd, Result &rst) override {
        return this->channel->send(CommonMessageType::CMD, &cmd, rst);
    }
    int test(Result &rst) override {
        return this->channel->send(CommonMessageType::TEST, 0, rst);
    }
};

} // namespace a8::link

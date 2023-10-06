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
    Channel<GsApi, FcApi> *channel;

public:
    static FcApi *create(Channel<GsApi, FcApi> *channel) {
        return new FcStub(channel);
    }

    static void release(FcApi *stub) {
        delete stub;
    }

    FcStub(Channel<GsApi, FcApi> *channel) {
        this->channel = channel;
    }
    void ping(String msg) override {
        Result rst;
        this->channel->send(CommonMessageType::PING, &msg, rst);
    }
    void command(String cmd) override {
        Result rst;
        this->channel->send(CommonMessageType::CMD, &cmd, rst);
    }
};

} // namespace a8::link

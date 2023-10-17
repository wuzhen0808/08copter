#pragma once
#include "a8/link/FcApi.h"
#include "a8/link/TsApi.h"
#include "a8/link/defines.h"
#include "a8/util/net.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::link {

// Stub to ground station interface.
class TsStub : public TsApi {

protected:
    Channel *channel;

public:
    static TsStub *create(Channel *channel) {
        return new TsStub(channel);
    }

    TsStub(Channel *channel) {
        this->channel = channel;
    }

    // check if the GS responsible or not.
    int ping(String msg, Result &rst) override {
        return this->channel->send(CommonMessageType::PING, &msg, rst);
    }
};

} // namespace a8::link

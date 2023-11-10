#pragma once
#include "a8/link/FcApi.h"
#include "a8/link/GsApi.h"
#include "a8/link/defines.h"
#include "a8/util/net.h"

namespace a8::link {
using namespace a8::util;
using namespace a8::util::net;


// Stub to ground station interface.
class GsStub : public GsApi {

protected:
    Channel *channel;

public:
    static GsStub *create(Channel *channel) {
        return new GsStub(channel);
    }

    GsStub(Channel *channel) {
        this->channel = channel;
    }

    // check if the GS responsible or not.
    int ping(String msg, Result &rst) override {
        return this->channel->send(CommonMessageType::PING, &msg, rst);
    }

    // send log message to GS.
    int log(String msg, Result &rst) override {
        return this->channel->send(CommonMessageType::LOG, &msg, rst);
    }
    int sensors(SensorsData ssd, Result &rst) override {
        return this->channel->send(CopterMessageType::SENSORS_DATA, &ssd, rst);
    }
};

} // namespace a8::link

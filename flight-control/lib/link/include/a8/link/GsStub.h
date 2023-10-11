#pragma once
#include "a8/link/FcApi.h"
#include "a8/link/GsApi.h"
#include "a8/link/defines.h"
#include "a8/util/net.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::link {

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
    void ping(String msg) override {
        Result rst;
        this->channel->send(CommonMessageType::PING, &msg, rst);
    }

    // send log message to GS.
    void log(String msg) override {
        Result rst;
        this->channel->send(CommonMessageType::LOG, &msg, rst);
    }
    void sensors(SensorsData ssd) override {
        Result rst;
        this->channel->send(CopterMessageType::SENSORS_DATA, &ssd, rst);
    }
};

} // namespace a8::link

#pragma once
#include "a8/link/FcApi.h"
#include "a8/link/GsApi.h"
#include "a8/link/defines.h"
#include "a8/util/net.h"

namespace a8::link {
using namespace a8::util;
using namespace a8::util::net;


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
    int throttle(float throttle, Result &rst) override {
        return this->channel->send(CopterMessageType::THROTTLE, &throttle, rst);
    }
    int roll(float roll, Result &rst) override {
        return this->channel->send(CopterMessageType::ROLL, &roll, rst);
    }
    int pitch(float pitch, Result &rst) override {
        return this->channel->send(CopterMessageType::PITCH, &pitch, rst);
    }
    int yaw(float yaw, Result &rst) override {
        return this->channel->send(CopterMessageType::YAW, &yaw, rst);
    }
};

} // namespace a8::link

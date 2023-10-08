#pragma once
#include "a8/link/LineApi.h"
#include "a8/link/defines.h"
#include "a8/util/net.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::link {

// Stub to flight control system.
class LineStub : public LineApi {
    Channel *channel;

public:
    static void *create(Channel *channel) {
        return new LineStub(channel);
    }

    static void release(void *stub) {
        delete static_cast<LineStub *>(stub);
    }

    LineStub(Channel *channel) {
        this->channel = channel;
    }

    void line(String line) override {
        Result rst;
        this->channel->send(CommonMessageType::LINE, &line, rst);
    }
};

} // namespace a8::link

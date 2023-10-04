#pragma once
#include "a8/util.h"

using namespace a8::util;

namespace a8::gs {

class Net : public Network {
    Sockets *sockets;
    SOCK sock;
    int port;

public:
    Net(Sockets *sockets, int port) : Component("net") {
        this->sockets = sockets;
        this->port = port;
    }

    void setup(StagingContext *context) override {
    }

    void postStart(StagingContext *context) override {
    }

    void shutdown(StagingContext *context) override {
    }
};

} // namespace a8::gs
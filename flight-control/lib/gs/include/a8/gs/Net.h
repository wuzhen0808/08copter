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

    void setup(Context *context) override {
    }

    void postStart(Context *context) override {
    }

    void shutdown(Context *context) override {
    }
};

} // namespace a8::gs
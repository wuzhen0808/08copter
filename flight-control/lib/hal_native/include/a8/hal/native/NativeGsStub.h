#pragma once
#include "a8/net/GsStub.h"
#include "a8/util/SocketConnector.h"
#include "a8/util/SocketRunnerComponent.h"
#include "a8/util/Sockets.h"
#include "a8/util/SocketWriter.h"

using namespace a8::net;

namespace a8::hal::native {
// TODO this class should not exist for that all the logic here should be split to two parts:
//  one is located in hal;
//  another is located in net.
// net depends on hal.
// net is configurable to select different hal hardware.
// for example socket or other protocol to communicate.

class NativeGsStub : public GsStub, public SocketConnector, public SocketRunnerComponent {
    Sockets *sockets;
    SOCK sockIn;

public:
    NativeGsStub(Sockets *sockets, int port) : GsStub(0), SocketRunnerComponent("gss", sockets, port, this) {
        this->sockets = sockets;
    }
    void send(int type, void *msg) override {
        if (this->writer == 0) { //
            // no gs connected in
            return;
        }
        GsStub::send(type, msg);
    }
    void onConnection(Sockets *sockets, SOCK sockIn) override {
        this->sockets = sockets;
        this->sockIn = sockIn;
        SocketWriter *writer = new SocketWriter(sockets, sockIn);
        this->writer = writer;
    }
};

} // namespace a8::hal::native

#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include "a8/util/net/Sockets.h"

namespace a8::util::net {

/**
 */
class SocketWriter : public Writer {
protected:
    Sockets *sockets;
    SOCK sock;

public:
    SocketWriter(Sockets *sockets, SOCK sock) {
        this->sock = sock;
        this->sockets = sockets;
    }

    ~SocketWriter() {
    }

    int write(const char *buf, int bufLen) override {
        sockets->send(sock, buf, bufLen);
        return bufLen;
    };
};

} // namespace a8::util
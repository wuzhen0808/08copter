#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Sockets.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"

namespace a8::util {

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

    void write(const char *buf, int bufLen) override {
        sockets->send(sock, buf, bufLen);
    };
};

} // namespace a8::util
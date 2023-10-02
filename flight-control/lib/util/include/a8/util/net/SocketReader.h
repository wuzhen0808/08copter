#pragma once
#include "a8/util/Logger.h"
#include "a8/util/Reader.h"
#include "a8/util/Runnable.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/String.h"

using namespace a8::util;
namespace a8::util::net {

class SocketReader : public Reader {
private:
    Sockets *sockets;
    SOCK socket;

public:
    SocketReader(Sockets *sockets, SOCK sock) {
        this->sockets = sockets;
        this->socket = sock;
    }

    ~SocketReader() {
    }

    virtual int read(char *buf, int bufLen) {
        return sockets->receive(socket, buf, bufLen);
    }
};
} // namespace a8::util
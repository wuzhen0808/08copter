#pragma once
#include "a8/util/Logger.h"
#include "a8/util/Reader.h"
#include "a8/util/Runnable.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/String.h"

namespace a8::util::net {
using namespace a8::util;

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

    int read(char *buf, int bufLen, Result& res) override{
        return sockets->receive(socket, buf, bufLen, res);        
    }
};
} // namespace a8::util
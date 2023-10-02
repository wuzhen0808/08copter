#pragma once
#include "a8/util/net/Sockets.h"

namespace a8::util::net {

class SocketConnector {
    Sockets *sockets;
    SOCK sock;
    int port;
public:
    SocketConnector(Sockets *sockets) {        
        this->sockets = sockets;
    }

    void connect(int port) {
        
    }
};

} // namespace a8::util
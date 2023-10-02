#pragma once
#include "a8/util/net/defines.h"
#include "a8/util/net/Sockets.h"

namespace a8::util::net {
enum Status {
    Idle,
    Bond,
    Listening

};

class Address {
public:
    String host;
    int port;
    SOCK sock;
    Status status;

    /**
     * After the address is connected as the client side code.
     * Or after the address is listening new connection as the server side code.
     *
     */
    FuncType::handle listen;
    void *receiver;

    Address(String host, int port) {
        this->host = host;
        this->port = port;
        this->status = Idle;
    }
    void bond(SOCK sock) {
        this->sock = sock;
        this->status = Bond;
    }
    void listening(FuncType::handle listen, void *context) {
        this->listen = listen;
        this->receiver = receiver;
    }
};

} // namespace a8::util::net

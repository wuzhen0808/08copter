#pragma once
#include "a8/hal/rf24/Rf24Node.h"
#include "a8/hal/rf24/Rf24Payload.h"
#include "a8/util/net.h"

namespace a8::hal::nrf24 {
using namespace a8::util;
enum Role {
    Unknown,
    Connector,
    Listener,
    Worker
};

enum Status {
    Idle,
    Connecting,
    Connected,
    Listening,
    Working
};
class Rf24Sock {
    Role role;
    Status status;
    int id;         // sock id
    Rf24Node *node; // local node
    int port;       // local port
    //
    int node2; // remote node
    int port2; // remote port

public:
    Rf24Sock(int id);
    ~Rf24Sock();
    void close();
    int connect(int node2, int port2);

    int bind(Rf24Node *node, int port) {
        if (this->node != 0) {
            return -1; // rebind not supported.
        }
        this->node = node;
        this->port = port;
        return 1;
    }

    int getPort() {
        return this->port;
    }
    int listen() {
        if (this->role != Role::Unknown) {
            return -1;
        }
        this->role = Role::Listener;
        this->status = Status::Listening;
    }

    template <typename C>
    int receive(C c, void (*consume)(Rf24ChannelFrame *payload)) {

    }
};

} // namespace a8::hal::nrf24
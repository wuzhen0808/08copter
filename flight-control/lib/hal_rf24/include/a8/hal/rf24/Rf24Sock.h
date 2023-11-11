#pragma once
#include "a8/hal/rf24/Rf24Node.h"

#include "a8/util/net.h"

namespace a8::hal::rf24 {
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
    Rf24Sock(int id) {
        this->role = Role::Unknown;
        this->id = id;
        this->node = 0;
        this->port = 0;
    }
    ~Rf24Sock() {}
    void close() {
        switch (this->role) {
        case Idle:
            break;
        case Connector:
            break;
        case Listener:
            break;
        case Worker:
            break;
        default:
            break;
        }
    }
    int connect(int node2, int port2, Result &res) {
        if (this->role == Role::Unknown) {
            this->role = Role::Connector;
            this->status = Status::Idle;
        }

        if (this->role != Role::Connector) {
            res << "cannot connect, because role(" << this->role << ") is not expected:" << Role::Connector;
            return -1;
        }

        if (this->status != Status::Idle) {
            res << "cannot connect, because status(" << this->status << ") is not expected:" << Status::Idle;
            return -2;
        }
        this->status = Status::Connecting;
        this->port2 = port2;

        // TODO send message to remote node and port
        this->status = Status::Connected;
        return 1;
    }

    int bind(Rf24Node *node, int port, Result &res) {
        if (this->node != 0) {
            res << "cannot rebind, sock is already bind to node/port:" << node->getId() << "/" << port << "";
            return -1; // rebind not supported.
        }
        this->node = node;
        this->port = port;
        return 1;
    }

    int getPort() {
        return this->port;
    }
    int listen(Result &res) {
        if (this->role != Role::Unknown) {
            res << "cannot listener, because the role(" << role << ") is not expected" << Role::Unknown;
            return -1;
        }
        this->role = Role::Listener;
        this->status = Status::Listening;
        return 1;
    }
};

} // namespace a8::hal::rf24
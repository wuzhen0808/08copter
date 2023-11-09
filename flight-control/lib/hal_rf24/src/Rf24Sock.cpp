#include "a8/hal/rf24/Rf24Socket.h"
#include "a8/hal/rf24/Rf24Sockets.h"

namespace a8::hal::nrf24 {

Rf24Sock::Rf24Sock(int id) {
    this->id = id;
    this->node = 0;
    this->port = 0;
}

Rf24Sock::~Rf24Sock() {
}

void Rf24Sock::close() {
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

int Rf24Sock::getPort() {
    return this->port;
}

int Rf24Sock::connect(int node2, int port2) {
    if (this->role == Role::Unknown) {
        this->role = Role::Connector;
    }

    if (this->role != Role::Connector) {
        return -1;
    }

    if (this->status != Status::Idle) {
        return -2;
    }
    this->status = Status::Connecting;
    this->port2 = port2;

    //TODO send message to remote node and port
    this->status = Status::Connected;
}
} // namespace a8::hal::nrf24
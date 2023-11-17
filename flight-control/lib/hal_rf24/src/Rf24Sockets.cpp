#include "a8/hal/rf24/Rf24Sockets.h"
#include "a8/hal/rf24/Rf24NetData.h"
#include "a8/hal/rf24/Rf24Sock.h"

#include <RF24.h>
#include <RF24Network.h>

namespace a8::hal::rf24 {

Rf24Sockets::Rf24Sockets(int id, Rf24Hosts *hosts, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, "Rf24Sockets") {
    this->hosts = hosts;
    this->host = host;
    this->ports = new Rf24Ports();
    this->node = new Rf24Node(hosts, id, sys, sch, logFac);
    this->socks = new Rf24Socks(node, hosts, ports, sys, sch, logFac);
    this->sys = sys;
    this->sch = sch;
}

Rf24Sockets::~Rf24Sockets() {
    delete this->socks;
    delete this->node;
    delete this->ports;
}

int Rf24Sockets::setup(int chipEnablePin, int chipSelectPin, int channel, Result &res) {
    int ret = this->node->setup(
        chipEnablePin, chipSelectPin, channel, this, [](void *this_, Rf24NetData *data) {
            Rf24Sockets *this__ = static_cast<Rf24Sockets *>(this_);
            this__->onNetData(data);
        },
        res);
    if (ret < 0) {
        return ret;
    }

    return ret;
}

void Rf24Sockets::onNetData(Rf24NetData *data) {

    if (data->node2 != this->node->getId()) {
        
        log(String() << "no node2 found with connect request:" << data);
        this->sendResponse(data->node1, data->port1);
        return;
    }

    Rf24Sock *s = this->socks->findByPort(data->port2);
    if (s == 0) {
        log(String() << "no port2 found with connect request:" << data);
        this->sendResponse(data->node1, data->port1);
        return;
    }
    s->onData(data);
}

int Rf24Sockets::socket(SOCK &sock) {
    Rf24Sock *s = this->socks->create();
    sock = s->getId();
    return 1;
}

int Rf24Sockets::close(SOCK sock) {

    return this->socks->close(sock);
}

int Rf24Sockets::connect(SOCK sock, const String host2, int port2, Result &res) {
    return socks->connect(sock, host2, port2, res);
}

int Rf24Sockets::bind(SOCK sock, const String host, int port, Result &res) {
    return socks->bind(sock, host, port, this->hosts, res);
}

int Rf24Sockets::listen(SOCK sock, Result &res) {
    return socks->listen(sock, res);
}
/**
 * Accept one good incoming connection, ignore the bad connections if necessary.
 */
int Rf24Sockets::accept(SOCK sock, SOCK &sockIn, Result &res) {
    int sId2;
    int ret = socks->accept(sock, sId2, res);
    if (ret > 0) {
        sockIn = sId2;
    }
    return ret;
}

bool Rf24Sockets::send(SOCK sock, const char *buf, int len, Result &res) {
    return socks->send(sock, buf, len, res);
}

int Rf24Sockets::receive(SOCK sock, char *buf, int bufLen, Result &res) {
    return socks->receive(sock, buf, bufLen, res);
}

int Rf24Sockets::getLastError() {
    return -1;
}

int Rf24Sockets::select(SOCK &sock) {
    return -1;
}
int Rf24Sockets::select(Buffer<SOCK> &buffer1, Buffer<SOCK> &buffer2, Buffer<SOCK> &buffer3) {
    return -1;
}
} // namespace a8::hal::rf24
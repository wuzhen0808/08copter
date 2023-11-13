#include "a8/hal/rf24/Rf24Sockets.h"
#include "a8/hal/rf24/Rf24Sock.h"

#include <RF24.h>
#include <RF24Network.h>

namespace a8::hal::rf24 {

Rf24Sockets::Rf24Sockets(int id, Rf24Hosts *hosts, System *sys, LoggerFactory *logFac) : FlyWeight(logFac) {
    this->hosts = hosts;
    this->host = host;
    this->ports = new Rf24Ports();    
    this->node = new Rf24Node(hosts, id, sys, logFac);
    this->socks = new Rf24Socks(node, hosts, ports, logFac);
    this->sys = sys;
}

int Rf24Sockets::setup(int chipEnablePin, int chipSelectPin, int channel, Result& res){
    return this->node->setup(chipEnablePin, chipSelectPin, channel, res);
}

Rf24Sockets::~Rf24Sockets() {
    delete this->socks;
    delete this->node;
    delete this->ports;
}

int Rf24Sockets::socket(SOCK &sock) {
    int id = this->socks->create(this->sys);
    sock = id;
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

int Rf24Sockets::accept(SOCK sock, SOCK &sockIn) {
    // receive a connect message from underlayer protocol.
    // create a worker socket.
    return socks->accept(sock, sockIn);
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
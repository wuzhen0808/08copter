#include "a8/hal/rf24/Rf24Sockets.h"
#include "a8/hal/rf24/Rf24Sock.h"

#include <RF24.h>
#include <RF24Network.h>

namespace a8::hal::rf24 {

Rf24Sockets::Rf24Sockets(Rf24Hosts *hosts, String host, int chipEnablePin, int chipSelectPin) {    
    this->hosts = hosts;
    this->host = host;
    this->radio = new RF24(chipEnablePin, chipSelectPin);
    this->network = new Rf24NetworkWrapper(new ESBNetwork<RF24>(*this->radio));    
    this->ports = new Rf24Ports();
}

Rf24Sockets::~Rf24Sockets() {
    delete this->socks;
    delete this->network;
    delete this->radio;
    delete this->ports;
}

int Rf24Sockets::socket(SOCK &sock) {
    int id = this->socks->create();
    sock = id;
    return 1;
}

int Rf24Sockets::close(SOCK sock) {
    return this->socks->close(sock);
}

int Rf24Sockets::connect(SOCK sock, const String host2, int port2, Result &res) {    
    return socks->connect(sock, host2, port2);
}

int Rf24Sockets::bind(SOCK sock, const String host, int port, Result &res) {
    return socks->bind(sock, host, port, this->hosts);
}

int Rf24Sockets::listen(SOCK sock, Result &rst) {
    return socks->listen(sock);
}

int Rf24Sockets::accept(SOCK sock, SOCK &sockIn) {
    //receive a connect message from underlayer protocol.
    //create a worker socket.
    return socks->accept(sock, sockIn);
}

bool Rf24Sockets::send(SOCK sock, const char *buf, int len) {
    return -1;
}

int Rf24Sockets::receive(SOCK sock, char *buf, int bufLen) {
    return -1;
}

int Rf24Sockets::getLastError() {
    return -1;
}
bool Rf24Sockets::isReady() {
    return -1;
}

int Rf24Sockets::select(SOCK &sock) {
    return -1;
}
int Rf24Sockets::select(Buffer<SOCK> &buffer1, Buffer<SOCK> &buffer2, Buffer<SOCK> &buffer3) {
    return -1;
}
} // namespace a8::hal::nrf24
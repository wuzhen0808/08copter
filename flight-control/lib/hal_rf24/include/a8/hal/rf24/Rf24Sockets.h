#pragma once
#include "a8/util/net.h"
#include "a8/hal/rf24/Rf24Socket.h"
#include <RF24.h>
#include <RF24Network.h>
#include <RF24Mesh.h>
#include <RF24Ethernet.h>
namespace a8::hal::nrf24 {

class Rf24Sockets : public Sockets {
private:
    Buffer<Rf24Socket *> *sockets;
    RF24* radio;
    RF24Network * network;
    RF24Mesh * mesh;
    RF24EthernetClass * ethernet;

public:
    Rf24Sockets();
    ~Rf24Sockets();
    bool isReady();
    void close(SOCK &sock) override;

    int connect(SOCK sock, const String host, int port, Result &res) override;

    int bind(SOCK sock, const String address, int port, Result &res) override;

    int listen(SOCK sock, Result &rst) override;

    int accept(SOCK sock, SOCK &sockIn) override;

    bool send(SOCK sock, const char *buf, int len) override;

    int receive(SOCK sock, char *buf, int bufLen) override;

    int getLastError() override;

    int socket(SOCK &sock) override;

    int select(SOCK &sock) override;
    int select(Buffer<SOCK> &buffer1, Buffer<SOCK> &buffer2, Buffer<SOCK> &buffer3) override;
};
} // namespace a8::hal::nrf24
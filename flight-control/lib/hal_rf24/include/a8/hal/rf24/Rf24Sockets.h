#pragma once
#include "a8/hal/rf24/Rf24Hosts.h"
#include "a8/hal/rf24/Rf24Node.h"
#include "a8/hal/rf24/Rf24NetworkWrapper.h"
#include "a8/hal/rf24/Rf24Ports.h"
#include "a8/hal/rf24/Rf24Socks.h"

#include "a8/util/net.h"

class RF24;
template class ESBNetwork<class T>;

namespace a8::hal::nrf24 {
using namespace a8::util;
/**
 * https://nrf24.github.io/RF24/
 * https://nrf24.github.io/RF24Network/
 *
 */

class Rf24Sockets : public Sockets {
private:
    // dynamic
    RF24 *radio;
    Rf24Node *node;
    Rf24NetworkWrapper *network;
    Rf24Ports *ports;
    Rf24Socks *socks;
    Rf24Hosts *hosts;

public:
    Rf24Sockets(Rf24Hosts *hosts, String host, int chipEnablePin, int chipSelectPin);
    ~Rf24Sockets();
    bool isReady();
    int close(SOCK sock) override;

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
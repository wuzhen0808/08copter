#pragma once
#include "a8/hal/rf24/Rf24Hosts.h"
#include "a8/hal/rf24/Rf24NetworkWrapper.h"
#include "a8/hal/rf24/Rf24Node.h"
#include "a8/hal/rf24/Rf24Ports.h"
#include "a8/hal/rf24/Rf24Socks.h"
#include "a8/util/net.h"
#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

namespace a8::hal::rf24 {
using namespace a8::util;
using namespace a8::util::net;
/**
 * https://nrf24.github.io/RF24/
 * https://nrf24.github.io/RF24Network/
 * https://registry.platformio.org/libraries/nrf24/RF24Network/examples
 *
 */

class Rf24Sockets : public Sockets {
private:
    // dynamic
    Rf24Node *node;
    Rf24Ports *ports;
    Rf24Socks *socks;
    Rf24Hosts *hosts;
    String host;

public:
    Rf24Sockets(int id, int chipEnablePin, int chipSelectPin, Rf24Hosts *hosts);
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
} // namespace a8::hal::rf24
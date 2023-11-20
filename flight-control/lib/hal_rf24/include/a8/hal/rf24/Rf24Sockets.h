#pragma once
#include "a8/hal/rf24/Rf24Hosts.h"
#include "a8/hal/rf24/Rf24NetRequest.h"
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

class Rf24Sockets : public Sockets, public FlyWeight {
private:
    // dynamic
    Rf24Node *node;
    Rf24Ports *ports;
    Rf24Socks *socks;
    Rf24Hosts *hosts;
    String host;
    System *sys;
    Scheduler *sch;

public:
    Rf24Sockets(int id, Rf24Hosts *hosts, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, "Rf24Sockets") {
        this->hosts = hosts;
        this->host = host;
        this->ports = new Rf24Ports();
        this->node = new Rf24Node(hosts, id, sys, sch, logFac);
        this->socks = new Rf24Socks(node, hosts, ports, sys, sch, logFac);
        this->sys = sys;
        this->sch = sch;
    }

    ~Rf24Sockets() {
        delete this->socks;
        delete this->node;
        delete this->ports;
    }

    int setup(int chipEnablePin, int chipSelectPin, int channel, Result &res) {
        int ret = this->node->setup(
            chipEnablePin, chipSelectPin, channel, this, [](void *this_, Rf24NetData *&data) {
                Rf24Sockets *this__ = static_cast<Rf24Sockets *>(this_);
                this__->onNetData(data);
            },
            res);
        if (ret < 0) {
            return ret;
        }

        return ret;
    }
    void onNetData(Rf24NetData *&data) {
        Rf24NetRequest *nReq = new Rf24NetRequest(data);
        handleNetRequest(nReq);
        if (!nReq->consumeLater) {
            delete nReq; // consume it
        }
    }
    void handleNetRequest(Rf24NetRequest *nReq) {
        if (nReq->data->node2 != this->node->getId()) {
            log(String() << "no such node id(" << nReq->data->node2 << ") to handle the net data:" << nReq->data);
            nReq->responseCode = -1;
            this->socks->sendResponseIfNecessary(nReq, 0); // not such node.
            return;
        }

        Rf24Sock *s2 = this->socks->findByPort(nReq->data->port2);
        if (s2 == 0) {
            log(String() << "no such port(" << nReq->data->port2 << "@" << nReq->data->node2 << ") to handle net data:" << nReq->data);
            nReq->responseCode = -2;
            this->socks->sendResponseIfNecessary(nReq, 0); // no such port.
            return;
        }
        s2->enqueueNetRequest(nReq);

        // response later.
    }

    int socket(SOCK &sock) {
        Rf24Sock *s = this->socks->create();
        sock = s->getId();
        return 1;
    }

    int close(SOCK sock) {

        return this->socks->close(sock);
    }

    int connect(SOCK sock, const String host2, int port2, Result &res) {
        return socks->connect(sock, host2, port2, res);
    }

    int bind(SOCK sock, const String host, int port, Result &res) {
        return socks->bind(sock, host, port, this->hosts, res);
    }

    int listen(SOCK sock, Result &res) {
        return socks->listen(sock, res);
    }
    /**
     * Accept one good incoming connection, ignore the bad connections if necessary.
     */
    int accept(SOCK sock, SOCK &sockIn, Result &res) {
        int sId2;
        int ret = socks->accept(sock, sId2, res);
        if (ret > 0) {
            sockIn = sId2;
        }
        return ret;
    }

    int send(SOCK sock, const char *buf, int len, Result &res) {
        return socks->send(sock, buf, len, res);
    }

    int receive(SOCK sock, char *buf, int bufLen, Result &res) {
        return socks->receive(sock, buf, bufLen, res);
    }

    int getLastError() {
        return -1;
    }

    int select(SOCK &sock) {
        return -1;
    }
    int select(Buffer<SOCK> &buffer1, Buffer<SOCK> &buffer2, Buffer<SOCK> &buffer3) {
        return -1;
    }
};
} // namespace a8::hal::rf24
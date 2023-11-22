#pragma once
#include "a8/util/rf24/Rf24Hal.h"
#include "a8/util/rf24/Rf24Hosts.h"
#include "a8/util/rf24/Rf24NetRequest.h"
#include "a8/util/rf24/Rf24Node.h"
#include "a8/util/rf24/Rf24Ports.h"
#include "a8/util/rf24/Rf24Sock.h"

#include "a8/util/net.h"


namespace a8::util::rf24 {
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
    Rf24Hal * hal;
    // dynamic
    Rf24Node *node;
    Rf24Ports *ports;
    Rf24Sockets *socks;
    Rf24Hosts *hosts;
    String host;
    System *sys;
    Scheduler *sch;
    int nextId;
    HashTable<int, Rf24Sock *> *table; // socks by id;
    HashTable<int, Rf24Sock *> *binds; // socks by port;

    Rf24Sock *create() {
        int sId = this->nextId++;
        Rf24Sock *s = new Rf24Sock(sId, sys, sch, this->loggerFactory);
        this->table->set(sId, s);
        return s;
    }
    int doBindRandom(Rf24Sock *s, int &port, Result &res) {
        int port1 = ports->randomPort();
        int ok = this->doBind(s, port1, res);
        if (ok < 0) {
            ports->release(port1);
            return -2;
        }
        port = port1;
        return 1;
    }
    int doBind(Rf24Sock *s, int port, Result &res) {

        Rf24Sock *s2 = this->binds->get(port, 0);
        if (s2 != 0) {
            res << "cannot bind to port:" << port << ",port is already bond by sock:" << s2->getId();
            return -4;
        }
        Rf24Watcher *player;
        int ret = s->getPlayer<Rf24Watcher>(Role::Watcher, player, res);
        if (ret < 0) {
            return -5;
        }
        ret = player->doBind(node, port, res);
        if (ret < 0) {
            return -6;
        }
        this->binds->set(port, s);
        return 1;
    }
    Rf24Sock *findByPort(int port) {
        return this->binds->get(port, 0);
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
            this->sendResponseIfNecessary(nReq, 0); // not such node.
            return;
        }

        Rf24Sock *s2 = this->findByPort(nReq->data->port2);
        if (s2 == 0) {
            log(String() << "no such port(" << nReq->data->port2 << "@" << nReq->data->node2 << ") to handle net data:" << nReq->data);
            nReq->responseCode = -2;
            this->sendResponseIfNecessary(nReq, 0); // no such port.
            return;
        }
        s2->getPlayer()->enqueueNetRequest(nReq);

        // response later.
    }

    void destroy(Rf24Sock *s) {
        int port = s->getPort();

        if (port > 0) {
            s->unBind();
            this->ports->release(port);
        }

        s->close();

        delete s;
    }

    Rf24Sock *get(int id, Result &res) {
        Rf24Sock *sock = this->get(id);
        if (sock == 0) {
            res << "no such sock id:" << id;
        }
        return sock;
    }
    Rf24Sock *get(int id) {
        return table->get(id, 0);
    }

    int doAccept(Rf24NetRequest *req, Rf24Sock *&sockIn, Result &res) {
        Rf24Sock *s2 = this->create();
        // bind
        int port22;
        int ret = this->doBindRandom(s2, port22, res); // bond new sock to the new random port.
        if (ret < 0) {
            this->close(s2->getId());
            res << ("failed to accept connection request, cannot bind, detail:" << res.errorMessage);
            req->responseCode = -1;
            return -1;
        }
        // change role
        Rf24Worker *player2;
        ret = s2->changeRole(Role::Worker, player2, res);
        if (ret < 0) {
            return ret;
        }

        player2->connectIn(req->data->node1, req->data->port1, res);
        if (ret < 0) {
            // connect failed.
            req->responseCode = -2;
            return -2;
        }
        req->responseCode = 1;
        sockIn = s2;
        return 1;
    }

    void sendResponseIfNecessary(Rf24NetRequest *nReq, int port1) {

        if (nReq->responseType == 0) {
            // no response type.
            log(String() << "no need to send response(" << nReq->responseCode << "), no response type for net data:" << nReq->data);
            return;
        }

        Result res2;
        int ret2 = this->node->send(nReq->responseType, port1, nReq->data->node1, nReq->data->port1, nReq->responseCode, 0, 0, res2);
        if (ret2 < 0) {
            log(String() << "failed to send response(" << nReq->responseCode << ") for data:" << nReq->data);
        }
    }

public:
    Rf24Sockets(Rf24Hal * hal, int id, Rf24Hosts *hosts, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, "Rf24Sockets") {
        this->hal = hal;
        this->hosts = hosts;
        this->host = host;
        this->sys = sys;
        this->sch = sch;
        this->nextId = 1;
        this->ports = new Rf24Ports();
        this->node = new Rf24Node(hal, hosts, id, sys, sch, logFac);
        this->table = new HashTable<int, Rf24Sock *>([](int k) { return k % 32; });
        this->binds = new HashTable<int, Rf24Sock *>([](int k) { return k % 16; });
    }

    ~Rf24Sockets() {
        delete this->socks;
        delete this->node;
        delete this->ports;
        delete this->binds;
        this->table->forEach<int>(0, [](int c, int k, Rf24Sock *v) {
            delete v;
        });
        delete this->table;
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

    int socket(SOCK &sock) override {
        Rf24Sock *s = this->create();
        sock = s->getId();
        return 1;
    }

    int close(SOCK sock) override {
        int sId = sock;
        int len = table->remove<Rf24Sockets *>(this, sId, [](Rf24Sockets *this_, int k, Rf24Sock *v) {
            this_->destroy(v);
        });
        return 1;
    }

    int connect(SOCK sock, const String host2, int port2, Result &res) override {
        int sId = sock;
        Rf24Sock *s = this->get(sId, res);
        if (s == 0) {
            return -1;
        }
        // bind if needed.
        if (!s->isBond()) {
            int port1;
            int ret = this->doBindRandom(s, port1, res);
            if (ret < 0) {
                return -2;
            }
        }
        // role transform if needed.
        Rf24Connector *player;
        int ret = s->asRole(Role::Connector, player, res);
        if (ret < 0) {
            return ret;
        }
        // resolve remote node.
        int nodeId2 = -1;
        ret = hosts->resolveNodeId(host2, nodeId2, res);
        if (ret < 0) {
            return -1;
        }

        return player->connect(nodeId2, port2, res);
    }

    int bind(SOCK sock, const String host, int port, Result &res) override {

        int sId = sock;
        Rf24Sock *s = this->get(sId, res);
        if (s == 0) {
            return -1; // no such sock
        }

        int nodeId = -1;
        int ret = hosts->resolveNodeId(host, nodeId, res);
        if (ret < 0) {
            return -2; // no such host.
        }

        if (nodeId != this->node->getId()) {
            res << "cannot bind to remote node:" << nodeId << ",only local node(" << this->node->getId() << ") can be bind to.";
            return -3; // host node is not local node.
        }
        return this->doBind(s, port, res);
    }

    int listen(SOCK sock, Result &res) override {
        int sId = sock;
        Rf24Sock *s = this->get(sId, res);
        if (s == 0) {
            return -1; //
        }
        Rf24Listener *listener;
        int ret = s->changeRole<Rf24Listener>(Role::Listener, listener, res);
        if (ret < 0) {
            return ret;
        }
        ret = listener->listen(res);
        if (ret < 0) {
            return ret;
        }
        return ret;
    }
    /**
     * Accept one good incoming connection, ignore the bad connections if necessary.
     */
    int accept(SOCK sock, SOCK &sockIn, Result &res) override {

        int sId = sock;
        Rf24Sock *s = this->get(sId, res);
        if (s == 0) {
            return -1; //
        }
        Rf24Listener *player;
        int ret = s->getPlayer<Rf24Listener>(Role::Listener, player, res);
        if (ret < 0) {
            return ret;
        }

        Rf24Sock *s2 = 0;
        long timeout = -1; // no timeout?
        ret = player->consumeByType<Rf24Sockets *, Rf24Sock *&, Result &, int>(
            Rf24NetData::TYPE_CONNECT_REQUEST, //
            this, s2, res, timeout,
            [](Rf24Sockets *this_, Rf24Sock *&s2, Result &res, Rf24NetRequest *req) {
                int ret = this_->doAccept(req, s2, res);
                int port1 = req->data->port2; //
                if (ret > 0) {
                    port1 = s2->getPort(); // the new port ready for client to communicate.
                }

                this_->sendResponseIfNecessary(req, port1);
                return ret;
            });

        if (ret < 0) {
            return ret;
        }
        sockIn = s2->getId();
        return ret;
    }

    int send(SOCK sock, const char *buf, int len, Result &res) override {

        int sId = sock;
        Rf24Sock *s = this->get(sId, res);
        if (s == 0) {
            return -1; //
        }

        return s->getPlayer()->send(buf, len, res);
    }

    int receive(SOCK sock, char *buf, int len, Result &res) override {

        int sId = sock;
        Rf24Sock *s = this->get(sId, res);
        if (s == 0) {
            return -1; //
        }

        int ret = s->getPlayer()->receive<Rf24Sockets *>(
            buf, len, this, //
            [](Rf24Sockets *this_, Rf24NetRequest *nReq) {
                this_->sendResponseIfNecessary(nReq, nReq->port);
            },
            res);

        return ret;
    }

    int getLastError() override {
        return -1;
    }

    int select(SOCK &sock) override {
        return -1;
    }
    int select(Buffer<SOCK> &buffer1, Buffer<SOCK> &buffer2, Buffer<SOCK> &buffer3) override {
        return -1;
    }
};
} // namespace a8::util::rf24
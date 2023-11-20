#pragma once
#include "a8/hal/rf24/Rf24Hosts.h"
#include "a8/hal/rf24/Rf24NetRequest.h"
#include "a8/hal/rf24/Rf24Ports.h"
#include "a8/hal/rf24/Rf24Sock.h"

#include "a8/util/net.h"

namespace a8::hal::rf24 {

class Rf24Socks : public FlyWeight {
    int nextId;
    HashTable<int, Rf24Sock *> *table; // socks by id;
    HashTable<int, Rf24Sock *> *binds; // socks by port;
    Rf24Node *node;
    Rf24Hosts *hosts;
    Rf24Ports *ports;
    System *sys;
    Scheduler *sch;

public:
    Rf24Socks(Rf24Node *node, Rf24Hosts *hosts, Rf24Ports *ports, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, "Rf24Socks") {
        this->node = node;
        this->hosts = hosts;
        this->ports = ports;
        this->nextId = 1;
        this->sys = sys;
        this->sch = sch;
        this->table = new HashTable<int, Rf24Sock *>([](int k) { return k % 32; });
        this->binds = new HashTable<int, Rf24Sock *>([](int k) { return k % 16; });
    }

    ~Rf24Socks() {
        this->table->forEach<int>(0, [](int c, int k, Rf24Sock *v) {
            delete v;
        });
        delete this->table;
        delete this->binds;
    }

    Rf24Sock *create() {
        int sId = this->nextId++;
        Rf24Sock *s = new Rf24Sock(sId, sys, sch, this->loggerFactory);
        this->table->set(sId, s);
        return s;
    }

    int doBind(Rf24Sock *s, int port, Result &res) {

        Rf24Sock *s2 = this->binds->get(port, 0);
        if (s2 != 0) {
            res << "cannot bind to port:" << port << ",port is already bond by sock:" << s2->getId();
            return -4;
        }
        if (s->doBind(node, port, res) < 0) {
            return -5;
        }
        this->binds->set(port, s);
        return 1;
    }

    int close(int id) {
        int len = table->remove<Rf24Socks *>(this, id, [](Rf24Socks *this_, int k, Rf24Sock *v) {
            this_->destroy(v);
        });
        return 1;
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

    int connect(int sId, String host2, int port2, Result &res) {

        Rf24Sock *s = this->get(sId, res);
        if (s == 0) {
            return -1;
        }

        if (s->getPort() == 0) {
            int port1 = ports->randomPort();
            int ok = this->doBind(s, port1, res);
            if (ok < 0) {
                ports->release(port1);
                return -2;
            }
        }

        int nodeId2 = -1;
        int ret = hosts->resolveNodeId(host2, nodeId2, res);
        if (ret < 0) {
            return -1;
        }

        return s->connect(nodeId2, port2, res);
    }

    int bind(int sId, String host, int port, Rf24Hosts *hosts, Result &res) {
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

    int listen(int sId, Result &res) {
        Rf24Sock *s = this->get(sId, res);
        if (s == 0) {
            return -1; //
        }
        return s->listen(res);
    }

    int accept(int sock, int &sockIn, Result &res) {
        Rf24Sock *s = this->get(sock, res);
        if (s == 0) {
            return -1; //
        }
        Rf24Sock *s2 = 0;
        int ret = s->consumeByType<Rf24Socks *, Rf24Sock *&, Result &, int>(
            Rf24NetData::TYPE_CONNECT_REQUEST, //
            this, s2, res,
            [](Rf24Socks *this_, Rf24Sock *&s2, Result &res, Rf24NetRequest *req) {
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

    int doAccept(Rf24NetRequest *req, Rf24Sock *&sockIn, Result &res) {
        Rf24Sock *s2 = this->create();
        int port22 = this->ports->randomPort();
        int ret = this->doBind(s2, port22, res); // bond new sock to the new random port.
        if (ret < 0) {
            this->close(s2->getId());
            res << ("failed to accept connection request, cannot bind, detail:" << res.errorMessage);
            req->responseCode = -1;
            return -1;
        }

        ret = s2->connectIn(req->data->node1, req->data->port1, res);
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

    int send(int sId, const char *buf, int len, Result &res) {
        Rf24Sock *s = this->get(sId, res);
        if (s == 0) {
            return -1; //
        }
        return s->send(buf, len, res);
    }

    int receive(int sId, char *buf, int len, Result &res) {
        Rf24Sock *s = this->get(sId, res);
        if (s == 0) {
            return -1; //
        }

        int ret = s->receive<Rf24Socks *>(
            buf, len, this, //
            [](Rf24Socks *this_, Rf24NetRequest *nReq) {
                this_->sendResponseIfNecessary(nReq, nReq->port);
            },
            res);

        return ret;
    }

    Rf24Sock *findByPort(int port) {
        return this->binds->get(port, 0);
    }
};

} // namespace a8::hal::rf24
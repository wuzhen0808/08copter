#include "a8/hal/rf24/Rf24Sockets.h"
#include "a8/hal/rf24/Rf24ConnectResponse.h"
#include "a8/hal/rf24/Rf24Sock.h"

#include <RF24.h>
#include <RF24Network.h>

namespace a8::hal::rf24 {

Rf24Sockets::Rf24Sockets(int id, Rf24Hosts *hosts, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac) {
    this->hosts = hosts;
    this->host = host;
    this->ports = new Rf24Ports();
    this->node = new Rf24Node(hosts, id, sys, logFac);
    this->socks = new Rf24Socks(node, hosts, ports, logFac);
    this->sys = sys;
    this->sch = sch;
}

Rf24Sockets::~Rf24Sockets() {
    delete this->socks;
    delete this->node;
    delete this->ports;
}

int Rf24Sockets::setup(int chipEnablePin, int chipSelectPin, int channel, Result &res) {
    int ret = this->node->setup(chipEnablePin, chipSelectPin, channel, res);
    this->sch->schedule(this, [](void *this_) {
        Rf24Sockets *this__ = static_cast<Rf24Sockets *>(this_);
        this__->run();
    });
    return ret;
}
/**
 * Receiving data from network.
 */
void Rf24Sockets::run() {

    while (true) {
        Result res;
        int ret = this->node->receive<Rf24Sockets *>(
            this, [](Rf24Sockets *this_, Rf24NodeData *data) {
                this_->onData(data);
            },
            1000, res);

        if (ret == 0) { // timeout
            continue;
        }
        if (ret < 0) {
            log("failed to receive data from network.");
            break;
        }
    }
    log("RF24 sockets work thread exited!");
}

void Rf24Sockets::onData(Rf24NodeData *data) {
    switch (data->type) {
    case Rf24NodeData::TYPE_Rf24ConnectRequest:
        handleConnectRequest(data->connectionRequest);
        break;
    case Rf24NodeData::TYPE_Rf24ConnectResponse:
        handleConnectResponse(data->connectionResponse);
        break;
    case Rf24NodeData::TYPE_Rf24UserData:
        handleUserData(data->userData);
        break;
    default:
        log(String() << "unknown data type:" << data->type);
    }
}

void Rf24Sockets::handleConnectRequest(Rf24ConnectRequest *req) {
    log(String() << "handleConnectRequest," << req->node1 + ":" << req->port1);
    if (req->node2 != this->node->getId()) {
        // ignore
        log("ignore the connection request for target node id is not match.");
        return;
    }
    Rf24Sock *s = this->socks->findByPort(req->port2);
    if (s == 0) {
        // ignore?
        // todo send a response.
        log("ignore the connection request for port not yet bond.");
        return;
    }

    SyncQueue *queue = s->getConnectionRequestQueue();

    int ret = queue->offer(req, 1000);
    if (ret < 0) {
        log("cannot receive more connect request.");
        // todo send a response.
        return;
    }
}
void Rf24Sockets::handleConnectResponse(Rf24ConnectResponse *resp) {
    log(String() << "handleConnectResponse," << resp->node1 + ":" << resp->port1);
    if (resp->node2 != this->node->getId()) {
        // ignore
        log("ignore the connection response for target node id is not match.");
        return;
    }
    Rf24Sock *s = this->socks->findByPort(resp->port2);
    if (s == 0) {
        // ignore
        log("ignore the connection response for port not yet bond.");
        return;
    }

    SyncQueue *queue = s->getConnectionResponseQueue();

    int ret = queue->offer(resp, 1000);
    if (ret < 0) {
        log("failed offer connect response to the queue of sock.");
        // todo send a response.
        return;
    }
}

void Rf24Sockets::handleUserData(Rf24UserData *userData) {
}

int Rf24Sockets::socket(SOCK &sock) {
    int id = this->socks->create(this->sys, this->sch);
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
/**
 * Accept one good incoming connection, ignore the bad connections if necessary.
 */
int Rf24Sockets::accept(SOCK sock, SOCK &sockIn) {
    log(String() << "accepting... sock:" << sock);
    Rf24Sock *s = this->socks->get(sock);
    if (s == 0) {
        // no such sock.
        log(String() << "fail to accept, no such sock:" << sock);
        return -1; //
    }

    
    SyncQueue *queue = s->getConnectionRequestQueue();
    while (true) {
        log("taking request from queue.");
        Rf24ConnectRequest *req = queue->take<Rf24ConnectRequest>(1000);
        log("taken request from queue.");
        if (req == 0) {
            log("timeout and continue to wait a connect request.");
            continue;
        }
        log("handle connection req.");
        SOCK sId2 = 0;
        int ret = this->socket(sId2);
        if (ret < 0) {
            // ignore?
            log("failed to accept a connection request, because cannot create new sock, ignore and continue.");
            continue;
        }

        Rf24Sock *s2 = this->socks->get(sId2);
        if (s2 == 0) {
            log("bug");
        }
        int port22 = this->ports->randomPort();

        Result res;
        ret = socks->doBind(s2, port22, res);
        if (ret < 0) {
            log(res.errorMessage);
            log("failed to accept connection request, cannot bind.");
            continue;
        }
        log("accept a connection request and sending response to client.");
        // send response to client.
        Rf24ConnectResponse *resp = new Rf24ConnectResponse();
        resp->node1 = this->node->getId();
        resp->port1 = port22;
        resp->node2 = req->node1;
        resp->port2 = req->port1;
        Rf24NodeData data;
        data.type = Rf24NodeData::TYPE_Rf24ConnectResponse;
        data.connectionResponse = resp;
        this->node->send(req->node1, &data, res);
        sockIn = sId2;
        break;
    }
    return 1;
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
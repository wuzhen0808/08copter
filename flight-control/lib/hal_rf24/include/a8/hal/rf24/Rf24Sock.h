#pragma once
#include "a8/hal/rf24/Rf24ConnectRequest.h"
#include "a8/hal/rf24/Rf24Node.h"
#include "a8/util.h"

#include "a8/util/net.h"

namespace a8::hal::rf24 {
using namespace a8::util;
enum Role {
    Unknown,
    Connector,
    Listener,
    Worker
};

enum Status {
    Idle,
    Connecting,
    Connected,
    Listening,
    Working
};
class Rf24Sock : FlyWeight {
    Role role;
    Status status;
    int id;         // sock id
    Rf24Node *node; // local node
    int port;       // local port
    //
    int node2; // remote node
    int port2; // remote port
    //
    Queue<char> *queue;
    System *sys;

public:
    Rf24Sock(int id, System *sys, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->role = Role::Unknown;
        this->sys = sys;
        this->id = id;
        this->node = 0;
        this->port = 0;
        this->queue = new Queue<char>();
    }
    ~Rf24Sock() {
        delete this->queue;
    }

    /**
     * Two use case:
     * 1) before connect as a client.
     * 2) before listen as a listener.
     */
    int bind(Rf24Node *node, int port, Result &res) {
        if (this->node != 0) {
            res << "cannot rebind, sock is already bind to node/port:" << node->getId() << "/" << port << "";
            return -1; // rebind not supported.
        }
        this->node = node;
        this->port = port;
        return 1;
    }
    /**
     * local port bond to.
     */
    int getPort() {
        return this->port;
    }
    /**
     * Remote node/port connect to.
     */
    int connect(int node2, int port2, Result &res) {
        log(">>Rf24Sock::connect");
        if (this->role == Role::Unknown) {
            this->role = Role::Connector;
            this->status = Status::Idle;
        }

        if (this->role != Role::Connector) {
            res << "cannot connect, because role(" << this->role << ") is not expected:" << Role::Connector;
            return -1;
        }

        if (this->status != Status::Idle) {
            res << "cannot connect, because status(" << this->status << ") is not expected:" << Status::Idle;
            return -2;
        }
        this->status = Status::Connecting;
        this->port2 = port2;

        // TODO send message to remote node and port
        Rf24ConnectRequest req;
        req.node1 = this->node->getId();
        req.port1 = this->port;
        WriterReaderBuffer wrb;
        Rf24NodeData data;
        data.type = Rf24NodeData::TYPE_Rf24ConnectRequest;
        data.connectionRequest = new Rf24ConnectRequest();
        data.connectionRequest->node1 = this->node->getId();
        data.connectionRequest->port1 = this->port;
        int ret = this->node->send(node2, &data, res);
        if (ret < 0) {
            this->status = Idle;
            return ret;
        }
        ret = 0;
        long timeout = 3000;
        long startTime = sys->getSteadyTime();

        for (int i = 0;; i++) {
            // TODO timeout.
            if (i > 0) {

                long now = sys->getSteadyTime();
                if (now - startTime > timeout) {
                    res << "timeout to wait the connection response.";
                    ret = -2;
                    break;
                }
            }
            ret = this->node->receive<Rf24Sock *>(        //
                [](Rf24Sock *this_, Rf24NodeData *data) { //
                    return true;
                },       //
                this,    //
                timeout, //
                res      //
            );
        }

        if (ret < 0) {
            return ret;
        }

        this->status = Status::Connected;
        return 1;
    }

    /**
     * Get ready for incoming connect.
     */
    int listen(Result &res) {
        if (this->role != Role::Unknown) {
            res << "cannot listener, because the role(" << role << ") is not expected" << Role::Unknown;
            return -1;
        }
        this->role = Role::Listener;
        this->status = Status::Listening;
        return 1;
    }
    void close() {
        switch (this->role) {
        case Idle:
            break;
        case Connector:
            break;
        case Listener:
            break;
        case Worker:
            break;
        default:
            break;
        }
    }

    int send(const char *buf, int len, Result &res) {
        Rf24NodeData data;
        data.type = Rf24NodeData::TYPE_Rf24UserData;
        data.userData = new Rf24UserData();
        data.userData->sockId = this->id;
        data.userData->buffer->append(buf, len);
        return this->node->send(this->node2, &data, res);
    }
    void handleData(char *buf, int len) {
        queue->append(buf, len);
    }
    bool available() {
        return queue->len() > 0;
    }

    int receive(char *buf, int bufLen, Result &res) {

        int len = this->queue->len();
        if (len == 0) {
            res << "queue is empty, please update on socks and check available before read.";
            return -1; // bug.
        }
        if (len > bufLen) {
            len = bufLen;
        }
        for (int i = 0; i < len; i++) {
            char ch;
            queue->take(ch);
            buf[i] = ch;
        }
        return len;
    }
};

} // namespace a8::hal::rf24
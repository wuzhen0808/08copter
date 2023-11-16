#pragma once

#include "a8/hal/rf24/Rf24ConnectRequest.h"
#include "a8/hal/rf24/Rf24Node.h"
#include "a8/util.h"
#include "a8/util/sched.h"

#include "a8/util/net.h"

namespace a8::hal::rf24 {
using namespace a8::util;
using namespace a8::util::sched;
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
    System *sys;

    // TODO queue of connection requests.
    // TODO use a lock to sync op on connectionRequest.
    Scheduler *sch;
    SyncQueue<Rf24ConnectRequest *> *connectionRequestIncomeQueue;
    SyncQueue<Rf24ConnectResponse *> *connectionResponseIncomeQueue;
    SyncQueue<Rf24UserData *> *userDataIncomeQueue;
    //
    Buffer<char> buffer;

public:
    Rf24Sock(int id, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->role = Role::Unknown;
        this->sys = sys;
        this->id = id;
        this->node = 0;
        this->port = 0;
        this->sch = sch;
        this->connectionRequestIncomeQueue = 0;
        this->connectionResponseIncomeQueue = 0;
        this->userDataIncomeQueue = 0;
    }

    ~Rf24Sock() {
        if (this->connectionRequestIncomeQueue != 0) {
            delete this->connectionRequestIncomeQueue;
        }
        if (this->connectionResponseIncomeQueue != 0) {
            delete this->connectionResponseIncomeQueue;
        }
        if (this->userDataIncomeQueue != 0) {
            delete this->userDataIncomeQueue;
        }
    }
    int getId() {
        return this->id;
    }

    Status getStatus() {
        return this->status;
    }

    SyncQueue<Rf24ConnectRequest *> *getConnectionRequestQueue() {
        return this->connectionRequestIncomeQueue;
    }

    SyncQueue<Rf24ConnectResponse *> *getConnectionResponseQueue() {
        return this->connectionResponseIncomeQueue;
    }

    SyncQueue<Rf24UserData *> *getUserDataQueue() {
        return this->userDataIncomeQueue;
    }

    /**
     * Two use case:
     * 1) before connect as a client.
     * 2) before listen as a listener.
     */
    int doBind(Rf24Node *node, int port, Result &res) {
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

        // for response
        this->connectionResponseIncomeQueue = this->sch->createSyncQueue<Rf24ConnectResponse *>(1);
        // send message to remote node and port
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

        // wait response, timeout in 5 sec.
        Rf24ConnectResponse *resp = this->connectionResponseIncomeQueue->take(5000, 0);
        if (resp == 0) {
            this->status = Status::Idle; // allow reconnect .
            log("failed connect, time out to receive the response from server side.");
        } else {
            this->userDataIncomeQueue = sch->createSyncQueue<Rf24UserData *>(128);
            this->status = Status::Connected;
            log("connect success.");
        }

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
        this->connectionRequestIncomeQueue = sch->createSyncQueue<Rf24ConnectRequest *>(1);
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
    /**
     * send user data
     */
    int send(const char *buf, int len, Result &res) {
        Rf24NodeData data;
        data.type = Rf24NodeData::TYPE_Rf24UserData;
        data.userData = new Rf24UserData();
        data.userData->sockId = this->id;
        data.userData->buffer->append(buf, len);
        return this->node->send(this->node2, &data, res);
    }
    /**
     * receive user data.
     */
    int receive(char *buf, int bufLen, Result &res) {

        while (this->buffer.len() == 0) {
            Rf24UserData *uData = this->userDataIncomeQueue->take(5000, 0);
            if (uData == 0) {
                log("continue to wait the user data from network.");
                continue;
                //
            }
            this->buffer.append(uData->buffer);
            break;
        }
        int len = Math::min(bufLen, this->buffer.len());
        Lang::copy<char>(this->buffer.buffer(), 0, len, buf, 0);
        return len;
    }
};

} // namespace a8::hal::rf24
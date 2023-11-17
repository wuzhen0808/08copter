#pragma once

#include "a8/hal/rf24/Rf24NetData.h"
#include "a8/hal/rf24/Rf24Node.h"
#include "a8/util.h"
#include "a8/util/sched.h"

#include "a8/util/net.h"

namespace a8::hal::rf24 {
using namespace a8::util;
using namespace a8::util::sched;
enum Role {
    Unknown,
    Connector, // Idle,Connecting,Connected.
    Listener,  // Idle,Listening.
    Worker     // Idle,Connected.
};

enum Status {
    Idle,
    Connecting,
    Connected,
    Error,
    Listening // for listner
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
    SyncQueue<Rf24NetData *> *dataQueue;
    //
    Buffer<char> buffer;

    template <typename T>
    static void destroyQueue(SyncQueue<T *> *&queue) {
        SyncQueue<T *> *queue2 = queue;
        queue = 0;
        if (queue2 == 0) {
            return;
        }
        while (true) {
            T *ele = queue2->take(0, 0);
            if (ele == 0) {
                break;
            }
            delete ele;
        }
    }

public:
    Rf24Sock(int id, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, "Rf24Sock") {
        this->role = Role::Unknown;
        this->status = Status::Idle;
        this->sys = sys;
        this->id = id;
        this->node = 0;
        this->port = 0;
        this->sch = sch;
        this->dataQueue = sch->createSyncQueue<Rf24NetData *>(128);
    }

    ~Rf24Sock() {
        destroyQueue<Rf24NetData>(this->dataQueue);
    }

    int getId() {
        return this->id;
    }

    Status getStatus() {
        return this->status;
    }

    void onData(Rf24NetData *data) {
        data = Rf24NetData::copy(data, *new Rf24NetData());
        log(String() << ">>onData:" << data);
        while (true) {
            int ret = this->dataQueue->offer(data, 1000 * 10);
            if (ret < 0) {
                continue;
            }
            break;
        }
        log(String() << "<<onData:" << data);
        return;
    }

    /**
     * Three use case:
     * 1) before connect as a client.
     * 2) before listen as a listener.
     * 3) before connected as a server.
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

    void unBind() {

        this->node = 0;
        this->port = 0;
    }

    /**
     * local port bond to.
     */
    int getPort() {
        return this->port;
    }
    /**
     * Connect to the remote node/port.
     * Sending a message to remote node/port.
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

        // send message to remote node and port

        Rf24NetData data;
        data.type = Rf24NetData::TYPE_CONNECT_REQUEST;
        data.node1 = this->node->getId();
        data.port1 = this->port;
        data.node2 = node2;
        data.port2 = port2;
        int ret = this->node->send(node2, &data, res);
        if (ret < 0) {
            this->status = Idle;
            return ret;
        }

        // wait response, timeout in 5 sec.
        Rf24NetData *resp = this->takeByType(Rf24NetData::TYPE_CONNECT_RESPONSE);
        onResponse(resp);
        delete resp;
        return 1;
    }
    void onResponse(Rf24NetData *resp) {
        log(String() << "got connect response:" << resp);
        if (resp->port1 == 0) {
            log("connect failure.");
            this->status = Idle;
            return;
        }
        this->node2 = resp->node1;
        this->port2 = resp->port1;
        this->status = Status::Connected;
        log("connect success.");
    }
    int connectIn(int node2, int port2, Result &res) {
        if (this->role == Role::Unknown) {
            this->role = Role::Worker;
        }
        if (this->role != Role::Worker) {
            res << "role:" << this->role << " is not expected:" << Role::Worker;
            return -1;
        }
        if (this->status != Status::Idle) {
            res << "status:" << this->status << " is not expected:" << Status::Idle;
            return -1;
        }
        this->node2 = node2;
        this->port2 = port2;
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
        case Role::Connector:
            break;
        case Role::Listener:
            break;
        case Role::Worker:
            break;
        default:
            break;
        }
    }
    /**
     * send user data
     */
    int send(const char *buf, int len, Result &res) {
        if (this->status != Status::Connected) {
            res << "cannot send data, connection is not established or broken.";
            return -1;
        }

        Rf24NetData data;
        data.type = Rf24NetData::TYPE_USER_DATA;
        data.node1 = this->node->getId();
        data.port1 = this->port;
        data.node2 = this->node2;
        data.port2 = this->port2;
        data.buffer->append(buf, len);
        return this->node->send(this->node2, &data, res);
    }
    /**
     * receive user data.
     */
    int receive(char *buf, int bufLen, Result &res) {

        while (this->buffer.len() == 0) {
            if (this->status != Status::Connected) {
                res << "connection not established or already broken.";
                return -1;
            }

            Rf24NetData *uData = this->takeByType(Rf24NetData::TYPE_USER_DATA);
            if (uData == 0) {
                log("continue to wait the user data from network.");
                continue;
                //
            }
            this->buffer.append(uData->buffer);
            delete uData;
            break;
        }

        int len = Math::min(bufLen, this->buffer.len());
        Lang::copy<char>(this->buffer.buffer(), 0, len, buf, 0);

        return len;
    }
    template <typename C>
    void consumeByType(int type, C c, void (*consume)(C c, Rf24NetData *)) {
        Rf24NetData *data = takeByType(type);
        consume(c, data);
        delete data;
    }

    Rf24NetData *takeByType(int type) {
        Rf24NetData *data = 0;
        while (data == 0) {
            data = this->dataQueue->take(1000 * 10, 0);
            if (data->type != type) {
                delete data;
                data = 0;
            }
        }

        return data;
    }
};

} // namespace a8::hal::rf24
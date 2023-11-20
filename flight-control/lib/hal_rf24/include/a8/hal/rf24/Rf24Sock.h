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
    SyncQueue<Rf24NetRequest *> *reqQueue;
    //
    Queue<char> buffer;

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
    Rf24Sock(int id, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, String() << "Rf24Sock(" << id << ")") {
        this->role = Role::Unknown;
        this->status = Status::Idle;
        this->sys = sys;
        this->id = id;
        this->node = 0;
        this->port = 0;
        this->sch = sch;
        this->reqQueue = sch->createSyncQueue<Rf24NetRequest *>(128);
    }

    ~Rf24Sock() {
        this->close();
        destroyQueue<Rf24NetRequest>(this->reqQueue);
    }

    int getId() {
        return this->id;
    }

    Status getStatus() {
        return this->status;
    }

    void enqueueNetRequest(Rf24NetRequest *nReq) {

        log(String() << ">>enqueue net req:" << nReq);
        while (true) {
            int ret = this->reqQueue->offer(nReq, 1000 * 10);
            if (ret < 0) {
                continue;
            }
            nReq->consumeLater = true;
            break;
        }
        log(String() << "<<enqueue net req:" << nReq);
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
        log(String() << ">>connecting to:" << node2 << "/" << port2);
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

        Rf24NetData data(Rf24NetData::TYPE_CONNECT_REQUEST,
                         this->node->getId(), // node1
                         this->port,          // port1
                         node2,               // node2
                         port2                // port2
        );
        int ret = this->node->send(&data, res);
        if (ret < 0) {
            this->status = Idle;
            return ret;
        }
        // todo timeout of response.
        return this->consumeByType<Rf24Sock *, Rf24NetData *, int>(Rf24NetData::TYPE_CONNECT_RESPONSE, this, &data, [](Rf24Sock *this_, Rf24NetData *data, Rf24NetRequest *resp) {
            return this_->handleConnectResponse(data, resp);
        });
    }

    int handleConnectResponse(Rf24NetData *data, Rf24NetRequest *resp) {
        if (resp->data->responseCode < 0) {
            this->status = Status::Idle;
            log(String() << "failed connect to node by data:" << data << ",responseCode:" << resp->data->responseCode);
            return -3;
        }
        this->node2 = resp->data->node1;
        this->port2 = resp->data->port1;
        this->status = Status::Connected;
        return 1;
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
        if (len < 1) {
            //
            res << "cannot send empty buf. ";
            return -1;
        }

        int ret = this->node->send(Rf24NetData::TYPE_USER_DATA,
                                   this->port,
                                   this->node2,
                                   this->port2,
                                   0,   //
                                   buf, //
                                   len, //
                                   res);
        if (ret < 0) {
            return ret;
        }
        Rf24NetRequest *resp = this->takeByType(Rf24NetData::TYPE_USER_DATA_RESPONSE);
        ret = handleUserDataResponse(resp, res);
        delete resp;
        return ret;
    }

    int handleUserDataResponse(Rf24NetRequest *resp, Result &res) {
        int ret = resp->data->responseCode;
        if (ret < 0) {
            res << "failed send data because remote error code:" << ret;
            return -2;
        }
        log(String() << "successfully send out data for the response code is " << resp->data->responseCode);
        return 1;
    }
    int doReceive(Rf24NetRequest *nReq) {
        if (nReq == 0) {
            // bug?
            nReq->responseCode = -1;
            return -1; //
        }
        if (nReq->data->buffer->isEmpty()) {
            // bug?
            nReq->responseCode = -2;
            return -2;
        }
        this->buffer.append(*nReq->data->buffer); // todo use move constructor.
        nReq->responseCode = 1;
        return 1;
    }
    /**
     * receive user data.
     */
    template <typename C>
    int receive(char *buf, int bufLen, C c, void (*response)(C, Rf24NetRequest *), Result &res) {
        log(String() << ">>receive");
        if (this->status != Status::Connected) {
            res << "connection not established or already broken.";
            return -1;
        }

        if (this->buffer.isEmpty()) {

            Rf24NetRequest *nReq = this->takeByType(Rf24NetData::TYPE_USER_DATA);
            int ret = doReceive(nReq);
            response(c, nReq);
            delete nReq; // consume it.
            if (ret < 0) {
                return ret;
            }
        }

        int len = this->buffer.take(buf, bufLen);
        if (len < 0) {
            // bug?
            return -1;
        }
        if (len == 0) {
            // bug?
            return -1;
        }
        log(String() << "received data:" << StringUtil::toHexString(buf, len));
        return len;
    }

    template <typename C, typename C2, typename C3, typename C4, typename R>
    R consumeByType(int type, C c, C2 c2, C3 c3, C4 c4, R (*consume)(C c, C2 c2, C3 c3, C4 c4, Rf24NetRequest *)) {
        struct Params {
            C c;
            C2 c2;
            C3 c3;
            C4 c4;
            R(*consume_)
            (C, C2, C3, C4, Rf24NetRequest *);

            R ret;
            Params(C c, C2 c2, C3 c3, C4 c4) : c(c), c2(c2), c3(c3), c4(c4) {
            }
        } p(c, c2, c3, c4);
        p.consume_ = consume;
        this->consumeByType<Params *>(type, &p, [](Params *pp, Rf24NetRequest *req) {
            pp->ret = pp->consume_(pp->c, pp->c2, pp->c3, pp->c4, req);
        });
        return p.ret;
    }

    template <typename C, typename C2, typename C3, typename R>
    R consumeByType(int type, C c, C2 c2, C3 c3, R (*consume)(C c, C2 c2, C3 c3, Rf24NetRequest *)) {
        struct Params {
            C c;
            C2 c2;
            C3 c3;
            R(*consume_)
            (C, C2, C3, Rf24NetRequest *);

            R ret;
            Params(C c, C2 c2, C3 c3) : c(c), c2(c2), c3(c3) {
            }
        } p(c, c2, c3);
        p.consume_ = consume;
        this->consumeByType<Params *>(type, &p, [](Params *pp, Rf24NetRequest *req) {
            pp->ret = pp->consume_(pp->c, pp->c2, pp->c3, req);
        });
        return p.ret;
    }

    template <typename C, typename C2, typename R>
    R consumeByType(int type, C c, C2 c2, R (*consume)(C c, C2 c2, Rf24NetRequest *)) {
        struct Params {
            C c;
            C2 c2;
            R(*consume_)
            (C, C2, Rf24NetRequest *);

            R ret;
            Params(C c, C2 c2) : c(c), c2(c2) {
            }
        } p(c, c2);
        p.consume_ = consume;
        this->consumeByType<Params *>(type, &p, [](Params *pp, Rf24NetRequest *req) {
            pp->ret = pp->consume_(pp->c, pp->c2, req);
        });
        return p.ret;
    }

    template <typename C>
    void consumeByType(int type, C c, void (*consume)(C c, Rf24NetRequest *)) {
        Rf24NetRequest *req = takeByType(type);
        consume(c, req);
        delete req;
    }

    Rf24NetRequest *takeByType(int type) {
        log(String() << ">>takeByType:" << type);

        Rf24NetRequest *req = 0;
        while (req == 0) {
            log(String() << ">>take, type:" << type);
            req = this->reqQueue->take(1000 * 10, 0);
            log(String() << "<<take:" << req);
            if (req == 0) {
                continue;
            }
            if (req->data->type != type) {
                delete req; // consume here.
                req = 0;
            }
        }
        log(String() << "<<takeByType:" << type << ",data:" << req);
        return req;
    }
};

} // namespace a8::hal::rf24
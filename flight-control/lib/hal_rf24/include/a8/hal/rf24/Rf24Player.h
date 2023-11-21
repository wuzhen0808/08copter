#pragma once

#include "a8/hal/rf24/Rf24NetData.h"
#include "a8/hal/rf24/Rf24NetRequest.h"
#include "a8/hal/rf24/Rf24Node.h"
#include "a8/util.h"
#include "a8/util/sched.h"

#include "a8/util/net.h"

namespace a8::hal::rf24 {
using namespace a8::util;
using namespace a8::util::sched;
enum Role {
    Watcher,
    Listener,  // Idle,Listening.
    Connector, // Idle,Connecting,Connected.
    Worker     // Idle,Connected.
};

class Rf24Player : FlyWeight {
    Role role;

protected:    
    int id;          // sock id
    Rf24Node *&node; // local node
    int &port;       // local port
    //
    int node2; // remote node
    int port2; // remote port
    //
    System *sys;

    // TODO queue of connection requests.
    // TODO use a lock to sync op on connectionRequest.
    Scheduler *sch;
    SyncQueue<Rf24NetRequest *> *reqQueue;

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
    Rf24Player(Role role, int id, Rf24Node *&node, int &port, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, String() << "Rf24Player(" << id << ")"),
                                                                                                                    node(node),
                                                                                                                    port(port) {
        this->role = role;
        this->sys = sys;
        this->id = id;
        this->sch = sch;
        this->reqQueue = sch->createSyncQueue<Rf24NetRequest *>(128);
    }

    ~Rf24Player() {
        this->close();
        destroyQueue<Rf24NetRequest>(this->reqQueue);
    }

    Role getRole() {
        return role;
    }

    bool isRole(Role role) {
        return this->role == role;
    }

    int assertRole(Role role, Result &res) {
        if (this->role != role) {
            res << "Role(" << this->role << ") of sock(" << this->id << ") is not expected:" << role;
            return -1;
        }
        return 1;
    }

    void close() {
    }

    int getId() {
        return this->id;
    }

    void log(const String &msg) {
        FlyWeight::log(msg);
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
     * local port bond to.
     */
    int getPort() {
        return this->port;
    }

    virtual int send(const char *buf, int len, Result &res) {
        res << "role(" << role << ") does not support this operation.";
        return -1;
    };

    template <typename C>
    int receive(char *buf, int bufLen, C c, void (*response)(C, Rf24NetRequest *), Result &res) {
        struct Params {
            C c;
            void (*response)(C, Rf24NetRequest *);
            Params(C c) {
                this->c = c;
            }
        } p(c);
        p.response = response;

        return doReceive(
            buf, bufLen, &p, [](void *pp, Rf24NetRequest *nReq) {
                Params *ppp = static_cast<Params *>(pp);
                ppp->response(ppp->c, nReq);
            },
            res);
    }

    virtual int doReceive(char *buf, int bufLen, void *c, void (*response)(void *, Rf24NetRequest *), Result &res) {
        res << "role(" << role << ") does not support this operation.";
        return -1;
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
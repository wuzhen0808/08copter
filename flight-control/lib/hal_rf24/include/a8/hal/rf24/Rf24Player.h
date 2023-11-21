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
    long defaultTimeout = -1;              // no timeout
    long defaultSendingTimeout = 5 * 1000; //
    long defaultSendingRetries = 3;

protected:
    Queue<char> buffer;

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

    template <typename C, typename C2, typename C3, typename C4, typename R>
    R consumeByType(int type, C c, C2 c2, C3 c3, C4 c4, long timeout, R (*callback)(C c, C2 c2, C3 c3, C4 c4, Rf24NetRequest *)) {
        struct Params {
            C c;
            C2 c2;
            C3 c3;
            C4 c4;
            R(*callback)
            (C, C2, C3, C4, Rf24NetRequest *);

            R ret;
            Params(C c, C2 c2, C3 c3, C4 c4) : c(c), c2(c2), c3(c3), c4(c4) {
            }
        } p(c, c2, c3, c4);
        p.callback = callback;
        this->consumeByType<Params *>(type, &p, timeout, [](Params *pp, Rf24NetRequest *req) {
            pp->ret = pp->callback(pp->c, pp->c2, pp->c3, pp->c4, req);
        });
        return p.ret;
    }

    template <typename C, typename C2, typename C3, typename R>
    R consumeByType(int type, C c, C2 c2, C3 c3, long timeout, R (*callback)(C c, C2 c2, C3 c3, Rf24NetRequest *)) {
        struct Params {
            C c;
            C2 c2;
            C3 c3;
            R(*callback)
            (C, C2, C3, Rf24NetRequest *);

            R ret;
            Params(C c, C2 c2, C3 c3) : c(c), c2(c2), c3(c3) {
            }
        } p(c, c2, c3);
        p.callback = callback;
        this->consumeByType<Params *>(type, &p, timeout, [](Params *pp, Rf24NetRequest *req) {
            pp->ret = pp->callback(pp->c, pp->c2, pp->c3, req);
        });
        return p.ret;
    }

    template <typename C, typename C2, typename R>
    R consumeByType(int type, C c, C2 c2, long timeout, R (*callback)(C c, C2 c2, Rf24NetRequest *)) {
        struct Params {
            C c;
            C2 c2;
            R(*callback)
            (C, C2, Rf24NetRequest *);

            R ret;
            Params(C c, C2 c2) : c(c), c2(c2) {
            }
        } p(c, c2);
        p.callback = callback;
        this->consumeByType<Params *>(type, &p, timeout, [](Params *pp, Rf24NetRequest *req) {
            pp->ret = pp->callback(pp->c, pp->c2, req);
        });
        return p.ret;
    }

    template <typename C>
    void consumeByType(int type, C c, long timeout, void (*callback)(C c, Rf24NetRequest *)) {
        Rf24NetRequest *req = takeByType(type, timeout);
        callback(c, req);
        if (req == 0) {
            // timeout
        } else {
            delete req;
        }
    }

    Rf24NetRequest *takeByType(int type) {
        return takeByType(type, defaultTimeout);
    }

    Rf24NetRequest *takeByType(int type, long timeout) {

        log(String() << ">>takeByType:" << type << ",timeout:" << timeout);
        Rf24NetRequest *req = 0;
        if (timeout < 0) {
            req = this->takeByTypeNoTimeout(type);
        } else {

            long time1 = this->sys->getSteadyTime();
            long timeout2 = timeout;
            for (int i = 0;; i++) {
                if (i > 0) {
                    long now = this->sys->getSteadyTime();
                    timeout2 = timeout - (now - time1);
                }

                if (timeout2 < 0) {
                    timeout2 = 0;
                }
                req = this->reqQueue->take(timeout2, 0);

                if (req != 0 && req->data->type != type) {
                    delete req; // consume here.
                    req = 0;
                }
                if (req != 0) {
                    break;
                }
                if (timeout2 == 0) {
                    break;
                }
            }
        }
        if (req == 0) {
            // log(String() << "<<takeByType(" << type << ") timeout.");
        } else {
            // log(String() << "<<takeByType(" << type << ") success");
        }
        log("<<takeByType");
        return req;
    }

    Rf24NetRequest *takeByTypeNoTimeout(int type) {
        log(String() << ">>takeByTypeNoTimeout:" << type);
        Rf24NetRequest *req = 0;
        while (req == 0) {
            req = this->reqQueue->take(10000, 0);
            if (req == 0) {
                continue;
            }
            if (req->data->type != type) {
                delete req; // consume here.
                req = 0;
            }
        }
        // log(String() << "<<takeByTypeNoTimeout(1" << type << ") success, data:" << req << ".");
        return req;
    }

    int send(const char *buf, int len, Result &res) {
        long timeout = this->defaultSendingTimeout;
        int retries = this->defaultSendingRetries;
        for (int i = 0;; i++) {
            Result resI;
            bool retryAble = false;
            int ret = doSend(buf, len, timeout, retryAble, resI);
            if (ret < 0 && retryAble && i < retries) {
                log(String() << "retry(" << (i + 1) << ") sending.");
                continue;
            }
            if (ret < 0) {
                if (i > 0) {
                    res << "sending failed after retries(" << (i) << ");";
                }
                res << resI.errorMessage;
            }
            return ret;
        }
        return -1;
    }
    /**
     * send user data
     */
    int doSend(const char *buf, int len, long timeout, bool &retryAble, Result &res) {
        if (!this->isConnected()) {
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
            retryAble = true;
            return ret;
        }
        return this->consumeByType<Rf24Player *, bool &, Result &, int>( //
            Rf24NetData::TYPE_USER_DATA_RESPONSE, this, retryAble, res, timeout,
            [](Rf24Player *this_, bool &retryAble, Result &res, Rf24NetRequest *resp) {
                return this_->handleUserDataResponse(resp, retryAble, res);
            });
    }

    int handleUserDataResponse(Rf24NetRequest *resp, bool &retryAble, Result &res) {
        if (resp == 0) {
            res << "timeout to wait the response from remote.";
            retryAble = true;
            return -1;
        }
        int ret = resp->data->responseCode;
        if (ret < 0) {
            res << "failed send data because remote error code:" << ret;
            retryAble = true;
            return -2;
        }
        log(String() << "successfully send out data for the response code is " << resp->data->responseCode);
        return 1;
    }

    virtual bool isConnected() {
        return false;
    }

    /**
     * receive user data.
     */
    int doReceive(char *buf, int bufLen, void *c, void (*response)(void *, Rf24NetRequest *), Result &res) {
        log(String() << ">>receive");
        if (!this->isConnected()) {
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
};

} // namespace a8::hal::rf24
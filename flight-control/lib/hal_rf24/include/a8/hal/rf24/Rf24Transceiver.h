#pragma once

#include "a8/hal/rf24/Rf24NetData.h"
#include "a8/hal/rf24/Rf24Player.h"

#include "a8/util.h"
#include "a8/util/sched.h"

#include "a8/util/net.h"

namespace a8::hal::rf24 {
using namespace a8::util;
using namespace a8::util::sched;
class Rf24Transceiver : public Rf24Player {
    //

protected:
    Queue<char> buffer;
    long sendingTimeout = -1; // 5 * 1000; //

protected:
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
    Rf24Transceiver(Role role, int id, Rf24Node *&node, int &port, System *sys, Scheduler *sch, LoggerFactory *logFac) : Rf24Player(role, id, node, port, sys, sch, logFac) {
    }
    ~Rf24Transceiver() {
    }

    /**
     * send user data
     */
    int send(const char *buf, int len, Result &res) override {
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
            return ret;
        }
        return this->consumeByType<Rf24Transceiver *, Result &, int>( //
            Rf24NetData::TYPE_USER_DATA_RESPONSE, this, res, sendingTimeout,
            [](Rf24Transceiver *this_, Result &res, Rf24NetRequest *resp) {
                return this_->handleUserDataResponse(resp, res);
            });
    }

    int handleUserDataResponse(Rf24NetRequest *resp, Result &res) {
        if (resp == 0) {
            res << "timeout to wait the response from remote.";
            return -1;
        }
        int ret = resp->data->responseCode;
        if (ret < 0) {
            res << "failed send data because remote error code:" << ret;
            return -2;
        }
        log(String() << "successfully send out data for the response code is " << resp->data->responseCode);
        return 1;
    }

    /**
     * receive user data.
     */
    virtual bool isConnected() = 0;
    int doReceive(char *buf, int bufLen, void *c, void (*response)(void *, Rf24NetRequest *), Result &res) override {
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
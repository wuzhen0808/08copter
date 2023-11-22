#pragma once

#include "a8/util/rf24/Rf24NetData.h"
#include "a8/util/rf24/Rf24Node.h"
#include "a8/util/rf24/Rf24Player.h"
#include "a8/util.h"
#include "a8/util/sched.h"

#include "a8/util/net.h"

namespace a8::util::rf24 {
using namespace a8::util;
using namespace a8::util::sched;
class Rf24Connector : public Rf24Player {
    enum Status {
        Unknown,
        Connecting,
        Connected,
        Error,
        Listening // for listner
    };
    Status status = Unknown;
    long connectTimeout = 10 * 1000;

public:
    Rf24Connector(int id, Rf24Node *&node, int &port, System *sys, Scheduler *sch, LoggerFactory *logFac) : Rf24Player(Role::Connector, id, node, port, sys, sch, logFac) {
        this->status = Status::Unknown;
    }

    ~Rf24Connector() {
    }

    /**
     * Connect to the remote node/port.
     * Sending a message to remote node/port.
     */
    int connect(int node2, int port2, Result &res) {
        log(String() << ">>connecting to:" << node2 << "/" << port2);

        if (this->status != Status::Unknown) {
            res << "cannot connect, because status(" << this->status << ") is not expected:" << Status::Unknown;
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
            this->status = Unknown;
            return ret;
        }
        // todo timeout of response.
        return this->template consumeByType<Rf24Connector *, Rf24NetData *, Result &, int>(Rf24NetData::TYPE_CONNECT_RESPONSE, this, &data, res, connectTimeout, [](Rf24Connector *this_, Rf24NetData *data, Result &res, Rf24NetRequest *resp) {
            return this_->handleConnectResponse(data, resp, res);
        });
    }

    int handleConnectResponse(Rf24NetData *data, Rf24NetRequest *resp, Result &res) {
        if (resp == 0) {
            res << "timeout to wait the response.";
            this->status = Status::Unknown;//allow reconnect.
            return -1;
        }
        if (resp->data->responseCode < 0) {
            this->status = Status::Unknown;
            res << (String() << "failed connect to node by data:" << data << ",responseCode:" << resp->data->responseCode);
            return -2;
        }
        this->node2 = resp->data->node1;
        this->port2 = resp->data->port1;
        this->status = Status::Connected;
        return 1;
    }

    bool isConnected() override {
        return this->status == Status::Connected;
    }
};

} // namespace a8::util::rf24
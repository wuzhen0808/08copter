#pragma once
#include "a8/hal/rf24/Rf24NetData.h"
#include "a8/util.h"
#include "a8/util/sched.h"

#include <RF24.h>
#include <RF24Network.h>
namespace a8::hal::rf24 {
using namespace a8::util::sched;
using namespace a8::util;

struct SendingTask {
    Rf24NetData *data;
    int node2;
    SyncQueue<int> *ret;
    Result rst;
    SendingTask(int node2, Rf24NetData *data, SyncQueue<int> *ret) {
        this->data = data;
        this->node2 = node2;
        this->ret = ret;
    }
    ~SendingTask() {
    }
};

class Rf24Net : public FlyWeight {
    using nodeDataHandler = void (*)(void *, Rf24NetData *);
    RF24 *radio;
    RF24Network *network;
    int nodeId;
    Scheduler *sch;
    void *nodeDataHandlerContext;
    nodeDataHandler nodeDataHandler_;
    SyncQueue<SendingTask *> *sendingQueue;

public:
    Rf24Net(RF24 *radio, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->radio = radio;
        this->network = new RF24Network(*radio);
        this->sch = sch;
        this->sendingQueue = 0;
    }

    ~Rf24Net() {
        delete this->network;
    }

    void begin(int nodeId, void *context, nodeDataHandler handler) {
        this->nodeId = nodeId;
        this->nodeDataHandlerContext = context;
        this->nodeDataHandler_ = handler;
        this->sendingQueue = sch->createSyncQueue<SendingTask *>(128);

        this->network->begin(nodeId);
        this->sch->createTask<Rf24Net *>(this, [](Rf24Net *this_) {
            this_->runTask();
        });
    }

    void runTask() {
        while (true) {
            this->network->update();
            // sending out, flush queue.
            while (true) {
                SendingTask *st = this->sendingQueue->take(0, 0);
                if (st == 0) {
                    break;
                }
                handleSendingTask(st);
            }
            // receive in
            while (this->network->available()) {
                RF24NetworkHeader header;
                int size = this->network->peek(header);
                if (size == 0) {
                    continue;
                }
                char buf[size];
                int size2 = this->network->read(header, buf, size);
                if (size2 != size) {
                    log(String() << "size of data received is not the expected.");
                }
                Rf24NetData *data = new Rf24NetData();
                WriterReaderBuffer wrb;
                (static_cast<Writer *>(&wrb))->write(buf, size);
                Rf24NetData::read(&wrb, *data);
                this->handleNetData(data);
                delete data;
            }
        }
    }

    int send(int node2, Rf24NetData *data, Result &res) {

        SyncQueue<int> *resultQueue = sch->createSyncQueue<int>(1);
        SendingTask *st = new SendingTask(node2, data, resultQueue);
        log("Rf24Net::send>>");
        int ret = this->send(st, res);
        delete st;
        delete resultQueue;
        log("<<Rf24Net::send");
        return ret;
    }

    int send(SendingTask *st, Result &res) {
        int ret = this->sendingQueue->offer(st, 1000 * 10L);
        if (ret < 0) {
            res << "failed sending data, timeout to offer data to queue.";
            return ret;
        }

        while (true) {
            int ret2 = st->ret->take(ret, 1000 * 10L);
            if (ret2 < 0) {
                log("timeout to take the sending result, retrying.");
                continue;
            }
            if (ret < 0) {
                res << "failed to send, detail:" << st->rst.errorMessage;
            }

            break;
        }

        return ret;
    }
    void handleSendingTask(SendingTask *st) {
        int ret = handleSendingTask(st, st->rst);
        int ret2 = st->ret->offer(ret, 1000 * 10);
        if (ret2 < 0) {
            log("fatal error, failed to offer the sending result to caller.");
        }
    }

    void handleNetData(Rf24NetData *data) {
        this->nodeDataHandler_(this->nodeDataHandlerContext, data);
    }

    int handleSendingTask(SendingTask *st, Result &res) {
        WriterReaderBuffer wrb;
        int ret = Rf24NetData::write(&wrb, *st->data);

        if (ret < 0) {
            res << "failed to encode data to send out.";
            return ret;
        }
        if (st->node2 == this->nodeId) {
            Rf24NetData data2;
            ret = Rf24NetData::read(&wrb, data2);
            if (ret < 0) {
                res << "failed to decode data for local node data.";
                return ret;
            }
            handleNetData(&data2);
            return 1;
        }
        RF24NetworkHeader header(st->node2);
        bool ok = this->network->write(header, wrb.buffer(), wrb.len());
        if (!ok) {
            res << "failed write to network, len:" << wrb.len();
            return -1;
        }
        return 1;
    }
};

} // namespace a8::hal::rf24
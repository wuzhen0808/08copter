#pragma once
#include "a8/hal/rf24/Rf24NodeData.h"
#include "a8/util.h"
#include "a8/util/sched.h"

#include <RF24.h>
#include <RF24Network.h>
namespace a8::hal::rf24 {
using namespace a8::util::sched;
using namespace a8::util;

struct SendingTask {
    Rf24NodeData *data;
    int node2;
    SyncQueue<int> *ret;
    Result rst;
    SendingTask(int node2, Rf24NodeData *data, SyncQueue<int> *ret) {
        this->data = data;
        this->node2 = node2;
        this->ret = ret;
    }
    ~SendingTask() {
    }
};

class Rf24Net : public FlyWeight {
    RF24 *radio;
    RF24Network *network;
    int nodeId;
    Scheduler *sch;
    SyncQueue<Rf24NodeData *> *receivingQueue;
    SyncQueue<SendingTask *> *sendingQueue;

public:
    Rf24Net(RF24 *radio, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->radio = radio;
        this->network = new RF24Network(*radio);
        this->sch = sch;
        this->receivingQueue = 0;
        this->sendingQueue = 0;
    }

    ~Rf24Net() {
        delete this->network;
    }

    template <typename C>
    int receive(C c, void (*consumer)(C c, Rf24NodeData *data), long timeout, Result &res) {
        Rf24NodeData *data = this->receivingQueue->take(timeout, 0);
        if (data == 0) {
            res << "timeout to take data from receiving queue.";
            return -1;
        }
        consumer(c, data);
        delete data;
        return 1;
    }

    void begin(int nodeId) {
        this->nodeId = nodeId;
        this->receivingQueue = sch->createSyncQueue<Rf24NodeData *>(128);
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
                handle(st);
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
                Rf24NodeData *data = new Rf24NodeData();
                WriterReaderBuffer wrb;
                (static_cast<Writer *>(&wrb))->write(buf, size);
                Rf24NodeData::read(&wrb, *data);
                while (true) {
                    int ret = this->receivingQueue->offer(data, 1000 * 10);
                    if (ret < 0) {
                        log("timeout to offer data to queue, retrying.");
                        continue;
                    }
                    break;
                }
            }
        }
    }

    int send(int node2, Rf24NodeData *data, Result &res) {

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
            log(String() << "got the sending result,ret:" << ret);
            if (ret < 0) {
                res << st->rst.errorMessage;
            }

            break;
        }

        return ret;
    }
    void handle(SendingTask *st) {
        int ret = handle(st, st->rst);
        int ret2 = st->ret->offer(ret, 1000 * 10);
        if (ret2 < 0) {
            log("fatal error, failed to offer the sending result to caller.");
        }
    }

    int handle(SendingTask *st, Result &res) {
        if (st->node2 == this->nodeId) {
            int ret = this->receivingQueue->offer(st->data, 1000 * 10);
            if (ret < 0) {
                res << "failed to sending data to local receiving queue.";
                return -1;
            }
            return 1;
        }

        WriterReaderBuffer wrb;
        int ret = Rf24NodeData::write(&wrb, *st->data);

        if (ret < 0) {
            res << "failed to encode data to send out.";
            return ret;
        }
        RF24NetworkHeader header(st->node2);
        bool ok = this->network->write(header, wrb.buffer(), wrb.len());
        if (!ok) {
            log("failed write to network");
            res << "failed write to network, len:" << wrb.len();
            return -1;
        }
        return 1;
    }
};

} // namespace a8::hal::rf24
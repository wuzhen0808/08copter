#pragma once
#include "a8/hal/rf24/Rf24NetData.h"
#include "a8/util.h"
#include "a8/util/sched.h"

#include <RF24.h>
#include <RF24Network.h>
namespace a8::hal::rf24 {
using namespace a8::util::sched;
using namespace a8::util;

class Rf24Net : public FlyWeight {
    using netDataHandler = void (*)(void *, Rf24NetData *);
    RF24 *radio;
    RF24Network *network;
    int nodeId;
    Scheduler *sch;
    void *netDataHandlerContext;
    netDataHandler netDataHandler_;
    Lock *netLock;
    SyncQueue<Rf24NetData *> *localDataQueue;

public:
    Rf24Net(RF24 *radio, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->radio = radio;
        this->network = new RF24Network(*radio);
        this->sch = sch;
        this->netLock = sch->createLock();
        this->localDataQueue = sch->createSyncQueue<Rf24NetData *>(16);
    }

    ~Rf24Net() {
        delete this->network;
    }

    void begin(int nodeId, void *context, netDataHandler handler) {
        this->nodeId = nodeId;
        this->netDataHandlerContext = context;
        this->netDataHandler_ = handler;

        this->network->begin(nodeId);
        this->sch->createTask<Rf24Net *>("Rf24Net.ReceivingTask", this, [](Rf24Net *this_) {
            this_->runReceivingTask();
        });
    }

    void update() {
        this->netLock->runInLock<Rf24Net *>(this, [](Rf24Net *this_) {
            this_->network->update();
        });
    }

    Rf24NetData *doReceiveRadioNet() {
        if (!this->network->available()) {
            return 0;
        }
        RF24NetworkHeader header;
        int size = this->network->peek(header);
        if (size == 0) {
            log(String() << "peek size is 0?");
            return 0;
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

        return data;
    }

    void runReceivingTask() {

        while (true) {
            Rf24NetData *data = this->localDataQueue->take(0, 0);
            if (data == 0) {
                this->netLock->lock();
                data = this->doReceiveRadioNet();
                this->netLock->unLock();
            }

            if (data == 0) {
                continue;
            }

            this->netDataHandler_(this->netDataHandlerContext, data);
            delete data;
        }
    }

    int send(int node2, Rf24NetData *data, Result &res) {

        WriterReaderBuffer wrb;
        int ret = Rf24NetData::write(&wrb, *data);

        if (ret < 0) {
            res << "failed to encode data to send out.";
            return ret;
        }
        if (node2 == this->nodeId) {
            // send to local queue.
            Rf24NetData *data2 = new Rf24NetData();
            ret = Rf24NetData::read(&wrb, *data2);
            if (ret < 0) {
                res << "failed to decode data for local node data.";
                delete data2;
                return ret;
            }
            localDataQueue->offer(data2);
            return 1;
        }
        // send to radio net.
        RF24NetworkHeader header(node2);
        bool ok = this->network->write(header, wrb.buffer(), wrb.len());
        if (!ok) {
            res << "failed write to network, len:" << wrb.len();
            return -1;
        }
        return 1;
    }
};

} // namespace a8::hal::rf24
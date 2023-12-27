#pragma once
#include "a8/util.h"
#include "a8/util/rf24/Rf24Hal.h"
#include "a8/util/rf24/Rf24NetData.h"
#include "a8/util/sched.h"

namespace a8::util::rf24 {
using namespace a8::util::sched;
using namespace a8::util;

class Rf24Net : public FlyWeight {
    using netDataHandler = void (*)(void *, Rf24NetData *&);
    Rf24Hal::Network *network;
    int nodeId;
    Scheduler *sch;
    void *netDataHandlerContext;
    netDataHandler netDataHandler_;
    Lock *netLock;
    SyncQueue<Rf24NetData *> *localDataQueue;

public:
    Rf24Net(Rf24Hal *hal, Rf24Hal::Radio *radio, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, "Rf24Net") {
        this->sch = sch;
        this->network = hal->newNetwork(radio);
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

    template <typename C, typename R>
    R syncNetOp(C c, R (*runner)(C)) {
        return this->netLock->callInLock<C, R>(this, runner);
    }

    template <typename C, typename C1, typename C2, typename C3, typename R>
    R syncNetOp5(C c, C1 c1, C2 c2, C3 c3, R (*runner)(C, C1, C2, C3)) {
        struct Params {
            R(*runner)
            (C, C1, C2, C3);
            C c;
            C1 c1;
            C2 c2;
            C3 c3;
        } p;
        p.runner = runner;
        p.c = c;
        p.c1 = c1;
        p.c2 = c2;
        p.c3 = c3;

        return this->netLock->callInLock<Params *, R>(&p, [](Params *pp) {
            return pp->runner(pp->c, pp->c1, pp->c2, pp->c3);
        });
    }

    Rf24NetData *doReceiveRadioNet() {

        this->network->update();
        if (!this->network->available()) {
            return 0;
        }
        // RF24NetworkHeader header;
        int size = this->network->peek();
        if (size == 0) {
            log(String() << "peek size is 0?");
            return 0;
        }
        char buf[size];
        int size2 = this->network->read(buf, size);
        if (size2 != size) {
            log(String() << "size of data received is not the expected.");
        }
        Rf24NetData *data = 0;
        int ret = Rf24NetData::read(buf, size, data);
        if (ret < 0) {
            return 0;
        }

        return data;
    }

    void runReceivingTask() {
        long loops = 0;
        int preLen = 0;
        long timeout = 0;
        while (true) {
            // log(String() << ">>loops:" << loops);

            int len = doReceiving(loops, timeout);
            if (len == 0) {
                timeout = 1;
            } else {
                timeout = 0;
            }

            loops++;
            // log(String() << "<<loops:" << loops);
        }
    }

    int doReceiving(long loops, long timeout) {
        Rf24NetData *data = 0;
        this->localDataQueue->take(data, timeout);
        if (data == 0) {
            data = this->syncNetOp<Rf24Net *, Rf24NetData *>(this, [](Rf24Net *this_) {
                return this_->doReceiveRadioNet();
            });
        }

        if (data == 0) {
            return 0;
        }

        this->netDataHandler_(this->netDataHandlerContext, data);
        return 1;
    }

    int send(Rf24NetData *data, Result &res) {

        WriterReaderBuffer wrb;
        int ret = Rf24NetData::write(&wrb, *data);

        if (ret < 0) {
            res << "failed to encode data to send out.";
            return ret;
        }
        if (data->node2 == this->nodeId) {
            // send to local queue.
            log(String() << "send to local node:" << this->nodeId);
            Rf24NetData *data2 = 0;
            ret = Rf24NetData::read(&wrb, data2);
            if (ret < 0) {
                res << "failed to decode data for local node data.";
                return ret;
            }

            localDataQueue->offer(data2);
            return 1;
        }

        log(String() << "sending data(" << data << ") to radio net");
        // RF24NetworkHeader header(data->node2);

        ret = this->syncNetOp5<Rf24Net *, int, char *, int, int>(
            this, data->node2, wrb.buffer(), wrb.len(),
            [](Rf24Net *this_, int nodeId2, char *buf, int len) {
                return this_->doSendToRadioNet(nodeId2, buf, len);
            } //
        );

        if (ret < 0) {
            res << "failed write to network, len:" << wrb.len();
            return -1;
        }

        return wrb.len();
    }
    int doSendToRadioNet(int nodeId, char *buf, int len) {
        // send to radio net.
        this->network->update();
        bool ok = this->network->write(nodeId, buf, len);
        if (!ok) {
            return -1;
        }
        return 1;
    }
};

} // namespace a8::util::rf24
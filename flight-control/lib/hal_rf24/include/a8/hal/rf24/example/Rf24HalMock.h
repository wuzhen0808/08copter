#pragma once
#include "a8/util.h"
#include "a8/util/rf24.h"
#include "a8/util/sched.h"
namespace a8::hal::rf24 {
using namespace a8::util::rf24;

class Rf24HalMock : public Rf24Hal, public FlyWeight {
    class RadioMock : public Rf24Hal::Radio {
    public:
        RadioMock(int ce, int cs) {
        }
        ~RadioMock() {
        }
        bool begin() override {
            return true;
        }
        void setChannel(int channel) override {
            // do nothing.
        }
    };
    class NetworkMock : public Rf24Hal::Network {
        int nodeId = -1;
        Queue<Buffer<char>> *queue;
        Lock *lock;
        Scheduler *sch;

    public:
        NetworkMock(Scheduler *sch) {
            this->sch = sch;
            this->lock = sch->createLock();
            this->queue = new Queue<Buffer<char>>();
        }
        ~NetworkMock() {
            delete this->queue;
        }
        void begin(int nodeId) override {
            this->nodeId = nodeId;
        }
        void update() override {
        }

        bool available() override {
            return 0;
        }
        int peek() override {

            return 0;
        }
        int read(char *buf, int bLen) override {
            return this->lock->callInLock<NetworkMock *, char *, int, int>([](NetworkMock *this_, char *buf, int bLen) {
                return this_->doRead(buf, bLen);
            });
        }

        int doRead(char *buf, int bLen) {
            int size = this->peek();
            if (size == 0) {
                return 0;
            }
            Buffer<char> buf2;
            int ret = this->queue->take(buf2);
            if (ret < 0) {
                return ret;
            }
            int size2 = buf2.len();
            if (size2 != size) {
                // return -1;
            }
            if (bLen != size2) {
                return -1;
            }
            int len3 = buf2.write(buf, bLen);
            if (len3 != size2) {
                return -1; // bug/
            }
            return size2;
        }

        int write(int nodeId, char *buf, int bLen) override {
            if (this->nodeId != nodeId) {
                return -1;
            }
            Buffer<char> buf2(buf, bLen);
            return this->lock->callInLock<NetworkMock *, Buffer<char> &>([](NetworkMock *this_, Buffer<char> &buf2) {
                this_->queue->append(buf2);
                return buf2.len();
            });
        }
    };
    Scheduler *sch;

public:
    Rf24HalMock(Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, "Rf24HalMock") {
        this->sch = sch;
    }

    ~Rf24HalMock() {
    }
    Radio *newRadio(int ce, int cs) {

        return new RadioMock(ce, cs);
    }

    Network *newNetwork(Radio *radio) {
        RadioMock *radioMock = static_cast<RadioMock *>(radio);
        return new NetworkMock(this->sch);
    }
};

} // namespace a8::hal::rf24
#pragma once

#include "a8/hal/rf24/Rf24Hosts.h"
#include "a8/hal/rf24/Rf24NodeData.h"
#include "a8/util/net.h"
#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

namespace a8::hal::rf24 {

class Rf24Node : public FlyWeight {
    int id;
    Rf24Hosts *hosts;
    RF24 *radio;
    RF24Network *network;
    System *sys;

public:
    Rf24Node(Rf24Hosts *hosts, int id, System *sys, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->hosts = hosts;
        this->id = id;
        this->sys = sys;
        this->radio = 0;
        this->network = 0;
    }
    ~Rf24Node() {
        Lang::free<RF24Network>(this->network);
        Lang::free<RF24>(this->radio);
    }

    int setup(int chipEnablePin, int chipSelectPin, int channel, Result &res) {
        if (this->radio != 0) {
            res << "cannot setup twice.";
            return -1;
        }
        this->radio = new RF24(chipEnablePin, chipSelectPin);
        this->network = new RF24Network(*radio);

        if (!radio->begin()) {
            res << "Radio hardware not responding!";
            return -1;
        }
        radio->setChannel(channel);
        network->begin(this->id);
        return 1;
    }

    int getId() {
        return this->id;
    }
    int send(int node2, Rf24NodeData *data, Result &res) {
        log("Rf24Node::send>>");
        int ret = doSend(node2, data, res);
        log("<<Rf24Node::send");
        return ret;
    }
    int doSend(int node2, Rf24NodeData *data, Result &res) {
        this->network->update(); // update regularly or here each time to send/receive a message?
        WriterReaderBuffer wrb;
        int ret = Rf24NodeData::write(&wrb, *data);
        if (ret < 0) {
            res << "failed to encode data to send out.";
            return ret;
        }

        RF24NetworkHeader header(node2);
        bool ok = this->network->write(header, wrb.buffer(), wrb.len());
        if (!ok) {
            res << "failed write to network.";
            return -1;
        }
        return 1;
    }

    template <typename C>
    int receive(bool (*consumer)(C c, Rf24NodeData *data), C c, long timeout, Result &res) {
        this->network->update();
        int len = 0;
        long startTime = sys->getSteadyTime();
        for (int i = 0;; i++) {
            if (i > 0) {
                long now = sys->getSteadyTime();
                if (now - startTime > timeout) {
                    res << "timeout to receive any data from newtork.";
                    return -1;
                }
            }
            if (!this->network->available()) {
                if (len == 0) {
                    this->network->update();
                    continue;
                } else {
                    break;
                }
            }

            RF24NetworkHeader header;
            int size = this->network->peek(header);
            if (size == 0) {
                // how to save power
                if (len == 0) {
                    continue;
                } else {
                    break;
                }
            }
            char buf[size];
            int size2 = this->network->read(header, buf, size);
            if (size2 != size) {
                res << "size of data received is not the expected.";
                return -2;
            }
            Rf24NodeData data;
            WriterReaderBuffer wrb;
            (static_cast<Writer *>(&wrb))->write(buf, size);
            Rf24NodeData::read(&wrb, data);
            bool brk = consumer(c, &data);
            len += size;
            if (brk) {
                break;
            }
        }
        return len;
    }
};

} // namespace a8::hal::rf24
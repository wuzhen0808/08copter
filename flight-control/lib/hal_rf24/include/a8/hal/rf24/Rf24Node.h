#pragma once

#include "a8/hal/rf24/Rf24Hosts.h"
#include "a8/hal/rf24/Rf24Net.h"
#include "a8/hal/rf24/Rf24NetData.h"
#include "a8/util/net.h"
#include <RF24.h>
#include <SPI.h>

namespace a8::hal::rf24 {

class Rf24Node : public FlyWeight {
    int id;
    Rf24Hosts *hosts;
    RF24 *radio;
    Rf24Net *net;
    System *sys;
    Scheduler *sch;

public:
    Rf24Node(Rf24Hosts *hosts, int id, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, String() << "Rf24Node(" << id << ")") {
        this->hosts = hosts;
        this->id = id;
        this->sys = sys;
        this->sch = sch;
        this->radio = 0;
        this->net = 0;
    }
    ~Rf24Node() {
        Lang::free<Rf24Net>(this->net);
        Lang::free<RF24>(this->radio);
    }

    int setup(int chipEnablePin, int chipSelectPin, int channel, void *c, void (*nodeDataHandler)(void *, Rf24NetData *), Result &res) {
        if (this->radio != 0) {
            res << "cannot setup twice.";
            return -1;
        }
        this->radio = new RF24(chipEnablePin, chipSelectPin);
        this->net = new Rf24Net(radio, this->sch, this->loggerFactory);

        if (!radio->begin()) {
            res << "Cannot begin radio,"
                << "cePin:" << chipEnablePin << ",csPin:" << chipSelectPin;
            return -1;
        }
        radio->setChannel(channel);
        net->begin(this->id, c, nodeDataHandler);
        return 1;
    }

    int getId() {
        return this->id;
    }
    int send(int node2, Rf24NetData *data, Result &res) {
        log(String() << "send data:" << data);
        int ret = this->net->send(node2, data, res);
        log(String() << "sent data:" << data);
        return ret;
    }
};

} // namespace a8::hal::rf24
#pragma once
#include "a8/util/rf24/Rf24Hal.h"
#include "a8/util/rf24/Rf24Hosts.h"
#include "a8/util/rf24/Rf24Net.h"
#include "a8/util/rf24/Rf24NetData.h"
#include "a8/util/net.h"

namespace a8::util::rf24 {

class Rf24Node : public FlyWeight {
    int id;
    Rf24Hosts *hosts;
    Rf24Hal::Radio *radio;
    Rf24Net *net;
    System *sys;
    Scheduler *sch;
    Rf24Hal * hal;

public:
    Rf24Node(Rf24Hal * hal, Rf24Hosts *hosts, int id, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, String() << "Rf24Node(" << id << ")") {
        this->hal = hal;
        this->hosts = hosts;
        this->id = id;
        this->sys = sys;
        this->sch = sch;
        this->radio = 0;
        this->net = 0;
    }
    ~Rf24Node() {
        Lang::free<Rf24Net>(this->net);
        Lang::free<Rf24Hal::Radio>(this->radio);
    }

    int setup(int chipEnablePin, int chipSelectPin, int channel, void *c, void (*nodeDataHandler)(void *, Rf24NetData *&), Result &res) {
        if (this->radio != 0) {
            res << "cannot setup twice.";
            return -1;
        }
        this->radio = hal->newRadio(chipEnablePin, chipSelectPin);
        this->net = new Rf24Net(hal, radio, this->sch, this->loggerFactory);

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

    int send(const int type, const int port1, const int node2, const int port2, const int responseCode, const char *buf, const int len, Result &res) {
        Rf24NetData data(type, this->id, port1, node2, port2, responseCode, buf, len);
        return send(&data, res);
    }

    int send(Rf24NetData *data, Result &res) {
        log(String() << "send data:" << data);
        int ret = this->net->send(data, res);
        log(String() << "sent data:" << data);
        return ret;
    }

};

} // namespace a8::util::rf24
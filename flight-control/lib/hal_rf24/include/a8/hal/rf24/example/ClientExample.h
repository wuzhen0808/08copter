
#pragma once
#include "a8/hal/rf24.h"
#include "a8/hal/rf24/example/BaseExample.h"

#include "a8/util.h"
#include "a8/util/net.h"

namespace a8::hal::rf24::example {

using namespace a8::util;
using namespace a8::util::net;
using namespace a8::hal::rf24;
using namespace a8;
using a8::util::String;

class ClientTimer : public FlyWeight {

public:
    Sockets *sockets;
    String host;
    int port;
    Scheduler *sch;
    //
    String host2;
    int port2;
    SOCK sock = 0;
    bool connected = false;
    //
    bool autoReConnect = true;
    int counter = 0;
    ClientTimer(Sockets *sockets, String host, int port, Scheduler *sch, String host2, int port2, LoggerFactory *logFac) : FlyWeight(logFac, "ClientTimer") {
        this->sockets = sockets;
        this->host = host;
        this->port = port;
        this->sch = sch;
        //
        this->host2 = host2;
        this->port2 = port2;
    }
    void log(String msg) {
        logger->info(String() << "" << msg);
    }
    void tick() {
        Result res;
        int ret = tick(res);
        if (ret < 0) {
            log(String() << "tick failed; detail:" << res.errorMessage);
        }
    }
    int tick(Result &res) {
        int ret = -1;
        if (sock == 0) {
            SOCK s1;
            ret = this->sockets->socket(s1);
            if (ret < 0) {
                return ret;
            }
            sock = s1;
        }
        if (!connected) {
            log(String() << "connecting to host:" << host2 << ",port:" << port2);
            ret = this->sockets->connect(sock, host2, port2, res);
            if (ret < 0) {
                res << ";failed connect, retry next tick;";
                return ret;
            }
            log(String() << "connected to host:" << host2 << ",port:" << port2);
            this->connected = true;
        }
        // encode the int value .
        WriterReaderBuffer buf;
        CodecUtil::writeInt16(&buf, counter);
        ret = this->sockets->send(sock, buf.buffer(), buf.len(), res);
        if (ret < 0) {
            res << "sending failed.";
            if (autoReConnect) {
                this->sockets->close(sock);
                sock = 0;
                connected = false;
                counter = 0; // reset counter.
                res << "allow reconnect.";
            }
            return ret;
        }
        log(String() << "send out:" << counter);
        counter++;
        return 1;
    }
};
class ClientExample : public BaseExample {
    Rate rate = 1.0f;

public:
    ClientExample(System *sys, LoggerFactory *logFac, Scheduler *sch) : BaseExample("ClientExample", sys, logFac, sch) {
    }

    void setRate(Rate rate) {
        this->rate = rate;
    }

    int start(Result &res) override {
        using a8::util::String;
        int ret = BaseExample::setup(this->clientNode, res);
        if (ret < 0) {
            return ret;
        }

        ClientTimer *rc = new ClientTimer(sockets, client, clientPort, sch, server, serverPort, loggerFactory);

        sch->createTimer<ClientTimer *>("ClientTimer", this->rate, rc, [](ClientTimer *ct) {
            ct->tick();
        });

        logger->info("startSchedule");
        sch->startSchedule();
        return 1;
    }
};
} // namespace a8::hal::rf24::example
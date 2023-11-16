
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

class ClientTimer {

public:
    Sockets *sockets;
    String host;
    int port;
    Logger *logger;
    Scheduler *sch;
    //
    String host2;
    int port2;
    SOCK sock = 0;
    bool connected = false;

    ClientTimer(Sockets *sockets, String host, int port, Logger *logger, Scheduler *sch, String host2, int port2) {
        this->sockets = sockets;
        this->host = host;
        this->port = port;
        this->logger = logger;
        this->sch = sch;
        //
        this->host2 = host2;
        this->port2 = port2;
    }
    void log(String msg) {
        logger->info(msg);
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
        }
        char buf[1] = {'1'};
        ret = this->sockets->send(sock, buf, 1, res);
        if (ret < 0) {
            res << "sending failed.";
            return ret;
        }

        return 1;
    }
};
class ClientExample : public BaseExample {

public:
    ClientExample(System *sys, LoggerFactory *logFac, Scheduler *sch) : BaseExample(sys, logFac, sch) {
    }

    int start(Result &res) {
        using a8::util::String;
        int ret = BaseExample::setup(this->clientNode, res);
        if (ret < 0) {
            return ret;
        }

        ClientTimer *rc = new ClientTimer(sockets, client, clientPort, logger, sch, server, serverPort);

        sch->createTimer<ClientTimer *>(1.0f, rc, [](ClientTimer *ct) {
            ct->tick();
        });

        logger->info("startSchedule");
        sch->startSchedule();
        return 1;
    }
};
} // namespace a8::hal::rf24::example
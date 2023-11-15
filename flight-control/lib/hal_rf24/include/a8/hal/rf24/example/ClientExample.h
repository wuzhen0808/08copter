
#pragma once
#include "a8/hal/rf24.h"
#include "a8/util.h"
#include "a8/util/net.h"

namespace a8::hal::rf24::example {

using namespace a8::util;
using namespace a8::util::net;
using namespace a8::hal::rf24;
using namespace a8;
using a8::util::String;

class TwoWire;
class MyTimer {

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

    MyTimer(Sockets *sockets, String host, int port, Logger *logger, Scheduler *sch, String host2, int port2) {
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
            log(res.errorMessage);
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
            ret = this->sockets->connect(sock, host2, port2, res);
            if (ret < 0) {
                return ret;
            }
        }
        char buf[1] = {'1'};
        ret = this->sockets->send(sock, buf, 1, res);
        if (ret < 0) {
            return ret;
        }

        return 1;
    }
};
class ClientExample {

    System *sys;
    LoggerFactory *logFac;
    Scheduler *sch;

public:
    ClientExample(
        System *sys,
        LoggerFactory *logFac,
        Scheduler *sch) {
        this->sys = sys;
        this->logFac = logFac;
        this->sch = sch;
    }

    int start(Result &res) {
        using a8::util::String;

        Logger *logger = logFac->getLogger();

        String tsHost = "ts";
        int tsNode = 00;
        int tsPort = 1;

        String fcHost = "fc";
        int fcNode = 01;
        int fcPort = 1;

        Rf24Hosts *hosts = new Rf24Hosts();
        hosts->set(tsHost, tsNode);
        hosts->set(fcHost, fcNode);

        Rf24Sockets *sockets = new Rf24Sockets(00, hosts, sys, sch, logFac);

        int ret = sockets->setup(9, 10, 90, res);
        if (ret < 0) {
            delete sockets;
            return -1;
        }
        MyTimer *rc = new MyTimer(sockets, tsHost, tsPort, logger, sch, fcHost, fcPort);

        sch->scheduleTimer(1.0f, rc, [](void *c) {
            MyTimer *mt = static_cast<MyTimer *>(c);
            mt->tick();
        });

        logger->info("startSchedule");
        sch->startSchedule();
        return 1;
    }
};
} // namespace a8::hal::rf24::example
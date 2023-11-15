#pragma once
#include "a8/hal/rf24.h"
#include "a8/util.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"

namespace a8::hal::rf24::example {

using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::schedule;
using namespace a8::hal::rf24;
using namespace a8;
using a8::util::String;

class TwoWire;
class YourTask {
public:
    SOCK sock;
    Sockets *sockets;
    Logger *logger;
    YourTask(SOCK sock, Sockets *sockets, Logger *logger) {
        this->sock = sock;
        this->sockets = sockets;
        this->logger = logger;
    }
    void run() {
        Result res;
        int ret = run(res);
        if (ret < 0) {
            logger->info(res.errorMessage);
        }
        logger->info(String() << "done of your task, sock:" << sock);
    }
    int run(Result &res) {
        while (true) {
            char buf[1];
            int ret = sockets->receive(sock, buf, 1, res);
            if (ret < 0) {
                return ret;
            }
            logger->info(String() << buf[0]);
        }
        return 1;
    }
};
class MyTask {

public:
    Sockets *sockets;
    String host;
    int port;
    Logger *logger;
    Scheduler *sch;
    MyTask(Sockets *sockets, String host, int port, Logger *logger, Scheduler *sch) {
        this->sockets = sockets;
        this->host = host;
        this->port = port;
        this->logger = logger;
        this->sch = sch;
    }
    void log(String msg) {
        logger->info(msg);
    }
    void run() {
        log("MyTask::run()");
        Result res;
        int ret = run(res);
        if (ret < 0) {
            log(res.errorMessage);
        }
        log("done of my task.");
    }

    int run(Result &res) {

        SOCK s1 = 0;
        int ret = sockets->socket(s1);
        if (ret < 0) {
            res << ";failed to got a socket.";
            return ret;
        }
        log(String() << "created sock:" << s1);
        ret = sockets->bind(s1, host, port, res);
        if (ret < 0) {
            res << ";failed bind to " << host << ":" << port;
            return ret;
        }
        log(String() << "bind to host:" << host << ":" << port);
        while (true) {

            SOCK s2 = 0;
            ret = sockets->accept(s1, s2);
            if (ret < 0) {
                res << ";failed to accept connect.";
                return ret;
            }
            log(String() << "accept a new connection with sock:" << s2);
            YourTask *yt = new YourTask(s2, sockets, logger);
            sch->schedule(yt, [](void *c) {
                YourTask *yt = static_cast<YourTask *>(c);
                yt->run();
            });
        }
        return 1;
    }
};
class ServerExample {
public:
    System *sys;
    LoggerFactory *logFac;
    Scheduler *sch;

    ServerExample(System *sys,
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

        Rf24Sockets *sockets = new Rf24Sockets(01, hosts, sys, sch, logFac);

        int ret = sockets->setup(9, 10, 90, res);
        if (ret < 0) {
            res << ";failed to setup sockets on RF24 radio network.";
            delete sockets;
            return -1;
        }
        MyTask *rc = new MyTask(sockets, fcHost, fcPort, logger, sch);
        rc->sockets = sockets;
        sch->schedule(rc, [](void *c) {
            MyTask *mt = static_cast<MyTask *>(c);
            mt->run();
        });

        logger->info("startSchedule");
        sch->startSchedule();
        return 1;
    }
};

} // namespace a8::hal::rf24::example
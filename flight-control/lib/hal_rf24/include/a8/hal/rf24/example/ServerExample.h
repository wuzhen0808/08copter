#pragma once
#include "a8/hal/rf24.h"
#include "a8/hal/rf24/example/BaseExample.h"
#include "a8/util.h"
#include "a8/util/net.h"
namespace a8::hal::rf24::example {

using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::sched;
using namespace a8::hal::rf24;
using namespace a8;
using a8::util::String;

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
class ServerTask {

public:
    Sockets *sockets;
    String host;
    int port;
    Logger *logger;
    Scheduler *sch;
    ServerTask(Sockets *sockets, String host, int port, Logger *logger, Scheduler *sch) {
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
        int ret = doRun(res);
        if (ret < 0) {
            log(res.errorMessage);
        }
        log("done of my task.");
    }

    int doRun(Result &res) {

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
        ret = sockets->listen(s1, res);
        if (ret < 0) {
            res << ";failed listen";
            return ret;
        }

        log(String() << "bind to host:" << host << ":" << port);
        while (true) {

            SOCK s2 = 0;
            ret = sockets->accept(s1, s2, res);
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

class ServerExample : public BaseExample {
public:
    ServerExample(System *sys, LoggerFactory *logFac, Scheduler *sch) : BaseExample(sys, logFac, sch) {
    }
    int start(Result &res) {
        using a8::util::String;
        int ret = BaseExample::setup(this->serverNode, res);
        if (ret < 0) {
            return ret;
        }

        ServerTask *rc = new ServerTask(sockets, server, serverPort, logger, sch);
        rc->sockets = sockets;
        sch->schedule(rc, [](void *c) {
            ServerTask *mt = static_cast<ServerTask *>(c);
            mt->run();
        });

        logger->info("startSchedule");
        sch->startSchedule();
        return 1;
    }
};

} // namespace a8::hal::rf24::example
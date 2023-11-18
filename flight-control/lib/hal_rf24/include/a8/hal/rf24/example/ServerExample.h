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
using namespace a8::util::net;
using a8::util::String;

class WorkerTask : public FlyWeight {

public:
    SOCK sock;
    Sockets *sockets;
    int port;
    WorkerTask(SOCK sock, Sockets *sockets, int port2, LoggerFactory *logFac)
        : FlyWeight(logFac, String() << "WorkerTask-" << sock << "@" << port2) {
        this->sock = sock;
        this->port = port2;
        this->sockets = sockets;
    }

    void run() {
        Result res;
        int ret = run(res);
        if (ret < 0) {
            log(res.errorMessage);
        }
        log(String() << "done of worker task, sock:" << sock << "@" << port);
    }
    int run(Result &res) {
        int cap = 128;
        char buf[cap];
        int len = 0;
        SocketReader sr(sockets, sock);

        for (int i = 0;; i++) {
            int16 counter = -1;
            int ret = CodecUtil::readInt16(&sr, counter);
            if (ret < 0) {
                log(res.errorMessage);
                return ret;
            }
            if (counter != i) {
                log(String() << "missing:" << i);
            }
            log(String() << counter);
        }
        return 1;
    }
};
class ServerTask : public FlyWeight {

public:
    Sockets *sockets;
    String host;
    int port;
    Scheduler *sch;
    ServerTask(Sockets *sockets, String host, int port, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, "ServerTask") {
        this->sockets = sockets;
        this->host = host;
        this->port = port;
        this->sch = sch;
    }

    void run() {
        log("run()");
        Result res;
        int ret = doRun(res);
        if (ret < 0) {
            log(res.errorMessage);
        }
        log("done of task.");
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
            log(String() << "accepted a new connection with sock:" << s2);

            WorkerTask *yt = new WorkerTask(s2, sockets, port, loggerFactory);
            sch->createTask("WokerTask", yt, [](void *c) {
                WorkerTask *yt = static_cast<WorkerTask *>(c);
                yt->run();
            });
        }
        return 1;
    }
};

class ServerExample : public BaseExample {
public:
    ServerExample(System *sys, LoggerFactory *logFac, Scheduler *sch) : BaseExample("ServerExample", sys, logFac, sch) {
    }
    int start(Result &res) {
        using a8::util::String;
        int ret = BaseExample::setup(this->serverNode, res);
        if (ret < 0) {
            return ret;
        }

        ServerTask *rc = new ServerTask(sockets, server, serverPort, sch, loggerFactory);
        rc->sockets = sockets;
        sch->createTask("ServerTask", rc, [](void *c) {
            ServerTask *mt = static_cast<ServerTask *>(c);
            mt->run();
        });

        logger->info("startSchedule");
        sch->startSchedule();
        return 1;
    }
};

} // namespace a8::hal::rf24::example
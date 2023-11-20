#pragma once
#include "a8/hal/rf24.h"
#include "a8/hal/rf24/example/BaseExample.h"
#include "a8/hal/rf24/example/ClientExample.h"
#include "a8/hal/rf24/example/ServerExample.h"
#include "a8/util.h"
#include "a8/util/net.h"
namespace a8::hal::rf24::example {

using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::sched;
using namespace a8::hal::rf24;
using namespace a8;
using a8::util::String;

class BothExample : public BaseExample {
    Rate rate = 1.0f;

public:
    BothExample(System *sys, LoggerFactory *logFac, Scheduler *sch) : BaseExample("BothExample", sys, logFac, sch) {
    }

    void setRate(Rate rate) {
        this->rate;
    }

    int start(Result &res) override {
        log(">>start");
        using a8::util::String;
        if (this->clientNode != this->serverNode) {
            res << "clientNode(" << clientNode << ") must equals to the Server node(" << serverNode << ")";
            return -1;
        }

        int ret = BaseExample::setup(clientNode, res);
        log(">>start.2");
        if (ret < 0) {
            return ret;
        }
        ServerTask *st = new ServerTask(sockets, server, serverPort, sch, loggerFactory);

        log(">>start.3");
        sch->createTask<ServerTask *>("ServerTask", st, [](ServerTask *st) {
            st->run();
        });
        log("waiting for server ready to accept connection.");
        st->waitForReady();
        log("server is ready.");
        ClientTimer *ct = new ClientTimer(sockets, client, clientPort, sch, server, serverPort, loggerFactory);
        //

        sch->createTimer<ClientTimer *>("ClientTimer", rate, ct, [](ClientTimer *ct) {
            ct->tick();
        });

        logger->info("startSchedule");
        sch->startSchedule();
        return 1;
    }
};

} // namespace a8::hal::rf24::example
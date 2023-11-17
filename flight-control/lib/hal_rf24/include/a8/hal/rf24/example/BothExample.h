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
public:
    BothExample(System *sys, LoggerFactory *logFac, Scheduler *sch) : BaseExample("BothExample", sys, logFac, sch) {
    }
    int start(Result &res) {
        using a8::util::String;
        int nodeId = this->clientNode;
        this->serverNode = this->clientNode;

        int ret = BaseExample::setup(nodeId, res);
        if (ret < 0) {
            return ret;
        }
        Logger *log1 = logFac->getLogger("ServerTask");
        ServerTask *st = new ServerTask(sockets, server, serverPort, log1, sch);

        sch->createTask<ServerTask *>(st, [](ServerTask *st) {
            st->run();
        });
        Logger *log2 = logFac->getLogger("ClientTimer");
        ClientTimer *ct = new ClientTimer(sockets, client, clientPort, log2, sch, server, serverPort);

        sch->createTimer<ClientTimer *>(1.0f, ct, [](ClientTimer *ct) {
            ct->tick();
        });

        logger->info("startSchedule");
        sch->startSchedule();
        return 1;
    }
};

} // namespace a8::hal::rf24::example
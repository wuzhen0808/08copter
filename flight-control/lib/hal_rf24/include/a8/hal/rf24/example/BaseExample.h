#pragma once
#include "a8/hal/rf24.h"
#include "a8/util.h"
#include "a8/util/net.h"

namespace a8::hal::rf24::example {

using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::sched;
using namespace a8::hal::rf24;
using namespace a8;
using a8::util::String;

const int defaultClientNode = 00;
const int defaultClientPort = 1;
const int defaultServerNode = 01;
const int defaultServerPort = 1;

class BaseExample {
public:
public:
    String client = "client";
    String server = "server";
    int clientNode = defaultClientNode;
    int clientPort = defaultClientPort;
    int serverNode = defaultServerNode;
    int serverPort = defaultServerPort;

    System *sys;
    LoggerFactory *logFac;
    Scheduler *sch;
    Logger *logger;
    Rf24Hosts *hosts;
    Rf24Sockets *sockets;
    String name;
    BaseExample(String name, System *sys,
                LoggerFactory *logFac,
                Scheduler *sch) {
        this->name = name;
        this->sys = sys;
        this->logFac = logFac;
        this->sch = sch;
    }

    Rf24Hosts *buildHosts() {

        Rf24Hosts *hosts = new Rf24Hosts();
        hosts->set(client, clientNode);
        hosts->set(server, serverNode);
        return hosts;
    }

    int setup(int nodeId, Result &res) {
        using a8::util::String;
        logger = logFac->getLogger(this->name);
        hosts = buildHosts();

        sockets = new Rf24Sockets(nodeId, hosts, sys, sch, logFac);

        int ret = sockets->setup(9, 10, 90, res);
        if (ret < 0) {
            res << ";failed to setup sockets on RF24 radio network.";
            delete sockets;
            return -1;
        }
        return 1;
    }
};

} // namespace a8::hal::rf24::example
#pragma once
#include "a8/hal/rf24.h"
#include "a8/link.h"
#include "a8/util.h"

namespace a8 {
using namespace a8::link;
using namespace a8::util;
using namespace a8::hal::rf24;

class SetupUtil {
public:
    static Links *setupLinks(System *sys, Scheduler *sch, LoggerFactory *logFac, Result &res) {
        String tsHost = "ts";
        int tsNode = 00;
        int tsPort = 1;

        String fcHost = "fc";
        int fcNode = 01;
        int fcPort = 1;

        Rf24Hosts *hosts = new Rf24Hosts();
        hosts->set(tsHost, tsNode);
        hosts->set(fcHost, fcNode);

        Rf24Sockets *sockets = new Rf24Sockets(00, hosts, sys, logFac);

        int ret = sockets->setup(9, 10, 90, res);
        if (ret < 0) {
            return 0;
        }
        Links *links = new Links(sockets, sch, logFac);
        links->ts(tsHost, tsPort);
        links->fc(fcHost, fcPort);
        links->build();
        return links;
    }

};
} // namespace a8
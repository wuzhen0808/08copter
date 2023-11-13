#include "a8/hal.h"
#include "a8/hal/arduino.h"
#include "a8/hal/esp.h"
#include "a8/hal/rf24.h"

#include "a8/link.h"
#include "a8/ts/esp.h"
#include "a8/util.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"

#include <stdio.h>

using namespace a8::hal::arduino_;
using namespace a8::ts;
using namespace a8::ts::esp;
using namespace a8::hal::rf24;
using namespace a8::hal::esp;

class TwoWire;

System *setupSystem() {
    ArduinoSystem *as = new ArduinoSystem();
    return as;
}

Transmitter *setupTs(Scheduler *sch, LoggerFactory *logFac, System *sys, Result &res) {
    using a8::util::String;
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

    int ret = 1;//sockets->setup(7, 8, 90, res);
    if (ret < 0) {
        delete hosts;
        delete sockets;
        return 0;
    }
    Links *links = new Links(sockets, sch, logFac);
    links->ts(tsHost, tsPort);
    links->fc(fcHost, fcPort);
    links->build();

    Transmitter *ts = new EspTransmitter(links);
    return ts;
}

void setup_(ArduinoHal *hal) {

    System *sys = setupSystem();

    Scheduler *scheduler = new FreeRtosScheduler();
    LoggerFactory *logFac = new ArduinoLoggerFactory(sys);
    Logger *log = logFac->getLogger();
    log->info("1.0");

    log->info(a8::util::String() << 1.0f);

    log->info(a8::util::String() << 1.0f);

    Scheduler *sch = new FreeRtosScheduler();
    StagingContext *context = new StagingContext(sch, logFac, sys);
    Result res;
    Transmitter *ts = setupTs(scheduler, logFac, sys, res);
    if (ts == 0) {
        log->error(res.errorMessage);
        return;
    }
    Application::start("appTs", context, ts);
    if (context->isStop()) {
        log->error(context->getMessage());
        log->error("cannot start app.");
        return;
    }
    log->info(">>startSchedule");
    sch->startSchedule();
    log->info("<<startSchedule");
}

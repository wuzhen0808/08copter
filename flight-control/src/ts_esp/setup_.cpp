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
Transmitter *setupTs(ArduinoHal *hal, Scheduler *sch, LoggerFactory *logFac) {
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

    Sockets *sockets = new Rf24Sockets(00, 7, 8, hosts);
    Links *links = new Links(sockets, sch, logFac);
    links->ts(tsHost, tsPort);
    links->fc(fcHost, fcPort);
    links->build();

    Transmitter *ts = new EspTransmitter(hal, links);
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
    Transmitter *ts = setupTs(hal, scheduler, logFac);
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

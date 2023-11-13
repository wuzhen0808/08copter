#include "a8/hal.h"
#include "a8/hal/arduino.h"
#include "a8/hal/esp.h"
#include "a8/hal/rf24.h"

#include "ConfigUtil.h"
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
using namespace a8;
class TwoWire;

Transmitter *setupTs(Scheduler *sch, LoggerFactory *logFac, System *sys, Logger *log, Result &res) {
    Links *links = SetupUtil::setupLinks(sys, sch, logFac, res);
    Transmitter *ts = new EspTransmitter(links);
    return ts;
}

int setup(Result &res) {

    System *sys = new ArduinoSystem();

    Scheduler *scheduler = new FreeRtosScheduler();
    LoggerFactory *logFac = new ArduinoLoggerFactory(sys);
    Logger *log = logFac->getLogger();
    log->info("1.0");

    log->info(a8::util::String() << 1.0f);

    log->info(a8::util::String() << 1.0f);

    Scheduler *sch = new FreeRtosScheduler();
    StagingContext *context = new StagingContext(sch, logFac, sys);
    Transmitter *ts = setupTs(scheduler, logFac, sys, log, res);
    if (ts == 0) {
        log->error(res.errorMessage);
        return -1;
    }
    Application::start("appTs", context, ts);
    if (context->isStop()) {
        log->error(context->getMessage());
        log->error("cannot start app.");
        return -1;
    }
    log->info(">>startSchedule");
    sch->startSchedule();
    log->info("<<startSchedule");
    return 1;
}

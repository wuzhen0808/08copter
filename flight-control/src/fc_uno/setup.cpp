
#include "a8/fc/uno.h"
#include "a8/hal/arduino.h"
#include "a8/hal/freertos.h"
#include "a8/hal/rf24.h"
#include "a8/link.h"
#include "a8/util.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"

#include "ConfigUtil.h"
#include <stdio.h>

using namespace a8::util;
using namespace a8::util::net;
using namespace a8::link;
using namespace a8::hal::arduino_;
using namespace a8::fc;
using namespace a8::fc::uno;
using namespace a8::hal::rf24;
using namespace a8;
using namespace a8::hal::freertos;
class TwoWire;

int setup(Result &res) {
    using a8::util::String;
    System *sys = new ArduinoSystem();

    Scheduler *scheduler = new FreeRtosScheduler();
    LoggerFactory *logFac = new ArduinoLoggerFactory(sys);
    Logger *log = logFac->getLogger();
    log->info("1.0");

    log->info(String() << 1.0f);

    log->info(String() << 1.0f);

    Scheduler *sch = new FreeRtosScheduler();
    StagingContext *context = new StagingContext(sch, logFac, sys);
    Links *links = SetupUtil::setupLinks(sys, sch, logFac, res);
    FlightControl *fc = new UnoFlightControl(links);
    Application::start("appFc", context, fc);
    if (context->isStop()) {
        log->error(context->getMessage());
        log->error("cannot start app.");
        return -1;
    }
    log->info("startSchedule");
    sch->startSchedule();
    return 1;
}

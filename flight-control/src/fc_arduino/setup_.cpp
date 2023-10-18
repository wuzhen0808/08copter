#include "a8/fc/arduino.h"
#include "a8/hal.h"
#include "a8/hal/arduino.h"
#include "a8/hal/freertos.h"
#include "a8/link.h"
#include "a8/util.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"
#include <stdio.h>

using namespace a8::hal::freertos;
using namespace a8::hal::arduino;
using namespace a8::fc::arduino;

System *setupSystem() {
    ArduinoSystem *as = new ArduinoSystem();
    return as;
}
FlightControl *setupFc(Scheduler *sch, LoggerFactory *logFac) {
    Sockets *sockets = new ArduinoSockets();
    Links *links = new Links(sockets, sch, logFac);
    FlightControl *fc = new ArduinoFlightControl(links);
    return fc;
}

void setup_() {

    System *sys = setupSystem();

    Scheduler *scheduler = new FreeRtosScheduler();
    LoggerFactory *logFac = new ArduinoLoggerFactory(sys);
    Logger *log = logFac->getLogger();
    log->info("1.0");
    
    log->info(String() << 1.0f);

    log->info(String() << 1.0f);

    Scheduler *sch = new FreeRtosScheduler();
    StagingContext *context = new StagingContext(sch, logFac, sys);
    FlightControl *fc = setupFc(scheduler, logFac);
    Application::start("appFc", context, fc);
    if (context->isStop()) {
        log->error(context->getMessage());
        log->error("cannot start app.");
        return;
    }
    log->info("startSchedule");
    sch->startSchedule();
}

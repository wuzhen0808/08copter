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
using namespace a8::hal::arduino_;
using namespace a8::fc::arduino_;

class TwoWire;

System *setupSystem() {
    ArduinoSystem *as = new ArduinoSystem();
    return as;
}
FlightControl *setupFc(Hal * hal, Scheduler *sch, LoggerFactory *logFac) {
    Sockets *sockets = new ArduinoSockets();
    Links *links = new Links(sockets, sch, logFac);
    FlightControl *fc = new ArduinoFlightControl(hal, links);
    return fc;
}

void setup__(void *ctx, void (*func)(void *)) {
    Scheduler *sch = new FreeRtosScheduler();
    Rate hz(1.0f);
    sch->scheduleTimer(hz, ctx, func);
    sch->startSchedule();
}

void setup_(Hal * hal) {

    System *sys = setupSystem();

    Scheduler *scheduler = new FreeRtosScheduler();
    LoggerFactory *logFac = new ArduinoLoggerFactory(sys);
    Logger *log = logFac->getLogger();
    log->info("1.0");

    log->info(String() << 1.0f);

    log->info(String() << 1.0f);

    Scheduler *sch = new FreeRtosScheduler();
    StagingContext *context = new StagingContext(sch, logFac, sys);
    FlightControl *fc = setupFc(hal, scheduler, logFac);
    Application::start("appFc", context, fc);
    if (context->isStop()) {
        log->error(context->getMessage());
        log->error("cannot start app.");
        return;
    }
    log->info("startSchedule");
    sch->startSchedule();
}

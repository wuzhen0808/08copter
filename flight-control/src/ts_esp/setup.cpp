#include "a8/hal.h"
#include "a8/hal/arduino.h"
#include "a8/hal/esp.h"
#include "a8/util/rf24.h"

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
using namespace a8::util::rf24;
using namespace a8::hal::esp;
using namespace a8;
class TwoWire;

int setup(Result &res) {

    System *sys = new ArduinoSystem();

    LoggerFactory *logFac = new ArduinoLoggerFactory(sys);
    Logger *log = logFac->getLogger();
    log->info("1.0");

    log->info(a8::util::String() << 1.0f);

    log->info(a8::util::String() << 1.0f);

    Scheduler *sch = new FreeRtosScheduler();
    StagingContext *context = new StagingContext(sch, logFac, sys);

    Links *links = SetupUtil::setupLinks(sys, sch, logFac, res);
    if (links == 0) {
        return -1;
    }
    Transmitter *ts = new EspTransmitter(links);

    Application::start("appTs", context, ts);
    if (context->isStop()) {
        res << context->getMessage();
        return -1;
    }
    log->info(">>startSchedule");
    sch->startSchedule();
    log->info("<<startSchedule");
    return 1;
}

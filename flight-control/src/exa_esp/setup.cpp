#include "a8/hal/arduino.h"
#include "a8/hal/esp.h"
#include "a8/hal/rf24/example/BothExample.h"
#include "a8/hal/rf24/example/ClientExample.h"
#include "a8/hal/rf24/example/ServerExample.h"

#include "a8/util/sched/example/SyncQueueExample.h"

int setup(a8::util::Result &res) {
    using namespace a8::hal::rf24::example;
    using namespace a8::util::sched::example;
    using namespace a8::hal::arduino_;
    using namespace a8::hal::esp;
    using namespace a8::util;
    using namespace a8::util::sched;

    System *sys = new ArduinoSystem();
    LoggerFactory *logFac = new ArduinoLoggerFactory(sys);
    Scheduler *sch = new FreeRtosScheduler();
    // ClientExample *example = new ClientExample(sys, logFac, sch);
    //  ServerExample *example = new ServerExample(sys, logFac, sch);
    BothExample *example = new BothExample(sys, logFac, sch);
    // SyncQueueExample *example = new SyncQueueExample(sys, sch, logFac);

    return example->start(res);
}

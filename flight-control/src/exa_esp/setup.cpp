#include "a8/hal/arduino.h"
#include "a8/hal/esp.h"
#include "a8/hal/rf24/example/BothExample.h"
#include "a8/hal/rf24/example/ClientExample.h"
#include "a8/hal/rf24/example/ExampleRunner.h"
#include "a8/hal/rf24/example/ServerExample.h"

#include "a8/util/sched/example/SyncQueueExample.h"

int setup(a8::util::Result &res) {
    using namespace a8::hal::rf24::example;
    using namespace a8::util::sched::example;
    using namespace a8::hal::arduino_;
    using namespace a8::hal::esp;
    using namespace a8::util;
    using namespace a8::util::sched;
    using a8::util::String;

    System *sys = new EspSystem();
    Scheduler *sch = new FreeRtosScheduler();
    LoggerFactory *logFac = new ArduinoLoggerFactory(sys, sch);
    ExampleRunner *er = new ExampleRunner(sys, sch, logFac);
    return er->start(res);
}

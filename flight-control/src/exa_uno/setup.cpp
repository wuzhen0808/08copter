#include "a8/hal/arduino.h"
#include "a8/hal/uno.h"
#include "a8/hal/freertos.h"
#include "a8/hal/rf24/example/ServerExample.h"
#include "a8/hal/rf24/example/ClientExample.h"

int setup(a8::util::Result &res) {
    using namespace a8::hal::rf24::example;
    using namespace a8::hal::arduino_;
    using namespace a8::hal::freertos;
    using namespace a8::util;
    using namespace a8::util::sched;

    System *sys = new ArduinoSystem();
    LoggerFactory *logFac = new ArduinoLoggerFactory(sys);
    Scheduler *sch = new FreeRtosScheduler();
    ServerExample *example = new ServerExample(sys, logFac, sch);
    //ClientExample *example = new ClientExample(sys, logFac, sch);
    return example->start(res);
}

#include "a8/fc/esp/example/BalanceExample.h"
#include "a8/hal/arduino.h"
#include "a8/hal/esp.h"
#include "a8/hal/rf24.h"
#include "a8/ts.h"
#include "a8/ts/example.h"
#include "a8/util/rf24/example/BothExample.h"
#include "a8/util/rf24/example/ClientExample.h"
#include "a8/util/rf24/example/Runner.h"
#include "a8/util/rf24/example/ServerExample.h"
#include "a8/util/sched/example/SyncQueueExample.h"

int setupRf24(a8::util::Result &res) {
    using namespace a8::util::rf24::example;
    using namespace a8::util::sched::example;
    using namespace a8::hal::arduino_;
    using namespace a8::hal::esp;
    using namespace a8::util;
    using namespace a8::util::sched;
    using namespace a8::hal::rf24;
    using a8::util::String;

    System *sys = new EspSystem();
    Scheduler *sch = new EspScheduler();
    LoggerFactory *logFac = new ArduinoLoggerFactory(sys, sch);

    Rf24Hal *hal = new Rf24HalImpl(logFac);
    Runner *er = new Runner(hal, sys, sch, logFac);
    return er->start(res);
}
int setupJoysticks(a8::util::Result &res) {
    using namespace a8::util::rf24::example;
    using namespace a8::util::sched::example;
    using namespace a8::hal::arduino_;
    using namespace a8::hal::esp;
    using namespace a8::ts;
    using namespace a8::ts::example;
    using namespace a8::util;
    using namespace a8::util::sched;
    using namespace a8::hal::rf24;
    using a8::util::String;

    System *sys = new EspSystem();
    Scheduler *sch = new EspScheduler();
    LoggerFactory *logFac = new ArduinoLoggerFactory(sys, sch);

    JoysticksExample *jse = new JoysticksExample(sys, logFac);

    return jse->start(res);
}
int setupBalance(a8::util::Result &res) {

    using namespace a8::hal::arduino_;
    using namespace a8::hal::esp;
    using namespace a8::util;
    using namespace a8::util::sched;
    using namespace a8::fc::esp::example;
    using namespace a8::fc::esp;

    using a8::util::String;

    System *sys = new EspSystem();
    Scheduler *sch = new EspScheduler();
    LoggerFactory *logFac = new EspLoggerFactory(sys, sch);

    Rpy *rpy = new EspRpy(logFac);

    BalanceExample *example = new BalanceExample(sch, sys, logFac);
    return example->start(res);
}
int setup(a8::util::Result &res) {
    return setupBalance(res);
}
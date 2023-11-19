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
    using a8::util::String;

    System *sys = new EspSystem();
    Scheduler *sch = new FreeRtosScheduler();
    LoggerFactory *logFac = new ArduinoLoggerFactory(sys, sch);
    String str;
    str << sys << "------------\n";
    logFac->getLogger()->info(str);
    BaseExample *example;

    int type = 0;
    while (type < 1 || type > 2) {
        Serial.println("Please select example type, 1:Server, 2:Client, 3:Both.");
        while (!Serial.available()) {
            delay(10);
        }
        char ch = Serial.read();
        type = ch - '0';
        Serial.println("type read:" + type);
    }

    Serial.println("selected type:" + type);
    // ClientExample *example = new ClientExample(sys, logFac, sch);
    //  ServerExample *example = new ServerExample(sys, logFac, sch);
    if (type == 1) {
        example = new ServerExample(sys, logFac, sch);
    } else if (type = 2) {
        example = new ClientExample(sys, logFac, sch);
    } else if (type = 3) {
        example = new BothExample(sys, logFac, sch);
    } else {
        res << "no such type:" << type;
        return -1;
    }

    // SyncQueueExample *example = new SyncQueueExample(sys, sch, logFac);

    return example->start(res);
}

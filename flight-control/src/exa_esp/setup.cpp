// #include "a8/fc/esp/example/BalanceExample.h"
#include "setup.h"
//#include "a8/fc/esp.h"
//#include "a8/fc/esp/example.h"
//#include "a8/hal/esp.h"

// #include "a8/hal/arduino.h"
// #include "a8/hal/esp.h"

/*
#include "a8/hal/rf24.h"
#include "a8/ts.h"
#include "a8/ts/example.h"
#include "a8/util/rf24/example/BothExample.h"
#include "a8/util/rf24/example/ClientExample.h"
#include "a8/util/rf24/example/Runner.h"
#include "a8/util/rf24/example/ServerExample.h"
#include "a8/util/sched/example/SyncQueueExample.h"
*/
// MPU9250 mpu;

// a8::util::System *sys;
// a8::util::LoggerFactory *logFac;
// a8::util::sched::Scheduler *sch;
//a8::fc::esp::example::EspBalanceExample *exa;
// int setupBalanceExample() {

//     using namespace a8::fc::esp::example;
//     exa = new EspBalanceExample(&mpu);

//     Result res;
//     return exa->start(res);
// }

// int setupSystem() {
//     sys = new a8::hal::esp::EspSystem();
//     sch = new a8::hal::esp::EspScheduler();
//     logFac = new a8::hal::esp::EspLoggerFactory(sys, sch);
//     return 1;
// }

void println(const char *msg) {
    Serial.println(msg);
}
void println(const float msg) {
    Serial.println(msg);
}

void delay_(long ms) {
    delay(ms);
}

/*

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
*/
/*
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
*/
/*
void setupSerial() {
    Serial.begin(115200);
    while (Serial.available()) {
        Serial.read();
    }
}
int setupWire() {
    bool ok = Wire.begin(2, 1);
    if (!ok) {
        return -1;
    }
    return 1;
}

int setupMpu9250(MPU9250 &mpu) {
    bool ok = mpu.setup(0x68);
    if (!ok) {
        Serial.println("Failed to setup Mpu9250.");
        return -1;
    }
    mpu.selectFilter(QuatFilterSel::MADGWICK);
    mpu.setFilterIterations(1);
    Serial.println("Success of setup Mpu9250.");
    return 1;
}
*/
/*
int setupBalance(a8::util::Result &res) {

    using namespace a8::hal::arduino_;
    using namespace a8::hal::esp;
    using namespace a8::util;
    using namespace a8::util::sched;
    using namespace a8::fc::esp::example;
    using namespace a8::fc::esp;

    using a8::util::String;

    int ret = setupWire(res);
    if (ret < 0) {
        return ret;
    }

    System *sys = new EspSystem();
    Scheduler *sch = new EspScheduler();
    LoggerFactory *logFac = new EspLoggerFactory(sys, sch);
    Logger *logger = logFac->getLogger();
    Rpy *rpy = new EspRpy(logFac);
    ret = rpy->setup(res);
    if (ret < 0) {
        return ret;
    }
    logger->info("wait some time for mpu9250 ready.");
    delay(3000);
    float r;
    float p;
    float y;
    ret = rpy->getRollPitchYaw(r, p, y);
    if (ret < 0) {
        res << "failed to get rpy.";
        return -1;
    }
    logger->info(String() << "rpy:(" << r << "," << p << "," << y << ")");

    BalanceExample *example = new BalanceExample(rpy, sch, sys, logFac);
    ret = example->start(res);
    return ret;
}
*/
/*
int setup(a8::util::Result &res) {
    int ret = 0;
    // return setupBalance(res);
    return ret;
}
*/
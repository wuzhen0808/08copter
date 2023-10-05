//clang-format off
#include <winsock2.h>
//clang-format on

#include "a8/hal.h"
#include "a8/hal/freertos.h"
#include "a8/hal/native.h"
#include "a8/util/LoggerFactory.h"
#include "a8/util/comp.h"
#include "test/TestSystem.h"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

using namespace a8::util;
using namespace a8::util::comp;
using namespace a8::util::sys;
using namespace a8::hal::native;
using namespace a8::hal;
using namespace testing;
using namespace test;
using namespace a8::hal::freertos;

System *a8::hal::S = new TestSystem();
class Base {

};
class MyCom : public Base, public Component {
public:
    MyCom(String name) : Component(name) {
    }
};

class Runner {
public:
    StagingContext *staging;
    MyCom *com;
};

void run(void *context) {
    Runner *runner = static_cast<Runner *>(context);
    MyCom *com1 = runner->com;
    MyCom *com2 = new MyCom("2");
    MyCom *com3 = new MyCom("3");
    com2->addChild(runner->staging, com3);

    com1->addChild(runner->staging, com2);
    com1->deleteChild(com2);
}

TEST(TestBuffer, testParse2) {
    Scheduler *sch = new FreeRtosScheduler();

    MyCom *com1 = new MyCom("1");
    LoggerFactory *logFac = new NativeLoggerFactory();
    StagingContext *sc = new StagingContext(sch, logFac, a8::hal::S);
    com1->stageTo(Boot, sc);
    Runner *runner = new Runner();
    runner->staging = sc;
    runner->com = com1;
    sch->schedule(run, runner);
    sch->startSchedule();
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}
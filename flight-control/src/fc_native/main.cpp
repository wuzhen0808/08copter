//clang-format off
#include <winsock2.h>
//clang-format on

#include "a8/fc/native.h"
#include "a8/hal.h"
#include "a8/hal/freertos.h"
#include "a8/hal/native.h"
#include "a8/link.h"
#include "a8/util.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace a8::hal;
using namespace a8::hal::native;
using namespace a8::fc;
using namespace a8::fc::native;
using namespace a8::link;
using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::sched;
using namespace a8::util::comp;
using namespace a8::hal::freertos;

System *a8::hal::S = new NativeSystem();

System &SR = *S;

int main(int argc, char **argv) {
    Scheduler *sch = new FreeRtosScheduler();
    Sockets *sockets = new NativeSockets();
    LoggerFactory *logFac = new NativeLoggerFactory(S);
    Links *links = new Links(sockets, sch, logFac);
    FlightControl *fc = new NativeFlightControl(argc, argv, links);
    StagingContext *context = new StagingContext(sch,       //
                                                 logFac,    //
                                                 a8::hal::S //
    );
    Application::start("appFc", context, fc);
    if (context->isStop()) {
        return 0;
    }
    sch->startSchedule();
}

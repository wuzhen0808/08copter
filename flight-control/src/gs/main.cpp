//clang-format off
// #include <winsock2.h>
//clang-format on

#include "a8/gs.h"
#include "a8/hal.h"
#include "a8/hal/freertos.h"
#include "a8/hal/native.h"
#include "a8/link.h"
#include "a8/util/comp.h"

using namespace a8::link;
using namespace a8::util;
using namespace a8::util::comp;
using namespace a8::hal;
using namespace a8::hal::native;
using namespace a8::gs;
using namespace a8::hal::freertos;
using namespace a8::util::schedule;

System *a8::hal::S = new NativeSystem();

int main(int argc, char **argv) {
    Scheduler *sch = new FreeRtosScheduler();
    LoggerFactory *logFac = new NativeLoggerFactory(S);
    Sockets * sockets = new NativeSockets();
    Application::start("appGs", new StagingContext(sch,    //
                                                   logFac, //
                                                   a8::hal::S),
                       new GroundStation(argc,                          //
                                         argv,                          //
                                         new Links(sockets, //
                                                   sch,                 //
                                                   logFac               //
                                                   )                    //
                                         )                              //
    );
    // note, this method is blocking.
    // until sch->endSchedule() is called.
    logFac->getLogger("default")->info("before startScheduler");
    sch->startSchedule();
    logFac->getLogger("default")->info("after startScheduler");
    return 0;
}


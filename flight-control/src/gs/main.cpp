//clang-format off
#include <winsock2.h>
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

System *a8::hal::S = new NativeSystem();

int main(int argc, char **argv) {
    Scheduler *sch = new FreeRtosScheduler();
    Application::start(new Context(sch,                      //
                                   new NativeLoggerFactory() //
                                   ),
                       new GroundStation(argc,                         //
                                         argv,                         //
                                         new Links(new NativeSockets() //
                                                   )                   //
                                         )                             //
    );
    sch->startSchedule();
}

#include "a8/link.h"
#include "a8/hal.h"
#include "a8/util.h"
#include "a8/hal/native.h"
#include "a8/gs.h"
#include "a8/hal/freertos.h"

using namespace a8::link;
using namespace a8::util;
using namespace a8::hal;
using namespace a8::hal::native;
using namespace a8::gs;
using namespace a8::hal::freertos;

System *a8::hal::S = new NativeSystem();

int main(int argc, char **argv) {
    Sockets *sockets = new NativeSockets();    
    Links* links = new Links(sockets);
    GroundStation *gs = new GroundStation(argc, argv, links);
    Scheduler *scheduler = new FreeRtosScheduler();
    LoggerFactory *logFac = new NativeLoggerFactory();
    Application *app = new Application("app", scheduler, logFac);
    app->start(new Properties());
}

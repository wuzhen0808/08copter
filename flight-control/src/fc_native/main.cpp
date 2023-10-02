#include <winsock2.h>
#include "a8/hal.h"
#include "a8/hal/System.h"
#include "a8/hal/native.h"
#include "a8/fc/native.h"
#include "a8/util.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace a8::hal;
using namespace a8::hal::native;
using namespace a8::fc;
using namespace a8::fc::native;
using namespace a8::util;
using namespace a8::util::net;
using namespace a8::hal::freertos;

System *a8::hal::S = new NativeSystem();

int main(int argc, char **argv) {
    Sockets * sockets = new NativeSockets();    
    FlightControl *fc = new NativeFlightControl(argc, argv, sockets);
    Scheduler* scheduler = new FreeRtosScheduler();
    LoggerFactory* logFac = new NativeLoggerFactory();
    Application * app = new Application("app", scheduler, logFac);
    app->start(new Properties());

    
}

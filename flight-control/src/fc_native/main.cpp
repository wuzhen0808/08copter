#include <winsock2.h>
#include "a8/hal/Hal.h"
#include "a8/hal/System.h"
#include "a8/hal/native/NativeFlightControl.h"
#include "a8/hal/native/NativeFileReader.h"
#include "a8/hal/native/NativeSystem.h"
#include "a8/net/GsStub.h"
#include "a8/util/Properties.h"
#include "a8/util/Application.h"
#include "a8/hal/native/NativeSockets.h"
#include "a8/freertos/FreeRtosScheduler.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace a8::hal;
using namespace a8::hal::native;
using namespace a8::net;
using namespace a8::freertos;

System *a8::hal::S = new NativeSystem();

int main(int argc, char **argv) {
    Sockets * sockets = new NativeSockets();    
    FlightControl *fc = new NativeFlightControl(argc, argv, sockets);
    Scheduler* scheduler = new FreeRtosScheduler();
    LoggerFactory* logFac = new NativeLoggerFactory();
    Application * app = new Application("app", scheduler, logFac);
    app->start(new Properties());

    
}

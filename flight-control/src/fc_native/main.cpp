//clang-format off
#include <winsock2.h>
//clang-format on

#include "a8/fc/native.h"
#include "a8/hal.h"
#include "a8/hal/System.h"
#include "a8/hal/native.h"
#include "a8/link.h"
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
using namespace a8::hal::freertos;

System *a8::hal::S = new NativeSystem();

int main(int argc, char **argv) {
    Sockets *sockets = new NativeSockets();
    Links *links = new Links(sockets);
    Application::start(new Context(new FreeRtosScheduler(),  //
                                   new NativeLoggerFactory() //
                                   ),
                       new NativeFlightControl(argc, //
                                               argv, //
                                               links,
                                               sockets //
                                               )       //
    );
}

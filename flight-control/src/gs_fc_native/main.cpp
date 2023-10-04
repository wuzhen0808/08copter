//clang-format off
#include <winsock2.h>
//clang-format on

#include "a8/fc.h"
#include "a8/fc/native.h"
#include "a8/gs.h"
#include "a8/hal.h"
#include "a8/hal/freertos.h"
#include "a8/hal/native.h"
#include "a8/link.h"
#include "a8/util.h"

using namespace a8::link;
using namespace a8::util;
using namespace a8::util::thread;
using namespace a8::hal;
using namespace a8::hal::native;
using namespace a8::gs;
using namespace a8::fc;
using namespace a8::fc::native;
using namespace a8::hal::freertos;

System *a8::hal::S = new NativeSystem();
struct AppRunner {
    Scheduler *sch;
    int argc;
    char **argv;
    bool isReady = false;
};

void runGs(void *ar) {
    AppRunner *runner = static_cast<AppRunner *>(ar);

    Application::start("appGs", new StagingContext(new FreeRtosScheduler(),  //
                                            new NativeLoggerFactory() , //
                                            a8::hal::S 
                                            ),
                       new GroundStation(runner->argc,                 //
                                         runner->argv,                 //
                                         new Links(new NativeSockets() //
                                                   )                   //
                                         )                             //
    );
    runner->isReady = true;
}
void runFc(void *ar) {
    AppRunner *runner = static_cast<AppRunner *>(ar);
    Sockets *sockets = new NativeSockets();
    Links *links = new Links(sockets);
    Application::start("appFc", new StagingContext(new FreeRtosScheduler(),  //
                                            new NativeLoggerFactory() , //
                                            a8::hal::S 
                                            ),
                       new NativeFlightControl(runner->argc, //
                                               runner->argv, //
                                               links,
                                               sockets //
                                               )       //
    );
    runner->isReady = true;
}

int main(int argc, char **argv) {
    Scheduler *sch = new FreeRtosScheduler();
    AppRunner *ar1 = new AppRunner();
    ar1->argc = argc;
    ar1->argv = argv;
    ar1->sch = sch;

    AppRunner *ar2 = new AppRunner();
    ar2->argc = argc;
    ar2->argv = argv;
    ar2->sch = sch;
    //
    sch->schedule(runGs, ar2);
    sch->schedule(runFc, ar1);
    sch->startSchedule();
    //

    while (!ar1->isReady || !ar2->isReady) {
        cout << "sleep second." << endl;
        S->sleep(1000);
    }
    //
    cout << "Start schedule." << endl;
}

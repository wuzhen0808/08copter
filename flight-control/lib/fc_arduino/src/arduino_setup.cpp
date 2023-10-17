#include "a8/hal.h"
#include "a8/fc/arduino.h"
#include "a8/hal.h"
#include "a8/hal/arduino.h"
#include "a8/hal/freertos.h"
#include "a8/link.h"
#include "a8/util.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"
using namespace a8::hal::freertos;
using namespace a8::hal::arduino;
using namespace a8::fc::arduino;
void setup_() {
System *S = new a8::hal::arduino::ArduinoSystem();
Scheduler *scheduler = new FreeRtosScheduler();
    
    using namespace a8::hal::freertos;

    Scheduler *sch = new FreeRtosScheduler();
    Sockets *sockets = new ArduinoSockets();
    LoggerFactory *logFac = new a8::hal::arduino::ArduinoLoggerFactory(S);
    Links *links = new Links(sockets, sch, logFac);
    FlightControl *fc = new ArduinoFlightControl(links);
    StagingContext *context = new StagingContext(sch,       //
                                                 logFac,    //
                                                 a8::hal::S //
    );
    Application::start("appFc", context, fc);
    if (context->isStop()) {
        return;
    }
    sch->startSchedule();
}

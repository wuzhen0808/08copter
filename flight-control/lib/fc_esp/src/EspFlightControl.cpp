#include "a8/fc/esp/EspFlightControl.h"
#include "a8/fc/esp/EspAttitudeSensor.h"
#include "a8/fc/esp/EspServos.h"
namespace a8::fc::esp {
using namespace a8::fc;
using namespace a8::util;
using namespace a8::util::sched;

EspFlightControl::EspFlightControl(Links *links) : FlightControl("fcs", 0, 0, links, 4) {
    this->servoAttachPins = new int[4]{3, 5, 9, 11};
}

EspFlightControl::~EspFlightControl() {
    delete[] this->servoAttachPins;
}

void EspFlightControl::populate(StagingContext *context) {
    servosControl_ = new EspServosControl(totalServos_, context->loggerFactory);
    attitudeSensor_ = new EspAttitudeSensor(context->loggerFactory);
    FlightControl::populate(context);
}

void EspFlightControl::start(StagingContext *context) {
    FlightControl::start(context);
}

} // namespace a8::fc::arduino_
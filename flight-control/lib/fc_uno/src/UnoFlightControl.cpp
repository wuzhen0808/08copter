#include "a8/fc/uno/UnoFlightControl.h"
#include "a8/fc/uno/UnoAttitudeSensor.h"
#include "a8/fc/uno/UnoServosControl.h"
#include "a8/hal/freertos.h"
namespace a8::fc::uno {
using namespace a8::fc;
using namespace a8::util;
using namespace a8::util::schedule;

UnoFlightControl::UnoFlightControl(Links *links) : FlightControl("fcs", 0, 0, links, 4) {
    this->servoAttachPins = new int[4]{3, 5, 9, 11};
}

UnoFlightControl::~UnoFlightControl() {
    delete[] this->servoAttachPins;
}

void UnoFlightControl::populate(StagingContext *context) {
    servosControl_ = new UnoServosControl(totalServos_, context->loggerFactory);
    attitudeSensor_ = new UnoAttitudeSensor(context->loggerFactory);
    FlightControl::populate(context);
}

void UnoFlightControl::start(StagingContext *context) {
    FlightControl::start(context);
}

} // namespace a8::fc::arduino_
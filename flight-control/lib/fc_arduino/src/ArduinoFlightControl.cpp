#include "a8/fc/arduino/ArduinoFlightControl.h"
#include "a8/fc/arduino/ArduinoAttitudeSensor.h"
#include "a8/fc/arduino/ArduinoServosControl.h"
#include "a8/hal/freertos.h"
#include <Arduino.h>
using namespace a8::fc;
using namespace a8::util;
using namespace a8::util::schedule;
using namespace a8::hal::freertos;

namespace a8::fc::arduino_ {

ArduinoFlightControl::ArduinoFlightControl(Hal*hal, Links *links) : FlightControl("fcs", 0, 0, links, 4) {
    this->servoAttachPins = new int[4]{3, 5, 9, 11};
    this->hal = hal;
}

ArduinoFlightControl::~ArduinoFlightControl() {
    delete[] this->servoAttachPins;
}

void ArduinoFlightControl::populate(StagingContext *context) {
    servosControl_ = new ArduinoServosControl(totalServos_, context->loggerFactory);
    attitudeSensor_ = new ArduinoAttitudeSensor(this->hal, context->loggerFactory);
    FlightControl::populate(context);
}

void ArduinoFlightControl::start(StagingContext *context) {
    FlightControl::start(context);
}

} // namespace a8::fc::arduino_
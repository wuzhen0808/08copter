
//RF24 need this include 
#include <Arduino.h>
#include "a8/ts/esp/EspTransmitter.h"

namespace a8::ts::esp {

using namespace a8::ts;
using namespace a8::util;
using namespace a8::util::schedule;
using namespace a8::hal::arduino_;

EspTransmitter::EspTransmitter(ArduinoHal *arduino, Links *links) : Transmitter("ts", 0, 0, links) {
    this->arduino = arduino;
}

EspTransmitter::~EspTransmitter() {
}

void EspTransmitter::populate(StagingContext *context) {
    Transmitter::populate(context);
}

void EspTransmitter::start(StagingContext *context) {
    Transmitter::start(context);
}

} // namespace a8::ts::esp
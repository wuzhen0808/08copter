#pragma once
#include "a8/hal/arduino/ArduinoHal.h"
#include "a8/link.h"
#include "a8/ts/Transmitter.h"
#include "a8/util/schedule.h"

namespace a8::ts::esp {
using namespace a8::ts;
using namespace a8::hal::arduino_;

class EspTransmitter : public Transmitter {
private:
    ArduinoHal *arduino;

public:
    EspTransmitter(ArduinoHal *arduino, Links *links);
    ~EspTransmitter();
    void populate(StagingContext *context) override;
    void start(StagingContext *context) override;
};

} // namespace a8::ts::esp

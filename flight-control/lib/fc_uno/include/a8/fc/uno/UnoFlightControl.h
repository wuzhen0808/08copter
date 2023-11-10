#pragma once
#include "a8/fc.h"
#include "a8/hal/arduino/Hal.h"
#include "a8/link.h"
#include "a8/util/schedule.h"
namespace a8::fc::arduino_ {
using namespace a8::fc;
using namespace a8::hal::arduino_;

class UnoFlightControl : public FlightControl {
private:
    int *servoAttachPins;
    Hal *hal;

public:
    UnoFlightControl(Hal *hal, Links *links);
    ~UnoFlightControl();
    void populate(StagingContext *context) override;
    void start(StagingContext *context) override;
};

} // namespace a8::fc::arduino_

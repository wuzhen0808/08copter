#pragma once
#include "a8/fc.h"
#include "a8/link.h"
#include "a8/util/sched.h"
namespace a8::fc::esp {
using namespace a8::fc;

class EspFlightControl : public FlightControl {
private:
    int *servoAttachPins;

public:
    EspFlightControl(Links *links);
    ~EspFlightControl();
    void populate(StagingContext *context) override;
    void start(StagingContext *context) override;
};

} // namespace a8::fc::arduino_

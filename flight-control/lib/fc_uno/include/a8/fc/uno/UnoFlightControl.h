#pragma once
#include "a8/fc.h"
#include "a8/link.h"
#include "a8/util/sched.h"
namespace a8::fc::uno {
using namespace a8::fc;

class UnoFlightControl : public FlightControl {
private:
    int *servoAttachPins;

public:
    UnoFlightControl(Links *links);
    ~UnoFlightControl();
    void populate(StagingContext *context) override;
    void start(StagingContext *context) override;
};

} // namespace a8::fc::arduino_

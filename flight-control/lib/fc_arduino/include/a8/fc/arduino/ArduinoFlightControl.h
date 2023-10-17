#pragma once
#include "a8/fc.h"
#include "a8/util/thread.h"
#include "a8/link.h"

using namespace a8::fc;

namespace a8::fc::arduino {

class ArduinoFlightControl : public FlightControl {
private:
    int *servoAttachPins;

public:
    ArduinoFlightControl(Links *links);
    ~ArduinoFlightControl();
    void populate(StagingContext *context) override;
    void start(StagingContext *context) override;
};

} // namespace a8::fc::arduino

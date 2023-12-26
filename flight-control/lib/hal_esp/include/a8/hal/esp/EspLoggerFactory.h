#pragma once

#include "a8/hal/arduino.h"
namespace a8::hal::esp {
using namespace a8::hal::arduino_;
class EspLoggerFactory : public ArduinoLoggerFactory {
public:
    EspLoggerFactory(System *sys, Scheduler *sch) : ArduinoLoggerFactory(sys, sch, 0) {
    }
};

} // namespace a8::hal::esp

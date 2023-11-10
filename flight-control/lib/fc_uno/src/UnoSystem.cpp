#include "a8/fc/arduino/ArduinoSystem.h"
#include "a8/fc/arduino/ArduinoOutput.h"
#include <Arduino.h>

namespace a8::fc::arduino_ {

UnoSystem::UnoSystem() {
    out = new UnoOutput();
}

long UnoSystem::millis() {
    return ::millis();
}

} // namespace a8::arduino
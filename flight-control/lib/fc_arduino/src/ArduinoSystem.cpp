#include "a8/fc/arduino/ArduinoSystem.h"
#include "a8/fc/arduino/ArduinoOutput.h"
#include <Arduino.h>

namespace a8::fc::arduino_ {

ArduinoSystem::ArduinoSystem() {
    out = new ArduinoOutput();
}

long ArduinoSystem::millis() {
    return ::millis();
}

} // namespace a8::arduino
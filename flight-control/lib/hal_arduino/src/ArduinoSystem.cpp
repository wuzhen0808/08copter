#include "a8/arduino/ArduinoSystem.h"
#include "a8/arduino/ArduinoOutput.h"
#include <Arduino.h>

namespace a8::arduino {

ArduinoSystem::ArduinoSystem() {
    out = new ArduinoOutput();
}

long ArduinoSystem::millis() {
    return ::millis();
}

} // namespace a8::arduino
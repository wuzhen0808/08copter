#include "a8/arduino/ArduinoSystem.h"
#include <Arduino.h>

namespace a8::arduino {

long ArduinoSystem::millis() {
    return ::millis();
}

} // namespace a8::arduino
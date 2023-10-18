#include "a8/hal/arduino/ArduinoOutput.h"
#include <Arduino.h>

namespace a8::hal::arduino {
using a8::util::Output;
using a8::util::String;

void ArduinoOutput::print(const String msg) {
    Serial.print(msg.text());
}

} // namespace a8::hal::native

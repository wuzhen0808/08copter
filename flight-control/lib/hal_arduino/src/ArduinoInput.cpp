#include "a8/hal/arduino/ArduinoInput.h"
#include <Arduino.h>

namespace a8::hal::arduino_ {
using namespace a8::util;
ArduinoInput::ArduinoInput() {
    while (Serial.available()) {
        Serial.read();
    }
}

ArduinoInput::~ArduinoInput() {
}

int ArduinoInput::read(char *buf, int bufLen, Result &res) {
    while (!Serial.available()) {
        delay(10);
    }
    buf[0] = Serial.read();
    return 1;
}
} // namespace a8::hal::arduino_

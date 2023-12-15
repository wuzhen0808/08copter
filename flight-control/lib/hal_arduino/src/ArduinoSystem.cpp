#include "a8/hal/arduino/ArduinoSystem.h"
#include "a8/hal/arduino/ArduinoInput.h"
#include "a8/hal/arduino/ArduinoOutput.h"
#include <Arduino.h>

namespace a8::hal::arduino_ {
using namespace a8::util;
ArduinoSystem::ArduinoSystem() {
    out = new ArduinoOutput();
    input = new ArduinoInput();
}

long long ArduinoSystem::getSteadyTime() {
    return millis();
}

long long ArduinoSystem::getSysTime() {
    return millis();
}

void ArduinoSystem::formatTime(long long ms, String *str) {
    str->append(ms);
}

int ArduinoSystem::openFile(String fpath, Reader *&fReaderCb) {
    return -1;
}

String ArduinoSystem::getEnv(String name) {
    return "";
}
void ArduinoSystem::setPinMode(int pin, PinMode mode) {
    if (mode == PinMode::IN) {
        ::pinMode(pin, INPUT);
    } else if (mode == PinMode::OUT) {
        ::pinMode(pin, OUTPUT);
    } else {
    }
}
bool ArduinoSystem::digitalRead(int pin) {
    return LOW != ::digitalRead(pin);
}

void ArduinoSystem::digitalWrite(int pin, bool high) {
    ::digitalWrite(pin, high ? HIGH : LOW);
}

int ArduinoSystem::analogRead(int pin) {
    return ::analogRead(pin);
}
void ArduinoSystem::delay(long ms) {
    ::delay(ms);
}
} // namespace a8::hal::arduino_
#include "a8/hal/arduino/ArduinoSystem.h"
#include "a8/hal/arduino/ArduinoOutput.h"
#include <Arduino.h>

namespace a8::hal::arduino_ {
using namespace a8::util;
ArduinoSystem::ArduinoSystem() {
    out = new ArduinoOutput();
    
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
} // namespace a8::hal::arduino_
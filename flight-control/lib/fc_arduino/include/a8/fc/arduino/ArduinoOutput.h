#pragma once
#include "a8/util.h"
#include <Arduino.h>

using namespace a8::util;

namespace a8::fc::arduino_ {

class ArduinoOutput : public Output {
    void print(a8::util::String msg) {
        Serial.write(msg.text(), msg.length());
    }
};

} // namespace a8::native

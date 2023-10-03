#pragma once
#include "a8/util.h"
#include <Arduino.h>

using namespace a8::util;

namespace a8::fc::arduino {

class ArduinoOutput : public Output {
    void print(a8::util::String msg) {
        Serial.write(msg.getText(), msg.getLength());
    }
};

} // namespace a8::native

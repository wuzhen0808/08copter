#pragma once
#include "a8/util/Output.h"
#include <Arduino.h>

using namespace std;

namespace a8::arduino {
    
class ArduinoOutput : public Output {
    void print(a8::util::String msg) {
        Serial.write(msg.getText(), msg.getLength());
    }
};

} // namespace a8::native

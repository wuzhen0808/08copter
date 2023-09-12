#pragma once
#include "a8/util/Output.h"
#include <Arduino.h>

using namespace std;
using namespace a8::util;
namespace a8::arduino {
    
class ArduinoOutput : public Output {
    void print(String msg) {
        Serial.print(msg.getText());
    }
}

} // namespace a8::native

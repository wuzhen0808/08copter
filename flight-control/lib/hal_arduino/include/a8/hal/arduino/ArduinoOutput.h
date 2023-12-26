#pragma once
#include "a8/util/Output.h"
#include <Arduino.h>
namespace a8::hal::arduino_ {
using a8::util::Output;
using a8::util::String;
Output *createOutput(int serial);

Output *createOutput();
class ArduinoOutput : public Output {

public:
    void print(const String msg) override {
        A8_TRACE2(">>ArduinoOutput::print,msg:", msg.text());
        Serial.print(msg.text());
        A8_TRACE("<<ArduinoOutput::print");
    }
};

class ArduinoOutput1 : public Output {

public:
    void print(const String msg) override {
        A8_TRACE2(">>ArduinoOutput1::print,msg:", msg.text());
        Serial.print(msg.text());
        A8_TRACE("<<ArduinoOutput1::print");
    }
};

} // namespace a8::hal::arduino_

#pragma once
#include "a8/util/Output.h"
namespace a8::hal::arduino {
using a8::util::Output;
using a8::util::String;

class ArduinoOutput : public Output {
    virtual void print(const String msg) override;
};

} // namespace a8::hal::arduino

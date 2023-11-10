#pragma once
#include "a8/util.h"
#include <Arduino.h>

namespace a8::fc::arduino_ {
using namespace a8::util;


class UnoOutput : public Output {
    void print(a8::util::String msg) {
        Serial.write(msg.text(), msg.length());
    }
};

} // namespace a8::native

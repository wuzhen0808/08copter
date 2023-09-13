#pragma once
#include "a8/hal/System.h"
#include <Arduino.h>

using a8::hal::System;

namespace a8::arduino {

class ArduinoSystem : public System {
public:
    ArduinoSystem();
    virtual long millis();
};
} // namespace a8::arduino
#pragma once
#include "a8/hal/System.hpp"
#include <Arduino.h>

using a8::hal::System;

namespace a8::arduino {

class ArduinoSystem : public System {
    virtual long millis();
};
} // namespace a8::arduino
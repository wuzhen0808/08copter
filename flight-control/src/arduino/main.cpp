#include "a8/arduino/ArduinoCopter.h"
#include "a8/arduino/ArduinoSystem.h"
#include "a8/hal/Hal.h"

using a8::arduino::ArduinoCopter;
using a8::arduino::ArduinoSystem;
using a8::hal::System;


System * a8::hal::S = new ArduinoSystem();

void setup() {
    S->out->println("hello");
    Copter *copter = new ArduinoCopter();
    copter->start();
}
void loop() {}

#include "a8/hal/Hal.hpp"
#include "a8/arduino/ArduinoCopter.h"
#include "a8/arduino/ArduinoSystem.h"
using a8::arduino::ArduinoCopter;
using a8::arduino::ArduinoSystem;

System* a8::hal::S = new ArduinoSystem();

void setup() {
    Copter *copter = new ArduinoCopter();
    copter->start();
}
void loop() {}

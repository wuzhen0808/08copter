#include "a8/hal/Hal.h"
#include "a8/arduino/ArduinoCopter.h"
#include "a8/arduino/ArduinoSystem.h"
#include "a8/hal/freertos/FreeRtosScheduler.h"

using a8::arduino::ArduinoCopter;
using a8::arduino::ArduinoSystem;
using a8::hal::System;
using a8::hal::freertos::FreeRtosScheduler;
using AString = a8::util::String;

System * a8::hal::S = new ArduinoSystem();
Scheduler* scheduler = new FreeRtosScheduler();
void setup() {
    
    Serial.begin(9600);
    while (!Serial) {
    }
    S->out->println("::setup()");
    Copter *copter = new ArduinoCopter(scheduler);
    copter->start();
}
void loop() {}

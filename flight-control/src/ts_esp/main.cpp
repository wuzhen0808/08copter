#include "a8/hal/arduino.h"
#include "a8/util.h"
#include "setup_.h"
#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
using namespace a8::util;
using namespace a8::hal::arduino_;

ArduinoHal *setupHal() {
    ArduinoHal *hal = new ArduinoHal();
    Serial.begin(9600);
    
    delay(1000);
    return hal;
}
void setup() {
    ArduinoHal *hal = setupHal();
    Serial.println(">>setup");

    // setup_(0, [](void *ctx) {
    //     Serial.println("...");
    // });
    setup_(hal);
    Serial.println("<<setup");
    // Serial.begin(9600);
    // delay(1000);
    // Serial.println("hello");
}

void loop() {
    //Serial.print("loop");
    delay(1000);
}
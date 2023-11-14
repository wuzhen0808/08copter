#include "a8/hal/arduino.h"
#include "a8/util.h"
#include "setup.h"
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
using namespace a8::util;

void setup() {
    Serial.begin(9600);

    delay(1000);
    Serial.println(">>setup");

    // setup_(0, [](void *ctx) {
    //     Serial.println("...");
    // });
    Result res;
    int ret = setup(res);
    Serial.println("<<setup");
    // Serial.begin(9600);
    // delay(1000);
    // Serial.println("hello");
}

void loop() {
    // Serial.print("loop");
    delay(1000);
}
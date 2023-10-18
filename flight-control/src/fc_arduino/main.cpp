#include "a8/util.h"
#include "setup_.h"
#include <Arduino.h>
using namespace a8::util;

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println(">>setup");

    // setup_(0, [](void *ctx) {
    //     Serial.println("...");
    // });
    setup_();
    Serial.println("<<setup");
    // Serial.begin(9600);
    // delay(1000);
    // Serial.println("hello");
}

void loop() {
}
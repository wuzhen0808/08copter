#include "a8/hal/arduino.h"
#include "a8/util.h"
#include "setup.h"
#include <Arduino.h>
#include <MPU9250.h>
#include <Wire.h>

void setup() {
    using a8::util::String;
    using namespace a8::util;
    Serial.begin(115200);
    while(Serial.available()){
        Serial.read();
    }
    Wire.begin();

    delay(1000);
    Serial.println(">>setup");

    Result res;
    int ret = setup(res);
    if (ret < 0) {
        Serial.println(res.errorMessage.text());
    }
    Serial.println("<<setup");   
}

void loop() {
}
#include "a8/hal/arduino.h"
#include "a8/util.h"
#include "setup.h"
#include <Arduino.h>
#include <MPU9250.h>
#include <Wire.h>

void setup() {
    using a8::util::String;
    using namespace a8::util;
    Serial.begin(9600);
    Wire.begin();

    // MPU9250_<HardwareI2C> *mpu = new MPU9250_<HardwareI2C>();
    // hal->mpu9250 = new Mpu9250Wrapper(mpu);
    // hal->mpu9250->setup(0x68, hal->i2c->i2c);

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
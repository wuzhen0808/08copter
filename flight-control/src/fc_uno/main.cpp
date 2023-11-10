#include "a8/hal/arduino.h"
#include "a8/util.h"
#include "setup_.h"
#include <Arduino.h>
#include <MPU9250.h>
#include <Wire.h>
using namespace a8::util;
using namespace a8::hal::arduino_;

Hal *setupHal() {
    Hal *hal = new Hal();
    hal->serial = new HardwareSerialWrapper(&Serial);
    hal->serial->begin(9600);
    
    hal->i2c = new HardwareI2CWrapper(0);//TODO
    Wire.begin();
    
    MPU9250_<HardwareI2C> *mpu = new MPU9250_<HardwareI2C>();
    hal->mpu9250 = new Mpu9250Wrapper(mpu);
    hal->mpu9250->setup(0x68, hal->i2c->i2c);
    
    delay(1000);
    return hal;
}
void setup() {
    Hal *hal = setupHal();
    hal->serial->println(">>setup");

    // setup_(0, [](void *ctx) {
    //     Serial.println("...");
    // });
    setup_(hal);
    hal->serial->println("<<setup");
    // Serial.begin(9600);
    // delay(1000);
    // Serial.println("hello");
}

void loop() {
}
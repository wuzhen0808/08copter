#include "MPU9250.h"

MPU9250 mpu; // You can also use MPU9255 as is

void setup() {
    Serial.begin(9600);
    Wire.begin();
    delay(2000);

    mpu.setup(0x68);  // change to your own address
}
unsigned long lastSend;
void loop() {
    if (mpu.update()) {
        unsigned long now = millis();
        if(now - lastSend > 10){
          lastSend = now;
          Serial.print(mpu.getYaw()); Serial.print(",");
          Serial.print(mpu.getPitch()); Serial.print(",");
          Serial.print(mpu.getRoll()); Serial.print("\r\n");
        }
    }
}
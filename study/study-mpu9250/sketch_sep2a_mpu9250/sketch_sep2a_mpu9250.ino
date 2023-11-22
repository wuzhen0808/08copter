//https://github.com/hideakitai/MPU9250/tree/master

#include "MPU9250.h"
#define MODE_ROLL 0
#define MODE_ACC 1
#define MODE_GYRO 2
MPU9250 mpu; // You can also use MPU9255 as is

void setup() {
    Serial.begin(9600);
    Wire.begin();
    delay(2000);

    mpu.setup(0x68);  // change to your own address
}
unsigned long lastSend;
int mode = MODE_GYRO;
void loop() {
    if (mpu.update()) {
        unsigned long now = millis();
        if(now - lastSend > 1){
          lastSend = now;
          float a = mpu.getRoll();
          float b = mpu.getPitch();
          float c = mpu.getYaw();
          if(mode == MODE_ACC) {
            a = mpu.getAccX();
            b = mpu.getAccY();
            c = mpu.getAccZ();
          } else if(mode == MODE_GYRO) {
            a = mpu.getGyroX();
            b = mpu.getGyroY();
            c = mpu.getGyroZ();
          }
          Serial.print(a); Serial.print(",");
          Serial.print(b); Serial.print(",");
          Serial.print(c); Serial.print("\r\n");

        }
    }
}
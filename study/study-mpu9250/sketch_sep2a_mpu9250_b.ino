// https://github.com/hideakitai/MPU9250

#include "MPU9250.h"

MPU9250 mpu; // You can also use MPU9255 as is

void setup() {
    Serial.begin(115200);
    Wire.begin();
    delay(2000);

    mpu.setup(0x68);  // change to your own address
    mpu.selectFilter(QuatFilterSel::MADGWICK);
    mpu.setFilterIterations(10);
}

void loop() {
    if (mpu.update()) {
      
        Serial.print("(");
        Serial.print(mpu.getAccX());         Serial.print(", ");
        Serial.print(mpu.getAccY());         Serial.print(", ");
        Serial.print(mpu.getAccZ());         Serial.print(");");
        Serial.print("(");
        Serial.print(mpu.getGyroX());        Serial.print(", ");
        Serial.print(mpu.getGyroY());        Serial.print(", ");
        Serial.print(mpu.getGyroZ());        Serial.print(");");
        Serial.print("(");
        Serial.print(mpu.getMagX());         Serial.print(", ");
        Serial.print(mpu.getMagY());         Serial.print(", ");
        Serial.print(mpu.getMagZ());         Serial.print("); ");
        Serial.print("(");
        Serial.print(mpu.getQuaternionX());         Serial.print(", ");
        Serial.print(mpu.getQuaternionY());       Serial.print(", ");
        Serial.print(mpu.getQuaternionZ());        Serial.print(");");
        Serial.print("(");
        Serial.print(mpu.getYaw());         Serial.print(", ");
        Serial.print(mpu.getPitch());       Serial.print(", ");
        Serial.print(mpu.getRoll());        Serial.print(");");
        Serial.print("(temperature:");
        Serial.print(mpu.getTemperature());       Serial.print(");");


    } else {
      Serial.print("No update.");
    }
    Serial.println("");
}